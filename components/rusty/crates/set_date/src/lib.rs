#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

pub fn get_date_picker() -> DatePicker {
    let ui = DatePicker::new().unwrap();
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
