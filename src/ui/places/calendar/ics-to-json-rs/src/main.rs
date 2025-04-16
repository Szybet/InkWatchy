extern crate ical;
// KNOWN BUGS
// if there are multiple events on same day from different calendars, it uses wrong title
// do multiple events on same day work? no idea

use std::collections::HashMap;
use std::fs::File;
use std::io::{BufReader, Read, Write};
use std::process::exit;
use std::str::FromStr;
use std::path::Path;

use log::*;

use chrono::{DateTime, NaiveDate, NaiveDateTime, TimeZone, Utc};
use chrono_tz::Tz as ChronoTz;
use clap::Parser;
use deunicode::deunicode;
use reqwest::blocking::Client;

use serde::{Deserialize, Serialize};

// importing for recurring events, otherwise not imported, when we are importing from today onwards
use rrule::{RRule, RRuleSet, Tz};
use regex::Regex; // trying this shit out

#[derive(Parser, Debug, Clone)]
#[command(author, version, about)]
pub struct Args {
    #[arg(short, long, help = "Path to the ics file")]
    path: Option<String>,
    #[arg(short, long, help = "Url to the ics file")]
    url: Option<String>,
    #[arg(
        short,
        long,
        help = "Path to the output dir (Inkwatchy littlefs filesystem). With / at the end"
    )]
    output_dir: String,
    #[arg(
        short,
        long,
        help = "Limit how many days to actually show",
        default_value_t = 20
    )]
    limit_days: usize,
}

fn normalize_rrule(rrule_str: &str) -> String {
    // This regex captures the UNTIL part regardless of 'Z'
    let re = Regex::new(r"(UNTIL=)(\d{8}T\d{6})(Z?)").unwrap();
    re.replace_all(rrule_str, |caps: &regex::Captures| {
        let prefix = &caps[1];
        let datetime = &caps[2];
        let z = &caps[3];
        if z.is_empty() {
            format!("{}{}Z", prefix, datetime)
        } else {
            caps[0].to_string()
        }
    }).to_string()
}

fn main() {
    env_logger::init_from_env(
        env_logger::Env::default().filter_or(env_logger::DEFAULT_FILTER_ENV, "debug"),
    );
    debug!("Start");

    let args = Args::parse();

    if let Some(path) = args.path.clone() {
        let mut string = String::new();
        BufReader::new(File::open(path).unwrap())
            .read_to_string(&mut string)
            .unwrap();
        parse_ical(string.as_bytes(), &args.clone());
    } else if let Some(url) = args.url.clone() {
        // HTTP client with User-Agent
        // Initialize the client builder with the user agent
        let mut builder = Client::builder().user_agent("ics-to-json/1.0");
    
        // Conditionally add the danger_accept_invalid_certs setting
        #[cfg(allow_insecure_cal)]
        {
            builder = builder.danger_accept_invalid_certs(true);
        }
    
        // Build the client
        let client = match builder.build() {
            Ok(c) => c,
            Err(e) => {
                error!("Failed to create HTTP client: {}", e);
                exit(1);
            }
        };

        // Send request
        let response = match client.get(&url).send() {
            Ok(resp) => resp,
            Err(e) => {
                error!("Failed to fetch URL '{}': {}", url, e);
                exit(1);
            }
        };

        // Read response body
        let body = match response.text() {
            Ok(text) => text,
            Err(e) => {
                error!("Failed to read response text: {}", e);
                exit(1);
            }
        };

        debug!("body: {}", body);
        parse_ical(body.as_bytes(), &args.clone());
    } else {
        error!("No ICS file provided");
    }
}


#[derive(Default, Clone, Debug, Serialize, Deserialize)]
pub struct Event {
    name: String,
    start_time: u64,
    end_time: u64,
    description: String,
    status: String,
}

