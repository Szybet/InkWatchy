extern crate ical;
// KNOWN BUGS
// if there are multiple events on same day from different calendars, it uses wrong title
// do multiple events on same day work? no idea

use std::collections::HashMap;
use std::fs::File;
use std::io::{BufReader, Read, Write};
use std::path::Path;
use std::process::exit;
use std::str::FromStr;

use log::*;

use chrono::{DateTime, NaiveDate, NaiveDateTime, TimeZone, Utc, Offset};
use chrono_tz::Tz as ChronoTz;
use clap::Parser;
use deunicode::deunicode;
use reqwest::blocking::Client;

use serde::{Deserialize, Serialize};

// importing for recurring events, otherwise not imported, when we are importing from today onwards
use regex::Regex;
use rrule::{RRule, RRuleSet, Tz}; // trying this shit out

// Helper function to get the local timezone from config.h
fn get_local_timezone() -> ChronoTz {
    let timezone_str = option_env!("TIMEZONE_OLSON").unwrap_or("Europe/Bratislava");
    let local_tz: ChronoTz = timezone_str.parse().unwrap_or(ChronoTz::Europe__Bratislava);
    debug!("Using timezone from config.h: {}", timezone_str);
    local_tz
}

// Get the current timezone offset in seconds
fn get_timezone_offset_seconds() -> i32 {
    let local_tz = get_local_timezone();
    let now = Utc::now();
    let local_now = now.with_timezone(&local_tz);
    let offset_seconds = local_now.offset().fix().local_minus_utc();
    debug!("Timezone offset: {} seconds", offset_seconds);
    offset_seconds
}

// Applies the timezone offset to a UTC timestamp to get a local time timestamp
fn apply_timezone_offset(utc_timestamp: u64) -> u64 {
    let offset_seconds = get_timezone_offset_seconds();
    (utc_timestamp as i64 + offset_seconds as i64) as u64
}

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
        default_value_t = 30
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
    })
    .to_string()
}

fn get_url_string(url: String) -> String {
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
    body
}

