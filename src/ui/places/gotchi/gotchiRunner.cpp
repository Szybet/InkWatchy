#include "gotchiPlace.h"

#if GOTCHI

#define TAMA_DISPLAY_FRAMERATE 3

#include "emulator/tamalib.h"
#include "emulator/hw.h"
#include "emulator/bitmaps.h"

#define GOTCHI_OFFSET_X
#define GOTCHI_OFFSET_Y

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
    // debugLog("Halt???");
}

static void hal_log(log_level_t level, char *buff, ...)
{
    yield();
    // debugLog(buff);
}

static timestamp_t hal_get_timestamp(void)
{
    yield();
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
    // debugLog("buzzer");
    yield();
}

static int hal_handler(void)
{
    if (gotchiButtonsEx.middle > 0)
    {
        debugLog("Button middle clicked");
        gotchiButtonsEx.middle = gotchiButtonsEx.middle - 1;
        hw_set_button(BTN_MIDDLE, BTN_STATE_PRESSED);
    }
    else
    {
        hw_set_button(BTN_MIDDLE, BTN_STATE_RELEASED);
    }

    if (gotchiButtonsEx.right > 0)
    {
        gotchiButtonsEx.right = gotchiButtonsEx.right - 1;
        hw_set_button(BTN_RIGHT, BTN_STATE_PRESSED);
    }
    else
    {
        hw_set_button(BTN_RIGHT, BTN_STATE_RELEASED);
    }

    if (gotchiButtonsEx.left > 0)
    {
        gotchiButtonsEx.left = gotchiButtonsEx.left - 1;
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

void drawTriangle(uint8_t x, uint8_t y, uint16_t color)
{
    // display.drawLine(x,y,x+6,y);
    gotchiBuff->drawLine(x + 1, y + 1, x + 5, y + 1, color);
    gotchiBuff->drawLine(x + 2, y + 2, x + 4, y + 2, color);
    gotchiBuff->drawLine(x + 3, y + 3, x + 3, y + 3, color);
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
            gotchiBuff->fillRect(i * 6, ActualLCD_y, 5, height + 3, GxEPD_BLACK);
        }
        else
        {
            gotchiBuff->fillRect(i * 6, ActualLCD_y, 5, height + 3, GxEPD_WHITE);
        }
    }
}

void drawTamaSelection(uint8_t y)
{
    for (int i = 0; i < 7; i++)
    {
        if (icon_buffer[i])
        {
            drawTriangle(i * 16 + 5, y, GxEPD_BLACK);
        }
        else
        {
            drawTriangle(i * 16 + 5, y, GxEPD_WHITE);
        }
        gotchiBuff->drawBitmap(i * 16 + 4, y + 6, gotchiBitmaps + i * 18, 16, 9, GxEPD_BLACK);
    }
    if (icon_buffer[7])
    {
        drawTriangle(7 * 16 + 5, y, GxEPD_BLACK);
        gotchiBuff->drawBitmap(7 * 16 + 4, y + 6, gotchiBitmaps + 7 * 18, 16, 9, GxEPD_BLACK);
    }
    else
    {
        drawTriangle(7 * 16 + 5, y, GxEPD_WHITE);
        gotchiBuff->drawBitmap(7 * 16 + 4, y + 6, gotchiBitmaps + 7 * 18, 16, 9, GxEPD_WHITE);
    }

#if DEBUG
    for (int i = 0; i < 8; i++)
    {
        if (icon_buffer[i])
        {
            debugLog("We are at icon: " + String(i));
        }
    }
#endif
}

// bool displayTamaCalled = false;
void displayTama()
{
    debugLog("Display tama called");
    gotchiBuffMutex.lock();
    for (int j = 0; j < LCD_HEIGHT; j++)
    {
        drawTamaRow(j, j * 6, 2);
    }
    yield();
    drawTamaSelection(108);
    gotchiBuffMutex.unlock();
    yield();
    // displayTamaCalled = true;
}

gotchiButtons gotchiButtonsEx = {0, 0, 0};

#define GOTCHI_TASK_STACK_SIZE 20000
#define GOTCHI_TASK_PRIORITY 6
std::mutex gotchiBuffMutex;
GFXcanvas1 *gotchiBuff;

TaskHandle_t gotchiHandle;

#define EVERY_X_DELAY 250

void gotchiRun(void *parameter)
{
    tamalib_register_hal(&hal);
    tamalib_set_framerate(TAMA_DISPLAY_FRAMERATE);
    tamalib_init(12500);
    esp_task_wdt_reset();

    uint32_t c = 0;
    while (true)
    {
        tamalib_mainloop_step_by_step();
        c = c + 1;
        if (c > EVERY_X_DELAY)
        {
            vTaskDelay(1);
            // displayTamaCalled = false;
            c = 0;
        }
    }
}

void startGotchiTask()
{
    debugLog("Init gotchi");
    gotchiBuffMutex.lock();
    gotchiBuff = new GFXcanvas1(200, 200, true);
    gotchiBuff->fillScreen(GxEPD_WHITE);
    gotchiBuffMutex.unlock();

    xTaskCreate(
        gotchiRun,
        "gotchiTask",
        GOTCHI_TASK_STACK_SIZE,
        NULL,
        GOTCHI_TASK_PRIORITY,
        &gotchiHandle);
}

void endGotchiTask()
{
    delete gotchiBuff;
}

#endif