pub fn parse_time(str: String, tzid: Option<&str>) -> DateTime<Utc> {
    debug!("Parsing time: {} with tzid: {:?}", str, tzid);
    let fmt = if str.contains('T') {
        if str.ends_with('Z') {
            "%Y%m%dT%H%M%SZ"
        } else {
            "%Y%m%dT%H%M%S"
        }
    } else {
        "%Y%m%d"
    };

    // Parse as naive datetime first
    let naive = NaiveDateTime::parse_from_str(&str, fmt)
        .or_else(|_| NaiveDate::parse_from_str(&str, "%Y%m%d").map(|d| d.and_hms_opt(0, 0, 0).unwrap()))
        .expect("Failed to parse date");

    // Convert to appropriate timezone
    match tzid {
        Some(tz) => {
            let tz: ChronoTz = tz.parse().unwrap_or(ChronoTz::UTC);
            tz.from_local_datetime(&naive)
                .single()
                .unwrap_or_else(|| tz.with_ymd_and_hms(1970, 1, 1, 0, 0, 0).unwrap())
                .with_timezone(&Utc)
        }
        None => {
            if str.ends_with('Z') {
                Utc.from_utc_datetime(&naive)
            } else {
                // Treat floating times as local time
                let local_tz = ChronoTz::Europe__Berlin; // Change this to your local timezone
                local_tz
                    .from_local_datetime(&naive)
                    .single()
//                    .unwrap_or_else(|| local_tz.ymd(1970, 1, 1).and_hms(0, 0, 0))
                    .unwrap_or_else(|| local_tz.with_ymd_and_hms(1970, 1, 1, 0, 0, 0).unwrap())
                    .with_timezone(&Utc)
            }
        }
    }
}

