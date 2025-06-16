// #![no_std]

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
        if str == "w" {
            return Button::Up;
        }
        if str == "s" {
            return Button::Down;
        }
        if str == "a" {
            return Button::Menu;
        }
        Button::None
    }
}

pub fn get_general_page() -> GeneralPage {
    let ui = GeneralPage::new().unwrap();

    let ui_weak = ui.as_weak();
    let ui_clone = ui_weak.clone();
    ui.on_key_pressed(move |key: SharedString| {
        let btn = Button::new(key);

        let an_an_ui = ui_clone.upgrade().unwrap();
        let an_ui = an_an_ui.global::<Adapter>();
        if !an_ui.get_buttons_shown() {
            //let vh = an_ui.get_scroll_viewport_height();

            //println!("vh: {:?}", vh);

            if btn == Button::Up {
                an_an_ui.invoke_handleUpButton();
            }
            if btn == Button::Down {
                an_an_ui.invoke_handleDownButton();
            }
            if btn == Button::Menu {
                an_an_ui.invoke_handleMenuButton();
            }
            /*
            if scroll < 0.0 {
                scroll = 0.0;
            }
            if scroll > vh {
                scroll = vh;
            }
             */
        }
    });

    let ui_weak = ui.as_weak();
    let ui_clone = ui_weak.clone();
    ui.on_button_pressed(move || {
        let an_an_ui = ui_clone.upgrade().unwrap();
        let an_ui = an_an_ui.global::<Adapter>();
        let i = an_ui.get_current_item();

        println!("button pressed on index: {}", i);
    });

    ui
}

#[cfg(test)]
mod tests {
    use slint::StandardListViewItem;

    use super::*;

    #[test]
    fn run_std() {
        let ui = get_general_page();
        ui.global::<Adapter>()
            .set_title_text("Test title is here".into());
        // ui.global::<Adapter>().set_title_enabled(false);
        ui.global::<Adapter>().set_main_text("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.".into());
        let items = vec![
            StandardListViewItem::from("Lorem"),
            StandardListViewItem::from("Ipsum"),
            StandardListViewItem::from("Dolor"),
            StandardListViewItem::from("Sit"),
            StandardListViewItem::from("Amet"),
            StandardListViewItem::from("Consectetur"),
        ];

        ui.global::<Adapter>().set_how_many_items(items.len() as i32);
        let model = ModelRc::new(VecModel::from(items));
        ui.set_button_items(model);

        ui.run().unwrap();
    }
}
