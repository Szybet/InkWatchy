use std::fs;
use std::path::Path;
use std::env;
use regex::Regex;

fn main() {
    println!("cargo::rustc-check-cfg=cfg(allow_insecure_cal)");
    println!("cargo::rustc-check-cfg=cfg(future_only)");

    let config_path = Path::new("../../../../defines/config.h");
    let config_content = fs::read_to_string(config_path)
        .expect("Failed to read config.h");

    if config_content.contains("#define ALLOW_INSECURE_CAL 1") {
        println!("cargo:rustc-cfg=allow_insecure_cal");
    }

    if config_content.contains("#define FUTURE_ONLY 1") {
        println!("cargo:rustc-cfg=future_only");
    }
    
    // Extract TIMEZONE_OLSON from config.h
    let re = Regex::new(r#"#define\s+TIMEZONE_OLSON\s+"([^"]+)""#).unwrap();
    if let Some(captures) = re.captures(&config_content) {
        if let Some(timezone) = captures.get(1) {
            let timezone_str = timezone.as_str();
            println!("cargo:rustc-env=TIMEZONE_OLSON={}", timezone_str);
            println!("cargo:warning=Using timezone: {}", timezone_str);
        }
    }
}

