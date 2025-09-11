#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

pub fn get_time_picker() -> TimePicker {
    let ui = TimePicker::new().unwrap();
    ui
}

// Return hour, minute
pub fn get_time(ui: &TimePicker) -> (u8, u8) {
    let hour_tens = ui.get_hour_tens();
    let hour_units = ui.get_hour_units();
    let minute_tens = ui.get_minute_tens();
    let minute_units = ui.get_minute_units();

    let hour = (hour_tens * 10 + hour_units) as u8;
    let minute = (minute_tens * 10 + minute_units) as u8;

    (hour, minute)
}

pub fn set_time(ui: &TimePicker, hour: u8, minute: u8) {
    ui.set_hour_tens((hour / 10) as i32);
    ui.set_hour_units((hour % 10) as i32);
    ui.set_minute_tens((minute / 10) as i32);
    ui.set_minute_units((minute % 10) as i32);
}

#[cfg(test)]
mod tests {
    

    use super::*;

    #[test]
    fn run_std() {
        let ui = get_time_picker();
        set_time(&ui, 12, 34);
        ui.run().unwrap();
    }
}