pub fn parse_ical(buf: &[u8], args: &Args) {
    let mut reader = ical::IcalParser::new(buf);
    let calendar = reader.next().unwrap().unwrap();

    // Calculate the cutoff time
    #[cfg(future_only)]
    let cutoff = Utc::now()
        .date_naive()
        .and_hms_opt(0, 0, 0)
        .unwrap()
        .and_utc()
        .timestamp() as u64;

    #[cfg(not(future_only))]
    let cutoff = 0; // Include all events

    let mut events: HashMap<String, Vec<Event>> = HashMap::new();
    let mut days: Vec<u64> = Vec::new();

    for event in calendar.events {
        debug!("Processing event: {:#?}", event);
        let mut evt = Event::default();
        let mut rrule_str = None;
        let mut dtstart = None;
        let mut dtend = None;
    
        for prop in event.properties {
            match prop.name.as_str() {
                "SUMMARY" => evt.name = prop.value.unwrap_or_default(),
                "DTSTART" => {
                    if let Some(value) = prop.value {
                        let tzid = prop.params.as_ref().and_then(|params| {
                            params.iter()
                                .find(|(k, _)| k == "TZID")
                                .and_then(|(_, v)| v.first())
                                .map(|s| s.as_str())  // Convert &String to &str here
                        });
                        let dt = parse_time(value, tzid);
                        dtstart = Some(dt);
                        evt.start_time = dt.timestamp() as u64;
                    }
                }
                "DTEND" => {
                    if let Some(value) = prop.value {
                        let tzid = prop.params.as_ref().and_then(|params| {
                            params.iter()
                                .find(|(k, _)| k == "TZID")
                                .and_then(|(_, v)| v.first())
                                .map(|s| s.as_str())  // Convert &String to &str here
                        });
                        let dt = parse_time(value, tzid);
                        dtend = Some(dt);
                        evt.end_time = dt.timestamp() as u64;
                    }
                }
                "RRULE" => {
                    if let Some(value) = prop.value {
                        rrule_str = Some(value);
                    }
                }
                "DESCRIPTION" => evt.description = prop.value.unwrap_or_default(),
                "STATUS" => evt.status = prop.value.unwrap_or_default(),
                _ => (),
            }
        }


          if let Some(rrule_str) = rrule_str {
                // Handle recurring events
                let dtstart = dtstart.expect("RRULE requires DTSTART");
                let duration = if let Some(end) = dtend {
                    end - dtstart
                } else {
                    chrono::Duration::days(1)
                };

           let normalized_rrule = normalize_rrule(&rrule_str);

            // Convert chrono DateTime to rrule DateTime (UTC)
            let start_rrule = dtstart.with_timezone(&Tz::UTC);
            
            // Parse and validate RRULE
            let original_tz = dtstart.timezone().into();
            let rrule = match RRule::from_str(&normalized_rrule) {
                Ok(r) => match r.validate(dtstart.with_timezone(&original_tz)) {
                    Ok(valid_rrule) => valid_rrule,
                    Err(e) => {
                        error!("Validation error for RRULE '{}': {:?}", normalized_rrule, e);
                        continue;
                    }
                },
                Err(e) => {
                    error!("Failed to parse RRULE '{}': {}", normalized_rrule, e);
                    continue;
                }
            }; 
            let mut rrule_set = RRuleSet::new(start_rrule);
            rrule_set = rrule_set.rrule(rrule);
        
            // Calculate max_date based on limit_days
            let max_date = cutoff + (args.limit_days as u64 * 86400);
            let max_date = DateTime::from_timestamp(max_date as i64, 0)
                .unwrap()
                .with_timezone(&Tz::UTC);
        
            // Generate occurrences between cutoff and max_date
            let result = rrule_set
                .after(DateTime::from_timestamp(cutoff as i64, 0).unwrap().with_timezone(&Tz::UTC))
                .before(max_date)
                .all(u16::MAX);
            
            // Check if we hit the limit (optional)
            if result.limited {
                warn!("Hit occurrence limit for recurring event");
            }
            
            // Use the 'dates' field instead of 'occurrences'
            for occurrence in &result.dates {
                let occurrence_start = *occurrence;
                let occurrence_end = occurrence_start + duration;
                let day = occurrence_start.format("%d.%m.%Y").to_string();
            
                if occurrence_start.timestamp() as u64 >= cutoff {
                    let new_evt = Event {
                        name: evt.name.clone(),
                        start_time: occurrence_start.timestamp() as u64,
                        end_time: occurrence_end.timestamp() as u64,
                        description: evt.description.clone(),
                        status: evt.status.clone(),
                    };
            
                    events.entry(day.clone())
                        .or_insert_with(Vec::new)
                        .push(new_evt.clone());
                    days.push(new_evt.start_time);
                }
            } 
        } else {
            // Handle non-recurring events
            let day = dtstart.map(|dt| dt.format("%d.%m.%Y").to_string()).unwrap_or_default();
            if !day.is_empty() && evt.start_time >= cutoff {
                events.entry(day)
                    .or_insert_with(Vec::new)
                    .push(evt.clone());
                days.push(evt.start_time);
            }
        }
    }
    debug!("Final hashmap: {:#?}", events);

    let json = serde_json::to_string_pretty(&events).unwrap();
    debug!("Json: \n{}", json);

    info!("There are {} days", events.len());
    for event in events {
        let path = format!("{}{}", args.output_dir, event.0);
        info!("Writing file: {}", path);
        let small_json = serde_json::to_string_pretty(&event.1).unwrap();
        let mut file = File::create(path).unwrap();

        let mut ascii_str = small_json;
        ascii_str = deunicode(&ascii_str);
        ascii_str = ascii_str.replace("    ", "\n");        
        ascii_str = jsonxf::pretty_print(&ascii_str).unwrap();

        file.write_all(ascii_str.as_bytes()).unwrap();
    }

    days.sort();
    debug!("Days: {:#?}", days);

    let mut index = String::new(); // << this line should stay here, not later

    let mut c = 0;
    for day in days {
        let time = DateTime::from_timestamp(day as i64, 0).unwrap();
        let time_str = &time.format("%d.%m.%Y").to_string();
        c += 1;
        if c > args.limit_days {
            let rm_path = format!("{}{}", args.output_dir, time_str);
            std::fs::remove_file(&rm_path).unwrap_or_else(|e| {
                warn!("Couldn't delete old file {}: {}", rm_path, e);
            });
            continue;
        }
        index.push_str(&day.to_string());
        index.push('\n');
    }

    // Move this logic *after* index is populated
    let base_filename = format!("{}index.txt", args.output_dir);
    let mut index_filename = base_filename.clone();
    let mut suffix = 0;
    while Path::new(&index_filename).exists() {
        suffix += 1;
        index_filename = format!("{}index.txt-{}", args.output_dir, suffix);
    }

    info!("Writing index file: {}", index_filename);
    let mut file = File::create(&index_filename).unwrap();
    file.write_all(index.to_ascii_lowercase().as_bytes()).unwrap();

    info!("Done, bye!");

}
