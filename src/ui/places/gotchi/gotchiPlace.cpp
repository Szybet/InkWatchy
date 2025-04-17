#include "gotchiPlace.h"

#if GOTCHI

#define TAMA_DISPLAY_FRAMERATE 3

#include "emulator/tamalib.h"
#include "emulator/hw.h"
#include "emulator/bitmaps.h"

void displayTama();

/**** TamaLib Specific Variables ****/
static uint16_t current_freq = 0;
static bool_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
// static byte runOnceBool = 0;
static bool_t icon_buffer[ICON_NUM] = {0};
static cpu_state_t cpuState;
static unsigned long lastSaveTimestamp = 0;
/************************************/

static void hal_halt(void)
{
    //debugLog("Halt???");
}

static void hal_log(log_level_t level, char *buff, ...)
{
    //debugLog(buff);
}

static timestamp_t hal_get_timestamp(void)
{
    return millis() * 1000;
}

static void hal_sleep_until(timestamp_t ts)
{
    // int32_t remaining = (int32_t) (ts - hal_get_timestamp());
    // if (remaining > 0) {
    // delayMicroseconds(1);
    // delay(1);
    //}
}

static void hal_update_screen(void)
{
    displayTama();
}

static void hal_set_lcd_matrix(u8_t x, u8_t y, bool_t val)
{
    uint8_t mask;
    if (val)
    {
        mask = 0b10000000 >> (x % 8);
        matrix_buffer[y][x / 8] = matrix_buffer[y][x / 8] | mask;
    }
    else
    {
        mask = 0b01111111;
        for (byte i = 0; i < (x % 8); i++)
        {
            mask = (mask >> 1) | 0b10000000;
        }
        matrix_buffer[y][x / 8] = matrix_buffer[y][x / 8] & mask;
    }
}

static void hal_set_lcd_icon(u8_t icon, bool_t val)
{
    icon_buffer[icon] = val;
}

static void hal_set_frequency(u32_t freq)
{
    current_freq = freq;
}

static void hal_play_frequency(bool_t en)
{
    debugLog("buzzer");
}

static bool_t button4state = 0;

static int hal_handler(void)
{
    unsigned long now = millis();

    buttonState btn = useButton();
    if (buttonRead(MENU_PIN) == BUT_CLICK_STATE)
    {
        hw_set_button(BTN_MIDDLE, BTN_STATE_PRESSED);
        debugLog("buutton");
    }
    else
    {
        hw_set_button(BTN_MIDDLE, BTN_STATE_RELEASED);
    }
    if (buttonRead(UP_PIN) == BUT_CLICK_STATE)
    {
        hw_set_button(BTN_RIGHT, BTN_STATE_PRESSED);
    }
    else
    {
        hw_set_button(BTN_RIGHT, BTN_STATE_RELEASED);
    }
    if (buttonRead(DOWN_PIN) == BUT_CLICK_STATE)
    {
        hw_set_button(BTN_LEFT, BTN_STATE_PRESSED);
    }
    else
    {
        hw_set_button(BTN_LEFT, BTN_STATE_RELEASED);
    }
    return 0;
}

static hal_t hal = {
    .halt = &hal_halt,
    .log = &hal_log,
    .sleep_until = &hal_sleep_until,
    .get_timestamp = &hal_get_timestamp,
    .update_screen = &hal_update_screen,
    .set_lcd_matrix = &hal_set_lcd_matrix,
    .set_lcd_icon = &hal_set_lcd_icon,
    .set_frequency = &hal_set_frequency,
    .play_frequency = &hal_play_frequency,
    .handler = &hal_handler,
};

void drawTriangle(uint8_t x, uint8_t y)
{
    // display.drawLine(x,y,x+6,y);
    dis->drawLine(x + 1, y + 1, x + 5, y + 1, GxEPD_BLACK);
    dis->drawLine(x + 2, y + 2, x + 4, y + 2, GxEPD_BLACK);
    dis->drawLine(x + 3, y + 3, x + 3, y + 3, GxEPD_BLACK);
}

void drawTamaRow(uint8_t tamaLCD_y, int ActualLCD_y, int height)
{
    uint8_t i;
    for (i = 0; i < LCD_WIDTH; i++)
    {
        uint8_t mask = 0b10000000;
        mask = mask >> (i % 8);
        if ((matrix_buffer[tamaLCD_y][i / 8] & mask) != 0)
        {
            dis->fillRect(i * 6, ActualLCD_y, 5, height + 3, GxEPD_BLACK);
        }
        else
        {
            dis->fillRect(i * 6, ActualLCD_y, 5, height + 3, GxEPD_WHITE);
        }
    }
}

void drawTamaSelection(uint8_t y)
{
    uint8_t i;
    for (i = 0; i < 7; i++)
    {
        if (icon_buffer[i])
        {
            dis->fillRect(i * 16 + 4, y + 6, 16, 9, GxEPD_BLACK);
            dis->drawBitmap(i * 16 + 4, y + 6, gotchiBitmaps + i * 18, 16, 9, GxEPD_WHITE);
        }
        else
        {
            dis->drawBitmap(i * 16 + 4, y + 6, gotchiBitmaps + i * 18, 16, 9, GxEPD_BLACK);
        }
    }
    if (icon_buffer[7])
    {
        drawTriangle(7 * 16 + 5, y);
        dis->drawBitmap(7 * 16 + 4, y + 6, gotchiBitmaps + 7 * 18, 16, 9, GxEPD_BLACK);
    }
}

unsigned long lastDisplay = 0;

void displayTama()
{
    debugLog("Display tama called");
    uint8_t j;
    for (j = 0; j < LCD_HEIGHT; j++)
    {
        drawTamaRow(j, j * 6, 2);
    }
    dUChange = true;
    unsigned long now = millis();
    if (now - lastDisplay >= 2000)
    {
        debugLog("Drawing the fucking screen");
        drawTamaSelection(100);
        lastDisplay = now;
        disUp(dUChange, false, true);
    }
}

uint8_t reverseBits(uint8_t num)
{
    uint8_t reverse_num = 0;
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((8 - 1) - i);
    }
    return reverse_num;
}

void initGotchi()
{
    debugLog("Init gotchi");
    tamalib_register_hal(&hal);
    tamalib_set_framerate(TAMA_DISPLAY_FRAMERATE);
    tamalib_init(1000000);
}

void loopGotchi()
{
    tamalib_mainloop_step_by_step();
    // debugLog("Looping gotchi");
}

void exitGotchi() {}

#endif