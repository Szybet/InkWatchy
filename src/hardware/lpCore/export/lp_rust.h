#define LPOG_UNKNOWN_255 255

#define LPOG_NOTHING_0 0

#define LPOG_START_1 1

#define LPOG_SCREEN_WRITE_START_2 2

#define LPOG_INITIALIZED_PINS_3 3

#define LPOG_SCREEN_INIT_4 4

#define LPOG_SCREEN_FIRST_ACTION_5 5

#define LPOG_SCREEN_END_6 6

#define LPOG_SCREEN_POWEROFF_7 7

#define LPOG_ERROR_8 8

#define NUMBER_WIDTH 32

#define NUMBER_HEIGHT 48

#define TIMEZONE_OFFSET (1 * 3600)

#define WAKEUP_MODULO 29

uint8_t do_rust_things(void);

void send_debug_message(uint8_t mess);

void send_debug_message(uint8_t _mess);
