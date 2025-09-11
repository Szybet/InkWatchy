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
    let hour = ui.get_hours() as u8;
    let minute = ui.get_minutes() as u8;
    (hour, minute)
}

pub fn set_time(ui: &TimePicker, hour: u8, minute: u8) {
    ui.set_hours(hour as i32);
    ui.set_minutes(minute as i32);
}

#[cfg(test)]
mod tests {
    

    use super::*;

    #[test]
    fn run_std() {
        let ui = get_time_picker();
        
        ui.run().unwrap();
    }
}
