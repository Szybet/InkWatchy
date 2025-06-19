use core::ffi::{c_char, CStr};

use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp, SLINT_APP_STORE},
    info,
};
use alloc::{boxed::Box, string::String, vec::Vec};
use general_page::{get_general_page, Adapter, GeneralPage};
use slint::{ComponentHandle, ModelRc, SharedString, StandardListViewItem, VecModel};

pub struct GeneralApp {
    window: GeneralPage,
    button_func: Vec<fn()>,
    main_str: String,
    main_change: bool,
}

impl SlintApp for GeneralApp {
    fn show(&self) {
        self.window.show().unwrap();
    }
    fn on_exit(&self) {}

    fn as_any(&self) -> &dyn core::any::Any {
        self
    }

    fn as_any_mut(&mut self) -> &mut dyn core::any::Any {
        self
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn init_general_page(str_capacity: u16) {
    slint_init();

    let ui = get_general_page();

    let ui_weak = ui.as_weak();
    let ui_clone = ui_weak.clone();
    ui.on_button_pressed(move || {
        let an_an_ui = ui_clone.upgrade().unwrap();
        let an_ui = an_an_ui.global::<Adapter>();
        let i = an_ui.get_current_item();
        button_clicked(i as u16);
        // println!("button pressed on index: {}", i);
    });

    let my_struct = Box::new(GeneralApp {
        window: ui,
        button_func: Vec::new(),
        main_str: String::with_capacity(str_capacity as usize),
        main_change: false,
    });

    preserve_app(my_struct);
}

pub fn c_char_to_string(s: *const c_char) -> String {
    let c_str = unsafe { CStr::from_ptr(s) };
    c_str.to_string_lossy().into_owned()
}

pub unsafe fn get_general_app() -> Option<&'static mut GeneralApp> {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(general_app) = slint_app.as_any_mut().downcast_mut::<GeneralApp>() {
            return Some(general_app);
        } else {
            info!("Error downcasting to general app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
    None
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_title(s: *const c_char) {
    let general_app = get_general_app().unwrap();
    let string = c_char_to_string(s);

    general_app
        .window
        .global::<Adapter>()
        .set_title_text(string.into());
}

/*
#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_main(s: *const c_char) {
    let general_app = get_general_app().unwrap();
    let string = c_char_to_string(s);

    general_app
        .window
        .global::<Adapter>()
        .set_main_text(string.into());
}
*/

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_main() {
    let general_app = get_general_app().unwrap();

    if general_app.main_change {
        general_app.main_change = !general_app.main_change;
    } else {
        return;
    }

    if general_app.main_str.ends_with('\n') {
        general_app.main_str.pop();
    }

    general_app
        .window
        .global::<Adapter>()
        .set_main_text(general_app.main_str.clone().into());
}

#[repr(C)]
pub struct GeneralPageButton {
    pub string: *const c_char,
    pub function: fn(),
}

#[no_mangle]
pub unsafe extern "C" fn general_page_set_buttons(buttons: *const GeneralPageButton, length: i16) {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    let mut button_names = Vec::new();
    button_names.push(StandardListViewItem::from(SharedString::from("exit")));

    for i in 0..length {
        let button = &*buttons.offset(i as isize);
        general_app.button_func.push(button.function);

        let string = c_char_to_string(button.string);
        button_names.push(StandardListViewItem::from(SharedString::from(string)));
    }

    general_app
        .window
        .global::<Adapter>()
        .set_how_many_items(button_names.len() as i32);
    let model = ModelRc::new(VecModel::from(button_names));
    general_app.window.set_button_items(model);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_add(s: *const c_char) -> u16 {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    let string = c_char_to_string(s);

    general_app.main_str.push_str(&string);
    let lines = general_app.main_str.lines().count() as u16 - 1;
    general_app.main_str.push('\n');

    general_app.main_change = true;

    lines
}

/*
#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_change(s: *const c_char, line: u16) {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    let string = c_char_to_string(s);

    let mut lines: Vec<&str> = general_app.main_str.lines().collect();
    lines[line as usize] = &string;
    general_app.main_str = lines.join("\n");

    general_app.main_change = true;
}
*/

// More efficient?
#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_change(s: *const c_char, line: u16) {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    let new_line = c_char_to_string(s);

    let mut start = 0;
    for _ in 0..line {
        start += general_app.main_str[start..].find('\n').map_or(0, |i| i + 1);
    }
    let end = start + general_app.main_str[start..].find('\n').unwrap_or_else(|| general_app.main_str.len() - start);
    general_app.main_str.replace_range(start..end, &new_line);

    general_app.main_change = true;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_is_menu() -> bool {
    let general_app: &mut GeneralApp = get_general_app().unwrap();

    general_app
    .window
    .global::<Adapter>().get_show_menu()
}

pub fn button_clicked(i: u16) {
    let general_app: &mut GeneralApp = unsafe { get_general_app().unwrap() };
    general_app.button_func[(i - 1) as usize]();
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_set_center() {
    let general_app: &mut GeneralApp = get_general_app().unwrap();

    general_app
    .window
    .global::<Adapter>().set_main_align_center(true);
}