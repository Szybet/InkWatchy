use core::ffi::{c_char, CStr};

use alloc::boxed::Box;
use general_page::{get_general_page, Adapter, GeneralPage};
use slint::ComponentHandle;
use crate::{graphics::slint_platform::{preserve_app, slint_init, SlintApp, SLINT_APP_STORE}, info};

pub struct GeneralApp {
    window: GeneralPage,
}

impl SlintApp for GeneralApp {
    fn show(&self) {
        self.window.show().unwrap();
    }
    fn on_exit(&self) {
    }
    
    fn as_any(&self) -> &dyn core::any::Any {
        self
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn init_general_page() {   
    slint_init();
     
    let ui = get_general_page();

    let my_struct = Box::new(GeneralApp {
        window: ui,
    });

    preserve_app(my_struct);
}

pub unsafe fn get_general_app() -> Option<&'static GeneralApp> {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &SLINT_APP_STORE {
        if let Some(general_app) = slint_app.as_any().downcast_ref::<GeneralApp>() {
            return Some(general_app);
        } else {
            info!("Error as any general app");
        }
    } else {
        info!("Error mut refs");
    }
    None
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_title(s: *const c_char) {
    let general_app = get_general_app().unwrap();
    let c_str = CStr::from_ptr(s);
    let string = c_str.to_string_lossy().into_owned();
    general_app.window.global::<Adapter>().set_title_text(string.into());
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn general_page_set_main(s: *const c_char) {
    let general_app = get_general_app().unwrap();
    let c_str = CStr::from_ptr(s);
    let string = c_str.to_string_lossy().into_owned();
    general_app.window.global::<Adapter>().set_main_text(string.into());
}
