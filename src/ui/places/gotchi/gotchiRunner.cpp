#include "gotchiPlace.h"

#if GOTCHI

#define TAMA_DISPLAY_FRAMERATE 3

#include "emulator/tamalib.h"
#include "emulator/hw.h"
#include "emulator/bitmaps.h"

#define GOTCHI_OFFSET_X
#define GOTCHI_OFFSET_Y

bool killYourself = false;

void displayTama();

/**** TamaLib Specific Variables ****/
uint16_t current_freq = 0;
bool_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
bool_t icon_buffer[ICON_NUM] = {0};
cpu_state_t cpuState;
/************************************/
uint64_t gotchiUnix = 0;
uint64_t initMillis = 0;

void saveState()
{
    cpu_get_state(&cpuState);

    // Save the state
    debugLog("Saving state");
    /*
    String path = String("/conf/") + String(CONF_GOTCHI_SAVE);
    if (fsFileExists(path) == true)
    {
        fsRemoveFile(path);
    }
    */

    if (cpuState.memory)
    {
        fsSetBlob(CONF_GOTCHI_SAVE, cpuState.memory, MEMORY_SIZE);
    }
    else
    {
        debugLog("cpuState.memory is invalid!");
    }

    // Saving also cpu state itself
    fsSetBlob(CONF_GOTCHI_CPU_SAVE, (uint8_t *)&cpuState, sizeof(cpu_state_t));
}

#define FORCE_NO_RESTORE_SAVE false

void restoreSave()
{
#if FORCE_NO_RESTORE_SAVE == false
    debugLog("Restoring save");
    bufSize buffCpu = fsGetBlob(CONF_GOTCHI_CPU_SAVE);
    if (buffCpu.size == sizeof(cpu_state_t))
    {
        debugLog("Restoring cpu");
        cpu_get_state(&cpuState);
        uint8_t *sourceBuff = buffCpu.buf;
        uint8_t *destinationBuff = (uint8_t *)&cpuState;
        u4_t *tmp = cpuState.memory;
        memcpy(destinationBuff, sourceBuff, sizeof(cpu_state_t));
        cpuState.memory = tmp;
        free(sourceBuff);
        cpu_set_state(&cpuState);
    }
    else
    {
        debugLog("Saved cpu is bad");
    }

    bufSize buffMem = fsGetBlob(CONF_GOTCHI_SAVE);
    if (buffMem.size == MEMORY_SIZE)
    {
        debugLog("Restoring memory");
        cpu_get_state(&cpuState);
        uint8_t *sourceBuff = buffMem.buf;
        uint8_t *destinationBuff = (uint8_t *)cpuState.memory;
        memcpy(destinationBuff, sourceBuff, MEMORY_SIZE);
        free(sourceBuff);
        cpu_set_state(&cpuState);
    }
    else
    {
        debugLog("Saved memory is bad");
    }
#endif
}

static void hal_halt(void)
{
    // debugLog("Halt???");
}

static void hal_log(log_level_t level, char *buff, ...)
{
    yield();
    // debugLog(buff);
}

#if DEBUG
uint64_t secTime = 0;
#endif

static timestamp_t hal_get_timestamp(void)
{
    yield();

#if DEBUG && true
    uint64_t seconds = (gotchiUnix) + ((millisBetter() - initMillis) / 1000);
    if (secTime != seconds)
    {
        debugLog("Seconds is: " + String(seconds));
        secTime = seconds;
    }
#endif

    // Animations are fast but times speeds up
    uint64_t us = (gotchiUnix * 1000000) + ((millisBetter() - initMillis) * 1000);
    // debugLog("us is: " + String(us));
    return us;

    // This way the time is accurate, but animations are slower
    // uint64_t seconds = (gotchiUnix) + ((millisBetter() - initMillis) / 1000);
    // debugLog("Seconds is: " + String(seconds));
    // return seconds * 1000000;

    // return millisBetter() * 1000;
}

static void hal_sleep_until(timestamp_t ts)
{
    int32_t remaining = (int32_t)(ts - hal_get_timestamp());
    if (remaining > 0)
    {
        debugLog("Sleeping: " + String(remaining));
        delayMicroseconds(remaining);
        // delay(1);
    }
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

#if GOTCHI_MOTOR
int64_t buzzerTimeout = 0;
#endif
static void hal_play_frequency(bool_t en)
{
#if GOTCHI_MOTOR
    if (millisBetter() - GOTCHI_MOTOR_DELAY_MS > buzzerTimeout)
    {
        debugLog("buzzer");
        buzzerTimeout = millisBetter();
        vibrateMotor(GOTCHI_MOTOR_MS);
    }
#endif
    yield();
}

static int hal_handler(void)
{
    if (gotchiButtonsEx.middle > 0)
    {
        // debugLog("Button middle clicked");
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

#if DEBUG && false
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
    // debugLog("Display tama called");
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

#define EVERY_X_DELAY 13

void gotchiRun(void *parameter)
{
    tamalib_register_hal(&hal);
    tamalib_set_framerate(TAMA_DISPLAY_FRAMERATE);
    tamalib_init(12500); // 12500
    esp_task_wdt_reset();

    // Here because tamalib_init resets the memory
    restoreSave();

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
            if (killYourself == true)
            {
                killYourself = false;
                vTaskDelete(NULL);
            }
        }
    }
}

#define RESET_GOTCHI_UNIX false
// #define RESET_GOTCHI_UNIX true

void startGotchiTask()
{
    debugLog("Init gotchi");
    gotchiBuffMutex.lock();
    gotchiBuff = new GFXcanvas1(200, 200, true);
    gotchiBuff->fillScreen(GxEPD_WHITE);
    gotchiBuffMutex.unlock();

    // Set up time
    initMillis = millisBetter();

    gotchiUnix = fsGetString(CONF_GOTCHI_UNIX, "0").toInt();
    if (gotchiUnix == 0 || RESET_GOTCHI_UNIX == true)
    {
        readRTC();
        gotchiUnix = getUnixTime(timeRTCUTC0);
        fsSetString(CONF_GOTCHI_UNIX, String(gotchiUnix));
    }

    gotchiUnix = getUnixTime(timeRTCUTC0) - gotchiUnix;
    debugLog("Gotchi unix on init is: " + String(gotchiUnix));

    // Reset tamalib
    current_freq = 0;
    memset(matrix_buffer, 0, sizeof(matrix_buffer));
    memset(icon_buffer, 0, sizeof(icon_buffer));
    cpuState = {0};

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
    killYourself = true;
    while (killYourself == true)
    {
        delayTask(5);
    }
    delayTask(30);
    gotchiHandle = NULL;
    delete gotchiBuff;

    saveState();
}

#endif