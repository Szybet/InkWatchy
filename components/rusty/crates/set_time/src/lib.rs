#![cfg_attr(not(test), no_std)]

#[allow(unused_imports)]
use slint::{ModelRc, SharedString, VecModel};

slint::include_modules!();

#[derive(PartialEq)]
enum Button {
    None,
    Up,
    Down,
    Menu,
}

impl Button {
    pub fn new(str: SharedString) -> Self {
        if str == "w" || str == "q" {
            return Button::Up;
        }
        if str == "s" || str == "z" {
            return Button::Down;
        }
        if str == "a" || str == "d" {
            return Button::Menu;
        }
        Button::None
    }
}

pub fn get_time_picker() -> TimePicker {
    let ui = TimePicker::new().unwrap();
    ui
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
