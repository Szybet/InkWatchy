use crate::external::generic::*;
use crate::graphics::slint_platform::SLINT_APP_STORE;
use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp},
    info,
};
use alloc::boxed::Box;
use set_time::set_time;
use set_time::{get_time, get_time_picker, TimePicker};
use slint::ComponentHandle;

pub struct SetTimeApp {
    window: TimePicker,
    cpu_speed: CpuSpeed,
}

impl SlintApp for SetTimeApp {
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
pub unsafe extern "C" fn init_set_time() {
    slint_init();

    let ui = get_time_picker();

    let my_struct = Box::new(SetTimeApp {
        window: ui,
        cpu_speed: get_cpu_speed(),
    });

    set_cpu_speed(&CpuSpeed::Max);

    preserve_app(my_struct);
}

#[repr(C)]
pub struct SetTimeTime {
    pub hour: u8,
    pub minute: u8,
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn set_time_get_time() -> SetTimeTime {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(set_time_app) = slint_app.as_any_mut().downcast_mut::<SetTimeApp>() {
            let t = get_time(&set_time_app.window);
            return SetTimeTime {
                hour: t.0,
                minute: t.1,
            };
        } else {
            info!("Error downcasting to time app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
    SetTimeTime {
        hour: 255,
        minute: 255,
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn set_time_set_time(hour: u8, minute: u8) {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(set_time_app) = slint_app.as_any_mut().downcast_mut::<SetTimeApp>() {
            return set_time(&set_time_app.window, hour, minute);
        } else {
            info!("Error downcasting to general app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
}
