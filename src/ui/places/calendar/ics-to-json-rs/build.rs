use std::fs;
use std::path::Path;
use regex::Regex;

fn main() {
    println!("cargo::rustc-check-cfg=cfg(allow_insecure_cal)");
    println!("cargo::rustc-check-cfg=cfg(future_only)");

    let config_path = Path::new("../../../../defines/config.h");
    let config_content = fs::read_to_string(config_path)
        .expect("Failed to read config.h");

    // Extract ALLOW_INSECURE_CAL from config.h
    let re_insecure = Regex::new(r#"#define\s+ALLOW_INSECURE_CAL\s+(\d+)"#).unwrap();
    if let Some(captures) = re_insecure.captures(&config_content) {
        if let Some(value_match) = captures.get(1) {
            let value = value_match.as_str();
            if value == "1" {
                println!("cargo:rustc-cfg=allow_insecure_cal");
                println!("cargo:warning=Calendar insecure connections allowed");
            }
        }
    }

    // Extract FUTURE_ONLY from config.h
    let re_future = Regex::new(r#"#define\s+FUTURE_ONLY\s+(\d+)"#).unwrap();
    if let Some(captures) = re_future.captures(&config_content) {
        if let Some(value_match) = captures.get(1) {
            let value = value_match.as_str();
            if value == "1" {
                println!("cargo:rustc-cfg=future_only");
                println!("cargo:warning=Calendar future only events enabled");
            }
        }
    }
    
    // Extract TIMEZONE_OLSON from config.h
    let re_timezone = Regex::new(r#"#define\s+TIMEZONE_OLSON\s+"([^"]+)""#).unwrap();
    if let Some(captures) = re_timezone.captures(&config_content) {
        if let Some(timezone) = captures.get(1) {
            let timezone_str = timezone.as_str();
            println!("cargo:rustc-env=TIMEZONE_OLSON={}", timezone_str);
            println!("cargo:warning=Using timezone: {}", timezone_str);
        }
    }

    // Extract CALENDAR_NEXT_DAYS from config.h
    let re_days = Regex::new(r#"#define\s+CALENDAR_NEXT_DAYS\s+(\d+)"#).unwrap();
    if let Some(captures) = re_days.captures(&config_content) {
        if let Some(days_match) = captures.get(1) {
            let days_str = days_match.as_str();
            println!("cargo:rustc-env=CALENDAR_NEXT_DAYS={}", days_str);
            println!("cargo:warning=Calendar next days: {}", days_str);
        }
    }
}

