#![no_std]
#![no_main]

pub mod external;

// use core::ffi::CStr;
use core::panic::PanicInfo;

#[cfg(feature = "debug")]
use crate::external::generic::rust_ink_test;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[cfg(feature = "debug")]
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_lib_test() -> *const core::ffi::c_char {
    /*
    CStr::from_bytes_with_nul(b"Rust works!\0")
        .expect("Invalid C string")
        .as_ptr()
    */
    
    unsafe{rust_ink_test()}
}