fn main() {
    env_logger::init_from_env(
        env_logger::Env::default().filter_or(env_logger::DEFAULT_FILTER_ENV, "debug"),
    );
    debug!("Start");

    // Log timezone info for debugging
    let offset_seconds = get_timezone_offset_seconds();
    let offset_hours = offset_seconds as f64 / 3600.0;
    info!("Local timezone offset: {} seconds ({} hours)", offset_seconds, offset_hours);

    let args = Args::parse();

    if let Some(path) = args.path.clone() {
        let mut string = String::new();
        BufReader::new(File::open(path).unwrap())
            .read_to_string(&mut string)
            .unwrap();
        parse_ical(string.as_bytes(), &args.clone());
    } else if let Some(url) = args.url.clone() {
        let str_from_url = get_url_string(url);
        parse_ical(str_from_url.as_bytes(), &args.clone());
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

    if str.contains('T') {
        let fmt = if str.ends_with('Z') {
            "%Y%m%dT%H%M%SZ"
        } else {
            "%Y%m%dT%H%M%S"
        };
        // Parse as naive datetime first
        let naive = NaiveDateTime::parse_from_str(&str, fmt).expect("Failed to parse datetime");

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
                    // Get timezone from config.h via build.rs environment variable
                    let local_tz = get_local_timezone();
                    
                    local_tz
                        .from_local_datetime(&naive)
                        .single()
                        .unwrap_or_else(|| local_tz.with_ymd_and_hms(1970, 1, 1, 0, 0, 0).unwrap())
                        .with_timezone(&Utc)
                }
            }
        }
    } else {
        let naive_date = NaiveDate::parse_from_str(&str, "%Y%m%d").expect("Failed to parse date");
        Utc.from_utc_datetime(&naive_date.and_hms_opt(0, 0, 0).unwrap())
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
        // debug!("Processing event: {:#?}", event);
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
                            params
                                .iter()
                                .find(|(k, _)| k == "TZID")
                                .and_then(|(_, v)| v.first())
                                .map(|s| s.as_str())
                        });
                        let dt = parse_time(value, tzid);
                        dtstart = Some(dt);
                        
                        // Apply timezone offset to the UTC timestamp
                        evt.start_time = apply_timezone_offset(dt.timestamp() as u64);
                    }
                }
                "DTEND" => {
                    if let Some(value) = prop.value {
                        let tzid = prop.params.as_ref().and_then(|params| {
                            params
                                .iter()
                                .find(|(k, _)| k == "TZID")
                                .and_then(|(_, v)| v.first())
                                .map(|s| s.as_str())
                        });
                        let dt = parse_time(value, tzid);
                        dtend = Some(dt);
                        
                        // Apply timezone offset to the UTC timestamp
                        evt.end_time = apply_timezone_offset(dt.timestamp() as u64);
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
                .after(
                    DateTime::from_timestamp(cutoff as i64, 0)
                        .unwrap()
                        .with_timezone(&Tz::UTC),
                )
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
                
                // Convert to local time for display and storage
                let local_tz = get_local_timezone();
                let local_start = occurrence_start.with_timezone(&local_tz); 
                let day = local_start.format("%d.%m.%Y").to_string();

                if occurrence_start.timestamp() as u64 >= cutoff {
                    let new_evt = Event {
                        name: evt.name.clone(),
                        // Apply timezone offset to the UTC timestamps
                        start_time: apply_timezone_offset(occurrence_start.timestamp() as u64),
                        end_time: apply_timezone_offset(occurrence_end.timestamp() as u64),
                        description: evt.description.clone(),
                        status: evt.status.clone(),
                    };

                    events
                        .entry(day.clone())
                        .or_insert_with(Vec::new)
                        .push(new_evt.clone());
                    days.push(new_evt.start_time);
                }
            }
        } else {
            // Handle non-recurring events
            let day = dtstart
                .map(|dt| {
                    // Convert to local time for display
                    let local_tz = get_local_timezone();
                    dt.with_timezone(&local_tz).format("%d.%m.%Y").to_string()
                })
                .unwrap_or_default();
            if !day.is_empty() && evt.start_time >= cutoff {
                // The timestamps in evt are already corrected with timezone offset
                events.entry(day).or_insert_with(Vec::new).push(evt.clone());
                days.push(evt.start_time);
            }
        }
    }
    // debug!("Final hashmap: {:#?}", events);

    // Sanitize days, remove duplicate days
    debug!("Before sanit days: {:?}", days);
    debug!("Days before sanit len: {}", days.len());
    loop {
        let c = days.len();
        let mut removed = false;
        for i1 in 0..c {
            for i2 in 0..c {
                if i1 == i2 {
                    continue;
                }
                
                // Create DateTimes from timestamps (already including timezone offset)
                let day1_time = DateTime::from_timestamp(days[i1] as i64, 0).unwrap();
                let day2_time = DateTime::from_timestamp(days[i2] as i64, 0).unwrap();
                
                let day1_date = day1_time.format("%d.%m.%Y").to_string();
                let day2_date = day2_time.format("%d.%m.%Y").to_string();
                
                debug!("Day 1: {} and Day 2: {}", day1_date, day2_date);
                if day1_date == day2_date {
                    debug!("Removed those days!");
                    days.remove(i1);
                    removed = true;
                    break;
                }
            }
            if removed {
                break;
            }
        }
        if !removed {
            break;
        }
    }

    debug!("Final days: {:?}", days);

    // debug!("Json: \n{}", json);

    info!("There are {} events", events.len());

    let mut file_proxy: HashMap<String, Vec<u8>> = HashMap::new();
    for event in events {
        // let path = format!("{}{}", args.output_dir, event.0);
        // info!("Writing file: {}", path);
        let small_json = serde_json::to_string_pretty(&event.1).unwrap();
        //let mut file = File::create(path).unwrap();

        let mut ascii_str = small_json;
        ascii_str = deunicode(&ascii_str);
        ascii_str = ascii_str.replace("    ", "\n");
        ascii_str = jsonxf::pretty_print(&ascii_str).unwrap();

        //file.write_all(ascii_str.as_bytes()).unwrap();
        file_proxy.insert(event.0, ascii_str.as_bytes().to_vec());
    }

    days.sort();
    debug!("Days: {:#?}", days);

    let mut index = String::new(); // << this line should stay here, not later

    // Display the formatted day
    let mut c = 0;
    for day in days {
        debug!("Iterating day: {}", day);
        
        // day already includes the timezone offset, so we create a UTC DateTime from it
        let date_time = DateTime::from_timestamp(day as i64, 0).unwrap();
        let time_str = &date_time.format("%d.%m.%Y").to_string();
        
        c += 1;
        if c > args.limit_days {
            if file_proxy.contains_key(time_str) {
                file_proxy.remove(time_str);
                debug!("Removing time from index: {}", time_str);
            }
            continue;
        }
        debug!("Adding day to index: {}", day.to_string());
        index.push_str(&day.to_string());
        index.push('\n');
    }

    // Now write files
    for (key, bytes) in file_proxy {
        let path = format!("{}{}", args.output_dir, key);
        info!("Writing file: {}", path);
        let mut file = File::create(path).unwrap();
        file.write_all(&bytes).unwrap();
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
    file.write_all(index.to_ascii_lowercase().as_bytes())
        .unwrap();

    info!("Done, bye!");
}
