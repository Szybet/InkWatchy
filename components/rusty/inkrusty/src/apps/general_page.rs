use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp, SLINT_APP_STORE},
    info,
};
use alloc::{boxed::Box, string::String, vec::Vec};
use core::ffi::{c_char, CStr};
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
        if let Err(_) = self.window.show() {
            info!("Failed to show window");
        }
    }

    fn on_exit(&self) {}

    fn as_any(&self) -> &dyn core::any::Any {
        self
    }

    fn as_any_mut(&mut self) -> &mut dyn core::any::Any {
        self
    }
}

#[no_mangle]
pub unsafe extern "C" fn init_general_page(line_init_capacity: u16) {
    slint_init();
    let ui = get_general_page();

    let ui_weak = ui.as_weak();
    let ui_clone = ui_weak.clone();
    ui.on_button_pressed(move || {
        if let Some(an_an_ui) = ui_clone.upgrade() {
            let an_ui = an_an_ui.global::<Adapter>();
            let i = an_ui.get_current_item();
            button_clicked(i as u16);
        } else {
            info!("Failed to upgrade UI weak reference");
        }
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
            info!("Failed to downcast to GeneralApp");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
    None
}

#[no_mangle]
pub unsafe extern "C" fn general_page_set_title(s: *const c_char) {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in set_title");
        return;
    };
    let string = c_char_to_string(s);
    general_app
        .window
        .global::<Adapter>()
        .set_title_text(string.into());
}

#[no_mangle]
pub unsafe extern "C" fn general_page_disable_title() {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in disable_title");
        return;
    };
    general_app
        .window
        .global::<Adapter>()
        .set_title_enabled(false);
}

#[no_mangle]
pub unsafe extern "C" fn general_page_set_main() {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in set_main");
        return;
    };

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

    info!("General page update yes yes!");
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
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in set_buttons");
        return;
    };
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

#[no_mangle]
pub unsafe extern "C" fn genpage_add(s: *const c_char) -> u16 {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in add");
        return u16::MAX;
    };
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

#[no_mangle]
pub unsafe extern "C" fn genpage_insert(s: *const c_char, add_after_id: u16) -> u16 {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in insert");
        return u16::MAX;
    };
    let new_string = c_char_to_string(s);

    let line = Line {
        id: general_app.str_id,
        str: new_string,
    };
    general_app.str_id += 1;

    let mut vec_index: Option<usize> = None;
    for (c, the_line) in general_app.main_str.iter().enumerate() {
        if the_line.id == add_after_id {
            vec_index = Some(c);
            break;
        }
    }

    let Some(idx) = vec_index else {
        info!("Failed to find line for insert");
        return u16::MAX;
    };

    general_app.main_str.insert(idx + 1, line);
    general_app.main_change = true;
    general_app.str_id - 1
}

#[no_mangle]
pub unsafe extern "C" fn genpage_remove(remove_id: u16) {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in remove");
        return;
    };

    let mut vec_index: Option<usize> = None;
    for (c, the_line) in general_app.main_str.iter().enumerate() {
        if the_line.id == remove_id {
            vec_index = Some(c);
            break;
        }
    }

    let Some(idx) = vec_index else {
        info!("Failed to find line for removal");
        return;
    };

    general_app.main_str.remove(idx);
    general_app.main_change = true;
}

#[no_mangle]
pub unsafe extern "C" fn genpage_change(s: *const c_char, line: u16) {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in change");
        return;
    };
    let new_string = c_char_to_string(s);

    for the_line in general_app.main_str.iter_mut() {
        if the_line.id == line {
            the_line.str = new_string;
            general_app.main_change = true;
            return;
        }
    }
    info!("Failed to find line to change");
}

#[no_mangle]
pub unsafe extern "C" fn genpage_is_menu() -> bool {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in is_menu");
        return false;
    };
    general_app.window.global::<Adapter>().get_show_menu()
}

pub unsafe fn button_clicked(i: u16) {
    let Some(general_app) = get_general_app() else {
        info!("GeneralApp not found in button_clicked");
        return;
    };

    if i == 0 || i as usize > general_app.button_func.len() {
        info!("Invalid button index");
        return;
    }

    (general_app.button_func[(i - 1) as usize])();
}

#[no_mangle]
pub unsafe extern "C" fn genpage_set_center() {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in set_center");
        return;
    };
    general_app
        .window
        .global::<Adapter>()
        .set_main_align_center(true);
    general_app.main_change = true;
}

#[no_mangle]
pub unsafe extern "C" fn genpage_set_center_vertical() {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in set_center_vertical");
        return;
    };
    general_app
        .window
        .global::<Adapter>()
        .set_main_align_center_vertical(true);
    general_app.main_change = true;
}

#[no_mangle]
pub unsafe extern "C" fn genpage_force_update() {
    let Some(general_app) = get_general_app() else {
        info!("Failed to get GeneralApp in genpage_force_update");
        return;
    };
    general_app.window.window().request_redraw();
}
