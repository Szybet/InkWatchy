use crate::external::generic::{getMirror, getRotation, getScreenBuffer};

const WIDTH: u16 = 200;
const HEIGHT: u16 = 200;

// Replicating this function here instead of doing it in C saves like 100-150ms
pub fn draw_pixel(mut x: u16, mut y: u16, color: bool) {
    let mirror = unsafe { getMirror() };
    if mirror {
        x = WIDTH - x - 1;
    }

    let rotation = unsafe { getRotation() };
    match rotation {
        1 => {
            core::mem::swap(&mut x, &mut y);
            x = WIDTH - x - 1;
        }
        2 => {
            x = WIDTH - x - 1;
            y = HEIGHT - y - 1;
        }
        3 => {
            core::mem::swap(&mut x, &mut y);
            y = HEIGHT - y - 1;
        }
        _ => {}
    }

    if x >= WIDTH || y >= HEIGHT {
        return;
    }

    let buffer = unsafe { getScreenBuffer() };
    let i = (x as usize / 8) + (y as usize * (WIDTH as usize / 8));

    let buffer_slice =
        unsafe { core::slice::from_raw_parts_mut(buffer, (WIDTH * HEIGHT / 8) as usize) };

    if color != false {
        buffer_slice[i] |= 1 << (7 - (x % 8));
    } else {
        buffer_slice[i] &= !(1 << (7 - (x % 8)));
    }
}
