use embedded_graphics_core::{pixelcolor::Rgb565, prelude::{Dimensions, DrawTarget, PixelColor, RgbColor}};
use crate::{external::generic::{drawPixel, updateScreen}};

#[derive(Copy, Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug)]
pub enum Color {
    Black,
    White,
}

impl PixelColor for Color {
    type Raw = ();
}

pub struct Framebuffer;

impl Framebuffer {
    pub fn new() -> Self {
        Self {}
    }

    pub fn flush(really_update: bool, ignore_counter: bool, ignore_sleep: bool) {
        unsafe {
            updateScreen(really_update, ignore_counter, ignore_sleep);
        }
    }
}

impl Dimensions for Framebuffer {
    fn bounding_box(&self) -> embedded_graphics_core::primitives::Rectangle {
        embedded_graphics_core::primitives::Rectangle {
            top_left: embedded_graphics_core::geometry::Point { x: 0, y: 0 },
            size: embedded_graphics_core::geometry::Size {
                width: 200 as u32,
                height: 200 as u32,
            },
        }
    }
}

impl DrawTarget for Framebuffer {
    type Color = Rgb565;
    type Error = ();

    fn draw_iter<I>(&mut self, pixels: I) -> Result<(), ()>
    where
        I: IntoIterator<Item = embedded_graphics_core::Pixel<Rgb565>>,
    {
        for embedded_graphics_core::Pixel(point, color) in pixels {
            let x = point.x as i16;
            let y = point.y as i16;

            if x >= 200 || y >= 200 {
                continue;
            }

            let color_bit1: u16 = match color {
                Rgb565::WHITE => 0xFFFF,
                Rgb565::BLACK => 0x0000,
                other => {
                    // info!(&format!("Processing color {:?}", other));
                    let r = other.r() as u16;
                    let g = other.g() as u16;
                    let b = other.b() as u16;
                    
                    const THRESHOLD: u16 = 5891;
                    
                    let value = 62 * (r + b) + 126 * g;
                    
                    if value >= THRESHOLD { 0xFFFF } else { 0x0000 }
                }
            };

            unsafe {
                drawPixel(x, y, color_bit1);
            }
        }
        Ok(())
    }
}
