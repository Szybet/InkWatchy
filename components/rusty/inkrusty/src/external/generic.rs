#[cfg(feature = "debug")]
unsafe extern "C" {
    pub fn rust_ink_test() -> *const core::ffi::c_char;
}