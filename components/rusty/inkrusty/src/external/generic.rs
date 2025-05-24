#[cfg(feature = "debug")]
use core::ffi::c_char;
use core::ffi::c_int;

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
}