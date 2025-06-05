#![no_std]
#![no_main]

// Alloc things
extern crate alloc;
use mallocator::Mallocator;
#[global_allocator]
static A: Mallocator = Mallocator;

// Panic things
#[cfg(feature = "debug")]
use core::ffi::*;
use core::panic::PanicInfo;
use external::generic::rustPanic;
#[cfg(feature = "debug")]
use {
    alloc::ffi::CString,
    external::generic::{delayRust, log_function_c},
};

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    #[cfg(feature = "debug")]
    {
        let location = _info.location();
        let file: *const u8 = match location {
            Some(loc) => loc.file().as_ptr(),
            None => b"unknown\0".as_ptr(),
        };
        let line: c_int = location.map(|loc| loc.line() as c_int).unwrap_or(0);
        let func: *const u8 = b"unknown\0".as_ptr();

        // let message: *const u8 = _info.message().as_str().unwrap_or("fuck").as_ptr();
        // Fixes null termination
        let message_str = _info.message().as_str().unwrap_or("unknown");
        let c_string = CString::new(message_str)
            .unwrap_or_else(|_| CString::new("unknown").expect("Failed to create default CString"));
        let message: *const u8 = c_string.as_ptr();

        unsafe { log_function_c(file, line, func, message) };

        // Wait
        unsafe { delayRust(1500) };
    }
    // We want to panic
    unsafe { rustPanic() };
    loop {}
}

// Rest
pub mod apps;
pub mod external;
pub mod graphics;
pub mod logs;

#[cfg(feature = "debug")]
use crate::external::generic::rust_ink_test;
// use core::ffi::CStr;

#[cfg(feature = "debug")]
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_lib_test() -> *const core::ffi::c_char {
    /*
    CStr::from_bytes_with_nul(b"Rust works!\0")
        .expect("Invalid C string")
        .as_ptr()
    */
    info!("Rust calls a log!");
    unsafe { rust_ink_test() }
}
