use core::ffi::{c_char, CStr};

use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp, SLINT_APP_STORE},
    info,
};
use alloc::{boxed::Box, vec::Vec};
use general_page::{get_general_page, Adapter, GeneralPage};
use slint::{ComponentHandle, ModelRc, SharedString, StandardListViewItem, VecModel};

pub struct GeneralApp {
    window: GeneralPage,
    button_func: Vec<fn()>,
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
pub unsafe extern "C" fn init_general_page() {
    slint_init();

    let ui = get_general_page();

    let my_struct = Box::new(GeneralApp {
        window: ui,
        button_func: Vec::new(),
    });

    preserve_app(my_struct);
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
    let c_str = CStr::from_ptr(s);
    let string = c_str.to_string_lossy().into_owned();
    general_app
        .window
        .global::<Adapter>()
        .set_title_text(string.into());
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_main(s: *const c_char) {
    let general_app = get_general_app().unwrap();
    let c_str = CStr::from_ptr(s);
    let string = c_str.to_string_lossy().into_owned();
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

        let c_str = CStr::from_ptr(button.string);
        let string = c_str.to_string_lossy().into_owned();
        button_names.push(StandardListViewItem::from(SharedString::from(string)));
    }

    general_app.window.global::<Adapter>()
        .set_how_many_items(button_names.len() as i32);
    let model = ModelRc::new(VecModel::from(button_names));
    general_app.window.set_button_items(model);
}
