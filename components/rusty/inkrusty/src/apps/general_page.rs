use core::ffi::{c_char, CStr};

use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp, SLINT_APP_STORE},
    info,
};
use alloc::{boxed::Box, string::String, vec::Vec};
use general_page::{get_general_page, Adapter, GeneralPage};
use slint::{ComponentHandle, ModelRc, SharedString, StandardListViewItem, VecModel};

struct Line {
    id: u16,
    str: String,
}

pub struct GeneralApp {
    window: GeneralPage,
    button_func: Vec<fn()>,
    main_str: Vec<Line>,
    str_id: u16,
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
pub unsafe extern "C" fn init_general_page(line_init_capacity: u16) {
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
        main_str: Vec::with_capacity(line_init_capacity as usize),
        str_id: 0,
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

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_disable_title() {
    let general_app = get_general_app().unwrap();

    general_app
        .window
        .global::<Adapter>()
        .set_title_enabled(false);
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

    let mut string = String::new();

    let len = general_app.main_str.len();
    for (c, the_line) in general_app.main_str.iter().enumerate() {
        string.push_str(&the_line.str);
        if c != len - 1 {
            string.push('\n');
        }
    }

    general_app
        .window
        .global::<Adapter>()
        .set_main_text(string.into());
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
    let new_string = c_char_to_string(s);

    let line = Line {
        id: general_app.str_id,
        str: new_string,
    };
    general_app.str_id += 1;

    general_app.main_str.push(line);

    general_app.main_change = true;

    general_app.str_id - 1
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_insert(s: *const c_char, add_after_id: u16) -> u16 {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    let new_string = c_char_to_string(s);

    let line = Line {
        id: general_app.str_id,
        str: new_string,
    };
    general_app.str_id += 1;

    let mut vec_index: usize = usize::MAX;
    for (c, the_line) in general_app.main_str.iter().enumerate() {
        if the_line.id == add_after_id {
            vec_index = c;
            break;
        }
    }
    #[cfg(feature = "debug")]
    if vec_index == usize::MAX {
        info!("Failed to find line! Critical! Ignoring adding the line");
        return u16::MAX;
    }

    general_app.main_str.insert(vec_index + 1, line);

    general_app.main_change = true;

    general_app.str_id - 1
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_remove(remove_id: u16) {
    let general_app: &mut GeneralApp = get_general_app().unwrap();

    let mut vec_index: usize = usize::MAX;
    for (c, the_line) in general_app.main_str.iter().enumerate() {
        if the_line.id == remove_id {
            vec_index = c;
            break;
        }
    }
    #[cfg(feature = "debug")]
    if vec_index == usize::MAX {
        info!("Failed to find line! Critical! Ignoring removing the line");
        return;
    }

    general_app.main_str.remove(vec_index);

    general_app.main_change = true;
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
    let new_string = c_char_to_string(s);

    for the_line in general_app.main_str.iter_mut() {
        if the_line.id == line {
            the_line.str = new_string;
            break;
        }
    }

    general_app.main_change = true;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_is_menu() -> bool {
    let general_app: &mut GeneralApp = get_general_app().unwrap();

    general_app.window.global::<Adapter>().get_show_menu()
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
        .global::<Adapter>()
        .set_main_align_center(true);

    general_app.main_change = true;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn genpage_set_center_vertical() {
    let general_app: &mut GeneralApp = get_general_app().unwrap();
    general_app
        .window
        .global::<Adapter>()
        .set_main_align_center_vertical(true);

    general_app.main_change = true;
}
