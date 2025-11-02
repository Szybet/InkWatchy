use crate::drawing::draw_pixel;
use crate::external::generic::getColorBlack;
use crate::external::generic::getColorWhite;
use crate::external::generic::updateScreen;
use embedded_graphics_core::{
    pixelcolor::Rgb565,
    prelude::{Dimensions, DrawTarget, PixelColor, RgbColor},
};

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
        let good_black = unsafe { getColorBlack() };
        let good_white = unsafe { getColorWhite() };
        for embedded_graphics_core::Pixel(point, color) in pixels {
            if point.x >= 200 || point.y >= 200 {
                continue;
            }

            let color_bit1: bool = match color {
                Rgb565::WHITE => *good_white != 0,
                Rgb565::BLACK => *good_black != 0,
                other => {
                    // info!(&alloc::format!("Processing color {:?}", other));
                    let r = other.r() as u16;
                    let g = other.g() as u16;
                    let b = other.b() as u16;

                    const THRESHOLD: u16 = 5891;

                    let value = 62 * (r + b) + 126 * g;

                    if value >= THRESHOLD {
                        *good_white != 0
                    } else {
                        *good_black != 0
                    }
                }
            };

            draw_pixel(point.x as u16, point.y as u16, color_bit1);
        }
        Ok(())
    }
}
