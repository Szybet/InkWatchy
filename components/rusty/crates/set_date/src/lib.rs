#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

pub fn get_date_picker() -> DatePicker {
    let ui = DatePicker::new().unwrap();
    ui
}

pub fn date_picker_set_date(ui: &DatePicker, year: u16, month: u8, day: u8) {
    let new_date = Date {
        year: year as i32,
        month: month as i32,
        day: day as i32,
    };
    ui.set_date(new_date);
}

// year, month, day
pub fn date_picket_get_date(ui: &DatePicker) -> (u16, u8, u8) {
    let current_date = ui.get_date();
    (
        current_date.year as u16,
        current_date.month as u8,
        current_date.day as u8,
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn run_std() {
        let ui = get_date_picker();
        date_picker_set_date(&ui, 2010, 10, 10);
        println!("Date set: {:?}", date_picket_get_date(&ui));
        ui.run().unwrap();
    }
}
