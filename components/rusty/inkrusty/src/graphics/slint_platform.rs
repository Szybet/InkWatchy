use alloc::boxed::Box;
use slint::Window;

use crate::{external::generic::updateScreen, info};

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
        core::time::Duration::from_micros(unsafe { crate::external::generic::rustMicros() as u64 })
    }
}

impl InkPlatform {
    pub fn set_platform() {
        let window =
            slint::platform::software_renderer::MinimalSoftwareWindow::new(Default::default());
        window.set_size(slint::PhysicalSize::new(200, 200));

        window.draw_if_needed(|renderer| {
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

            let mut line = [slint::platform::software_renderer::Rgb565Pixel(0); 320];
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
        });

        let platform = alloc::boxed::Box::new(InkPlatform { window });

        slint::platform::set_platform(platform).unwrap();
    }
}


use core::sync::atomic::{AtomicPtr, Ordering};

static APP_INSTANCE: AtomicPtr<snake_lib::AppWindow> = AtomicPtr::new(core::ptr::null_mut());

pub fn slint_init(ui: snake_lib::AppWindow) {
    APP_INSTANCE.store(Box::into_raw(Box::new(ui)), Ordering::SeqCst);
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn slint_loop() {
    slint::platform::update_timers_and_animations();
}
