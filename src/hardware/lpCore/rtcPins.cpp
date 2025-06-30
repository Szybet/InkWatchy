#include "rtcPins.h"
#include "rtcMem.h"

/*
const RES_PIN: u8 = 2; // Output
const DC_PIN: u8 = 3; // Output
const BUSY_PIN: u8 = 4; // Input
pub const MOSI_PIN: u8 = 5; // Output
pub const SCK_PIN: u8 = 7; // Output
*/

void initRtcInvidualGpio(int pin, rtc_gpio_mode_t direction)
{
    // https://github.com/espressif/esp-idf/blob/3c99557eeea4e0945e77aabac672fbef52294d54/examples/system/ulp/ulp_riscv/gpio/main/ulp_riscv_example_main.c#L42
    gpio_num_t gpio_pin = gpio_num_t(pin);
    debugLog("Configuring RTC pin: " + String(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_init(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_set_direction(gpio_pin, direction));
    // ESP_ERROR_CHECK(rtc_gpio_isolate(gpio_pin)); // This fixes current draw but doesn't allow the lp core to use it
    ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_pullup_dis(gpio_pin));

    // debugLog("Got level for this gpio rtc: " + String(rtc_gpio_get_level(gpio_pin))); // Reports 0 for reset

    // This messes up gpio when the hp core is not going to sleep, probably
    // Or when it's not cleared up properly? - Nope, this is fine to call even without this, but this still messes things up, maybe its not needed at all
    // https://github.com/search?q=repo%3Aespressif%2Fesp-idf+rtc_gpio_hold_en&type=code
    // it's used only in risv and fsm, not lp core?
#if 0
    debugLog("Setting rtc_gpio_hold_en for pin: " + String(pin));
    // This increases power consumption a lot...
    ESP_ERROR_CHECK(rtc_gpio_hold_en(gpio_pin));
#endif

    // This increases power consumption too, wtf
    // ESP_ERROR_CHECK(rtc_gpio_hold_dis(gpio_pin));
}

// This, without the screen, gives +150uA
void initRtcGpio()
{
    // De init the screen before this!
#if ATCHY_VER == YATCHY
    // Also make sure about this
    // Set screen cs to low
    rM.gpioExpander.setPinMode(YATCHY_DISPLAY_CS, MCP_OUTPUT);
    // setPinPullUp(YATCHY_DISPLAY_CS, false); // Not needed, it's false at default
    rM.gpioExpander.setPinState(YATCHY_DISPLAY_CS, LOW);
#endif

    initRtcInvidualGpio(EPD_RESET, RTC_GPIO_MODE_OUTPUT_ONLY);        // This funny guy increases power consumption up to 600 uA :(
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_RESET), true)); // Fixes high fucking power consumption
    initRtcInvidualGpio(EPD_DC, RTC_GPIO_MODE_OUTPUT_ONLY);
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(EPD_DC), true)); // Fixes high fucking power consumption
    initRtcInvidualGpio(EPD_BUSY, RTC_GPIO_MODE_INPUT_ONLY);
    initRtcInvidualGpio(EPD_SPI_MOSI, RTC_GPIO_MODE_OUTPUT_ONLY);
    initRtcInvidualGpio(EPD_SPI_SCK, RTC_GPIO_MODE_OUTPUT_ONLY);
}

void deInitRtcInvidualGpio(int pin)
{
    gpio_num_t gpio_pin = gpio_num_t(pin);
    debugLog("Deconfiguring RTC pin: " + String(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_deinit(gpio_pin));
    ESP_ERROR_CHECK(rtc_gpio_hold_dis(gpio_pin));
}

void deInitRtcGpio()
{
    deInitRtcInvidualGpio(EPD_RESET);
    deInitRtcInvidualGpio(EPD_DC);
    deInitRtcInvidualGpio(EPD_BUSY);
    deInitRtcInvidualGpio(EPD_SPI_MOSI);
    deInitRtcInvidualGpio(EPD_SPI_SCK);
}