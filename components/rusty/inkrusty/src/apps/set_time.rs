use crate::external::generic::*;
use crate::graphics::slint_platform::SLINT_APP_STORE;
use crate::{
    graphics::slint_platform::{preserve_app, slint_init, SlintApp},
    info,
};
use alloc::boxed::Box;
use set_time::{get_time, get_time_picker, TimePicker};
use slint::ComponentHandle;
pub use set_time::SetTimeTime;

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

    set_cpu_speed(&CpuSpeed::Max); // It's a tech demo, not a optimised game, otherwise it would be to boring to create

    preserve_app(my_struct);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn set_time_get_time() -> SetTimeTime {
    #[allow(static_mut_refs)]
    if let Some(slint_app) = &mut SLINT_APP_STORE {
        if let Some(set_time_app) = slint_app.as_any_mut().downcast_mut::<SetTimeApp>() {
            return get_time(&set_time_app.window);
        } else {
            info!("Error downcasting to general app");
        }
    } else {
        info!("SLINT_APP_STORE is None");
    }
    SetTimeTime {
        hour: 255,
        minute: 255,
    }
}
