#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

pub fn get_date_picker() -> DatePicker {
    let ui = DatePicker::new().unwrap();

    // Create a sample date for demonstration
    let current_date = Date {
        year: 2025,
        month: 9, // September
        day: 12,
    };

    // Set the initial date for the DatePicker
    ui.set_date(current_date.clone()); // Clone for the first use
    // Set today's date for highlighting
    ui.set_today_date(current_date);

    ui
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn run_std() {
        let ui = get_date_picker();
        ui.run().unwrap();
    }
}
