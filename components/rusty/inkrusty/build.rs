use std::env;
use std::fs;
use std::path::Path;

fn main() {
    println!("cargo:rerun-if-changed=../../../README.md");
    println!("cargo:rerun-if-changed=build.rs");

    let readme_path = Path::new("../../../README.md");
    let content = fs::read_to_string(readme_path).expect("Failed to read README.md");

    let mut credits = Vec::new();
    let in_credits_section = content.lines()
        .skip_while(|line| !line.starts_with("## Some credits"))
        .skip(1)
        .take_while(|line| line.starts_with("- "));

    for line in in_credits_section {
        let mut parts = line.splitn(3, '-');
        parts.next(); // consume before first '-'
        if let Some(name_part) = parts.next() {
            let name_trimmed = name_part.trim();
            
            let link_parts: Vec<&str> = name_trimmed.split("](").collect();
            let name = if link_parts.len() > 1 {
                // It's a link
                link_parts[0].trim_start_matches('[')
            } else {
                // Not a link
                name_trimmed
            };
            credits.push(name.to_string());
        }
    }

    let dest_path = Path::new(&env::var("OUT_DIR").unwrap()).join("credits.rs");
    fs::write(
        &dest_path,
        format!(
            "pub const CREDITS_NICKS: &str = r\"{}\";",
            credits.join("\n")
        ),
    )
    .unwrap();
}
