#[cfg(feature = "debug")]
use core::ffi::c_char;
use core::ffi::c_int;

use crate::info;

#[cfg(feature = "debug")]
unsafe extern "C" {
    pub fn rust_ink_test() -> *const c_char;
    pub fn log_function_c(
        file: *const c_char,
        line: c_int,
        func: *const c_char,
        message: *const c_char,
    );
}

unsafe extern "C" {
    pub fn rustPanic();
    pub fn delayRust(timeMs: c_int);
    pub fn drawPixel(x: i16, y: i16, color: u16);
    pub fn updateScreen(reallyUpdate: bool, ignoreCounter: bool, ignoreSleep: bool);
    pub fn rustMicros() -> u32;
    pub fn rustButtons() -> u32;
    pub fn rustResetDelay();
    pub fn rustSetCpuSpeed(speed: c_int);
    pub fn rustGetCpuSpeed() -> c_int;
}

pub const RUST_BUTTON_NONE: u32 = 0;
pub const RUST_BUTTON_UP: u32 = 1;
pub const RUST_BUTTON_DOWN: u32 = 2;
pub const RUST_BUTTON_MENU: u32 = 3;
pub const RUST_BUTTON_MENU_LONG: u32 = 4;

#[derive(PartialEq)]
pub enum RustButton {
    NoButton,
    Up,
    Down,
    Menu,
    MenuLong,
}

pub fn get_buttons() -> RustButton {
    let the_button = unsafe {rustButtons()};
    match the_button {
        RUST_BUTTON_NONE => RustButton::NoButton,
        RUST_BUTTON_UP => RustButton::Up,
        RUST_BUTTON_DOWN => RustButton::Down,
        RUST_BUTTON_MENU => RustButton::Menu,
        RUST_BUTTON_MENU_LONG => RustButton::MenuLong,
        _ => RustButton::NoButton, // Fallback for unexpected values
    }
}

#[derive(PartialEq)]
pub enum CpuSpeed {
    Min,
    Normal,
    Max,
}

impl CpuSpeed {
    pub fn speed_to_int(&self) -> c_int {
        match self {
            CpuSpeed::Min => 0,
            CpuSpeed::Normal => 1,
            CpuSpeed::Max => 2,
        }
    }
}

pub fn set_cpu_speed(speed: &CpuSpeed) {
    unsafe {rustSetCpuSpeed(speed.speed_to_int());}
}

pub fn get_cpu_speed() -> CpuSpeed {
    let speed = unsafe {rustGetCpuSpeed()};
    match speed {
        0 => CpuSpeed::Min,
        1 => CpuSpeed::Normal,
        2 => CpuSpeed::Max,
        _ => {
            info!(format!("Unknown cpu speed: {}", speed));
            panic!("Unknown cpu speed");
        }
    }
}