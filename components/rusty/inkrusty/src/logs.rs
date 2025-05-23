#[cfg(feature = "debug")]
#[macro_export]
macro_rules! info {
    ($message:expr) => {
        $crate::logs::log_internal(file!(), line!(), module_path!(), $message)
    };
}

#[cfg(feature = "debug")]
pub fn log_internal(file: &'static str, line: u32, func: &'static str, message: &str) {
    use alloc::ffi::CString;
    use core::ffi::c_char;

    let file_cstr = CString::new(file).expect("CString::new failed for file");
    let func_cstr = CString::new(func).expect("CString::new failed for func");
    let message_cstr = CString::new(message).expect("CString::new failed for message");

    let file_ptr: *const c_char = file_cstr.as_ptr();
    let func_ptr: *const c_char = func_cstr.as_ptr();
    let message_ptr: *const c_char = message_cstr.as_ptr();

    unsafe {
        crate::external::generic::log_function_c(file_ptr, line as i32, func_ptr, message_ptr);
    }
}

#[cfg(not(feature = "debug"))]
#[macro_export]
macro_rules! info {
    ($message:expr) => {};
}
