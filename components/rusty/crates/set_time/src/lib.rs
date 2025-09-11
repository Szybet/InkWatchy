#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

pub fn get_time_picker() -> TimePicker {
    let ui = TimePicker::new().unwrap();
    ui
}

#[repr(C)]
pub struct SetTimeTime {
    pub hour: u8,
    pub minute: u8,
}

// Return hour, minute
pub fn get_time(ui: &TimePicker) -> SetTimeTime {
    let hour = ui.get_hours() as u8;
    let minute = ui.get_minutes() as u8;
    SetTimeTime {hour, minute}
}

pub fn set_time(ui: &TimePicker) {

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
