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

}

#[cfg(test)]
mod tests {
    use slint::StandardListViewItem;

    use super::*;

    #[test]
    fn run_std() {
        let ui = get_time_picker();
        
        ui.run().unwrap();
    }
}
