use core::ffi::c_char;
use crate::CREDITS_NICKS;

pub mod generic;

#[no_mangle]
pub extern "C" fn get_credits_nicks() -> *const c_char {
    CREDITS_NICKS.as_ptr() as *const c_char
}
