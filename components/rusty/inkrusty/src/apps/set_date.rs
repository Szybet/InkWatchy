use crate::external::generic::*;
use crate::graphics::slint_platform::SLINT_APP_STORE;
use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp},
    info,
};
use alloc::boxed::Box;
use set_date::get_date_picker;
use set_date::{date_picker_set_date, date_picket_get_date, DatePicker};
use slint::ComponentHandle;

pub struct SetDateApp {
    window: DatePicker,
    cpu_speed: CpuSpeed,
}

impl SlintApp for SetDateApp {
    fn show(&self) {
        self.window.show().unwrap();
    }
    fn on_exit(&self) {
        // Restore
        set_cpu_speed(&self.cpu_speed);
    }
    fn as_any(&self) -> &dyn core::any::Any {
        self
    }
    fn as_any_mut(&mut self) -> &mut dyn core::any::Any {
        self
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn init_set_date() {
    slint_init();

    let ui = get_date_picker();

    let my_struct = Box::new(SetDateApp {
        window: ui,
        cpu_speed: get_cpu_speed(),
    });

    set_cpu_speed(&CpuSpeed::Max);

    preserve_app(my_struct);
}

#[repr(C)]
pub struct RustDate {
    pub year: u16,
    pub month: u8,
    pub day: u8,
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn set_date_get_date() -> RustDate {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(set_date_app) = slint_app.as_any_mut().downcast_mut::<SetDateApp>() {
            let t = date_picket_get_date(&set_date_app.window);
            return RustDate {
                year: t.0,
                month: t.1,
                day: t.2,
            };
        } else {
            info!("Error downcasting to date app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
    RustDate {
        year: 2025,
        month: 255,
        day: 255,
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn set_date_set_date(year: u16, month: u8, day: u8) {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(set_date_app) = slint_app.as_any_mut().downcast_mut::<SetDateApp>() {
            return date_picker_set_date(&set_date_app.window, year, month, day);
        } else {
            info!("Error downcasting to general app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
}
