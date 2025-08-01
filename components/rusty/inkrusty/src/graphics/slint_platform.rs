use core::any::Any;

use alloc::boxed::Box;
use slint::{platform::WindowEvent, run_event_loop, SharedString};

use crate::{
    external::generic::{get_buttons, rustCleanMemory, updateScreen, RustButton},
    info,
};

use super::embedded_graphics::Framebuffer;

pub struct InkPlatform {
    window: alloc::rc::Rc<slint::platform::software_renderer::MinimalSoftwareWindow>,
}

impl slint::platform::Platform for InkPlatform {
    fn create_window_adapter(
        &self,
    ) -> Result<alloc::rc::Rc<dyn slint::platform::WindowAdapter>, slint::PlatformError> {
        Ok(self.window.clone())
    }
    fn duration_since_start(&self) -> core::time::Duration {
        let ret = core::time::Duration::from_micros(unsafe {
            crate::external::generic::rustMicros() as u64
        });
        // let fmt = alloc::format!("ret: {:?}", ret);
        // info!(&fmt);
        ret
    }
    fn run_event_loop(&self) -> Result<(), slint::PlatformError> {
        if let Some(text) = match get_buttons() {
            RustButton::NoButton => None,
            RustButton::Up => Some(SharedString::from("w")),
            RustButton::Down => Some(SharedString::from("s")),
            RustButton::Menu => Some(SharedString::from("a")),
            RustButton::MenuLong => Some(SharedString::from("d")),
            RustButton::UpLong => Some(SharedString::from("q")),
            RustButton::DownLong => Some(SharedString::from("z")),
        } {
            self.window
                .try_dispatch_event(WindowEvent::KeyPressed { text: text.clone() })?;
            self.window
                .try_dispatch_event(WindowEvent::KeyReleased { text })?;
        }

        slint::platform::update_timers_and_animations();

        // It's maybe faster without it?
        // if self.window.has_active_animations() {
        // info!("has_active_animations");
        // continue;
        // return Ok(());
        // }

        // info!("Fucking platform run");

        if !self.window.draw_if_needed(|renderer| {
            // info!("draw_if_needed");
            /*
            if window.has_active_animations() {
                info!("Window has active animations!");
                return;
            }
            */
            use embedded_graphics_core::prelude::*;
            struct DisplayWrapper<'a, T>(
                &'a mut T,
                &'a mut [slint::platform::software_renderer::Rgb565Pixel],
            );

            let mut line = [slint::platform::software_renderer::Rgb565Pixel(0); 200];
            let mut display = Framebuffer::new();

            impl<T: DrawTarget<Color = embedded_graphics_core::pixelcolor::Rgb565>>
                slint::platform::software_renderer::LineBufferProvider for DisplayWrapper<'_, T>
            {
                type TargetPixel = slint::platform::software_renderer::Rgb565Pixel;
                fn process_line(
                    &mut self,
                    line: usize,
                    range: core::ops::Range<usize>,
                    render_fn: impl FnOnce(&mut [Self::TargetPixel]),
                ) {
                    let rect = embedded_graphics_core::primitives::Rectangle::new(
                        Point::new(range.start as _, line as _),
                        Size::new(range.len() as _, 1),
                    );
                    render_fn(&mut self.1[range.clone()]);
                    // NOTE! this is not an efficient way to send pixel to the screen, but it is kept simple on this template.
                    // It would be much faster to use the DMA to send pixel in parallel.
                    // See the example in https://github.com/slint-ui/slint/blob/master/examples/mcu-board-support/pico_st7789.rs
                    self.0
                        .fill_contiguous(
                            &rect,
                            self.1[range.clone()].iter().map(|p| {
                                embedded_graphics_core::pixelcolor::raw::RawU16::new(p.0).into()
                            }),
                        )
                        .map_err(drop)
                        .unwrap();
                }
            }
            renderer.render_by_line(DisplayWrapper(&mut display, &mut line));
            unsafe {
                updateScreen(true, false, true);
            }
            // info!("draw_if_needed end");
        }) {
            unsafe {
                crate::external::generic::delayRust(10);
            }
        }
        // info!("event loop end");

        Ok(())
    }
}

impl InkPlatform {
    pub fn set_platform() {
        let window =
            slint::platform::software_renderer::MinimalSoftwareWindow::new(Default::default());
        window.set_size(slint::PhysicalSize::new(200, 200));

        let platform = alloc::boxed::Box::new(InkPlatform { window });
        let better_platform = Box::leak(platform);
        let even_better_platform = unsafe { Box::from_raw(better_platform) };

        slint::platform::set_platform(even_better_platform).unwrap();
    }
}

static mut SLINT_INITED: bool = false;

pub fn slint_init() {
    unsafe {
        if !SLINT_INITED {
            SLINT_INITED = true;
            InkPlatform::set_platform();
        }
        rustCleanMemory();
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn slint_loop() {
    // info!("Executed slint_loop");
    run_event_loop().unwrap();
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn slint_exit() {
    info!("Executed slint_exit");
    #[allow(static_mut_refs)]
    if let Some(app) = &SLINT_APP_STORE {
        app.on_exit();
    }
    SLINT_APP_STORE = None;
}

pub trait SlintApp: Any {
    fn show(&self);
    fn on_exit(&self);
    fn as_any(&self) -> &dyn Any;
    fn as_any_mut(&mut self) -> &mut dyn Any;
}

pub static mut SLINT_APP_STORE: Option<Box<dyn SlintApp>> = None;

pub fn preserve_app(app: Box<dyn SlintApp>) {
    app.show();
    unsafe {
        //let platform = alloc::boxed::Box::new(ui);
        //let better_platform = Box::leak(platform);
        //let even_better_platform = Box::from_raw(better_platform);
        SLINT_APP_STORE = Some(app);
    }
}
