extern crate ical;

use std::collections::HashMap;
use std::fs::File;
use std::io::{BufReader, Read, Write};
use std::process::exit;
use std::str::FromStr;

use log::*;

use chrono::{DateTime, NaiveDate, NaiveDateTime, TimeZone, Utc};
use clap::Parser;
use deunicode::deunicode;

use serde::{Deserialize, Serialize};
use serde_json::Result;

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
        let body = reqwest::blocking::get(url).unwrap().text().unwrap();
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

pub fn parseTime(str: String) -> NaiveDateTime {
    debug!("Parsing time: {}", str);
    if str.len() == 15 {
        debug!("Time 15");
        NaiveDateTime::parse_from_str(&str, "%Y%m%dT%H%M%S")
                    .expect("Failed to parse date")
    } else if str.len() == 16 {
        debug!("Time 20");
        NaiveDateTime::parse_from_str(&str, "%Y%m%dT%H%M%SZ")
                    .expect("Failed to parse date")
    }  else if str.len() == 8 {
        debug!("Time 8");
        let date = NaiveDate::parse_from_str(&str, "%Y%m%d")
                    .expect("Failed to parse date");
        NaiveDateTime::new(date, chrono::NaiveTime::from_hms_opt(0, 0, 0).unwrap()) // Midnight
    } else {
        error!("Unknown time format, ehh.");
        exit(1);
    }
    
}

pub fn parse_ical(buf: &[u8], args: &Args) {
    let mut reader = ical::IcalParser::new(buf);

    // Why can't i clone it, why does count consume it...
    if ical::IcalParser::new(buf).count() != 1 {
        error!("Ical count is not 1, something is weird");
    }

    let mut events: HashMap<String, Vec<Event>> = HashMap::new();
    let mut days: Vec<u64> = Vec::new();

    for i in reader.nth(0).unwrap().unwrap().events {
        debug!("I: {:#?}", i);
        let mut event = Event::default();
        let mut day = String::new();
        for p in i.properties {
            if p.name == "SUMMARY" {
                event.name = p.value.unwrap();
            } else if p.name == "DTSTART" {
                let time = parseTime(p.value.unwrap());

                day = time.format("%d.%m.%Y").to_string();
                let unix = Utc.from_utc_datetime(&time).timestamp() as u64;
                debug!("Unix time: {}", unix);
                event.start_time = unix;
            } else if p.name == "DTEND" {
                let time = parseTime(p.value.unwrap());

                let unix = Utc.from_utc_datetime(&time).timestamp() as u64;
                debug!("Unix time: {}", unix);
                event.end_time = unix;
            } else if p.name == "STATUS" {
                event.status = p.value.unwrap();
            } else if p.name == "DESCRIPTION" {
                event.description = p.value.unwrap();
            }
        }
        if let Some(map) = events.get_mut(&day) {
            map.push(event);
        } else {
            events.insert(day, vec![event.clone()]);
            days.push(event.start_time);
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
        //ascii_str = ascii_str.replace("\\n", "\n");
        ascii_str = ascii_str.replace("    ", "\n");        
        ascii_str = jsonxf::pretty_print(&ascii_str).unwrap();

        file.write_all(ascii_str.as_bytes()).unwrap();
    }

    days.sort();
    debug!("Days: {:#?}", days);

    let mut index = String::new();

    let mut c = 0;
    for day in days {
        let time = DateTime::from_timestamp(day as i64, 0).unwrap();
        let time_str = &time.format("%d.%m.%Y").to_string();
        c = c + 1;
        if c > args.limit_days {
            let rm_path = format!("{}{}", args.output_dir, time_str);
            std::fs::remove_file(rm_path).unwrap();
            continue;
        }
        index.push_str(&day.to_string());
        index.push('\n');
    }

    let path = format!("{}{}", args.output_dir, "index.txt");
    info!("Writing file: {}", path);
    let mut file = File::create(path).unwrap();
    file.write_all(index.to_ascii_lowercase().as_bytes())
        .unwrap();

    info!("Done, bye!");
}
