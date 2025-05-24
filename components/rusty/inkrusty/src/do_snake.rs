use core::{cell::Cell, time::Duration};

use alloc::rc::Rc;
use slint::{ComponentHandle, SharedString};
use snake_lib::{
    consts::LOOP_DELAY_MS, game::{key_to_direction, Direction, Snake}, get_snake_app_window
};

use crate::{
    external::generic::rustMicros,
    graphics::slint_platform::{InkPlatform, slint_init},
};

pub fn random_snake() -> u64 {
    unsafe { rustMicros() as u64 }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn init_snake() {
    InkPlatform::set_platform();
    
    let ui = get_snake_app_window();

    let weak_ui = ui.as_weak();

    // Use Rc<Cell<>> for shared ownership
    let dir = Rc::new(Cell::new(Direction::None));

    // Clone for key handler
    let dir_key = Rc::clone(&dir);
    let weak_ui_key = weak_ui.clone();
    ui.on_key_pressed(move |key: SharedString| {
        if weak_ui_key.upgrade().is_some() {
            dir_key.set(key_to_direction(key));
        }
    });

    let mut snake = Snake::new(random_snake);
    let timer = slint::Timer::default();

    // Clone for timer closure
    let dir_timer = Rc::clone(&dir);
    let weak_ui_timer = weak_ui.clone();
    timer.start(
        slint::TimerMode::Repeated,
        Duration::from_millis(LOOP_DELAY_MS),
        move || {
            let ui = weak_ui_timer.upgrade().unwrap();

            // Get and reset direction atomically
            let current_dir = dir_timer.replace(Direction::None);

            let mut temp_dir = current_dir;
            snake.game_loop(&mut temp_dir);

            // Store back any modified direction
            dir_timer.set(temp_dir);

            snake.draw(&ui);
        },
    );

    slint_init(ui);
}
