#include "heartMonitor.h"
#include "rtcMem.h"

#if HEART_MONITOR
TaskHandle_t heartRateHandle;
/*
void heartrateHandle(void *parameter)
{
    debugLog("Starting heart rate monitor");
    uint64_t startMillHeart = millisBetter();
    while (true)
    {
        Accel acc;
        rM.SBMA.getAccel(&acc);


        if(millisBetter() - startMillHeart > 60 * 1000) {
            break;
        }
    }
    debugLog("Exiting");
    vTaskDelete(NULL);
}
*/

uint64_t startMillHeart = 0;
uint16_t heartBeats = 0;
bool started = false;

void heartrateHandle(void *parameter)
{
    debugLog("Starting heart rate monitor");

    // Configuration
    const uint16_t MEDIAN_WINDOW = 31;  // Must be odd
    const uint16_t NOISE_WINDOW = 60;   // Noise calculation samples
    const uint8_t SPIKE_MIN_WIDTH = 5;  // Minimum duration
    const uint16_t REFRACTORY_MS = 300; // 300ms lockout (43bpm max)
    const int16_t ENTRY_THRESHOLD = 12; // Minimum spike height
    const int16_t EXIT_THRESHOLD = 6;   // Must drop below this to reset

    // State variables
    int16_t median_buf[MEDIAN_WINDOW] = {0};
    int16_t noise_buf[NOISE_WINDOW] = {0};
    uint32_t spike_count = 0;
    uint16_t buf_index = 0;
    uint16_t noise_index = 0;
    uint32_t last_spike_time = 0;
    int32_t noise_floor = 1000;
    int16_t peak_height = 0;
    uint8_t spike_width = 0;
    bool in_spike = false;
    bool lockout = false;

    vibrateMotor(VIBRATION_ACTION_TIME);
    delayTask(3000);
    startMillHeart = millisBetter();

    // Horrible AI helped code, I'm sorry for being lazy
    while (true)
    {
        uint32_t current_time = millisBetter();
        if (current_time - startMillHeart > 15 * 1000)
            break;

        Accel acc;
        rM.SBMA.getAccelPure(&acc);
        int16_t current_z = acc.z;

        // 1. Median Filtering
        median_buf[buf_index % MEDIAN_WINDOW] = current_z;

        // Partial sort for median
        int16_t sorted[MEDIAN_WINDOW];
        memcpy(sorted, median_buf, sizeof(sorted));
        for (uint16_t i = 0; i <= MEDIAN_WINDOW / 2; i++)
        {
            uint16_t min_idx = i;
            for (uint16_t j = i + 1; j < MEDIAN_WINDOW; j++)
            {
                if (sorted[j] < sorted[min_idx])
                    min_idx = j;
            }
            int16_t temp = sorted[i];
            sorted[i] = sorted[min_idx];
            sorted[min_idx] = temp;
        }
        int16_t baseline = sorted[MEDIAN_WINDOW / 2];

        // 2. Residual Calculation
        int16_t residual = current_z - baseline;
        int16_t abs_residual = abs(residual);

        // 3. Noise Floor Update (only during quiet periods)
        if (!lockout && (current_time - last_spike_time) > REFRACTORY_MS)
        {
            noise_buf[noise_index % NOISE_WINDOW] = abs_residual < ENTRY_THRESHOLD ? abs_residual : 0;
            noise_index++;

            if (noise_index >= NOISE_WINDOW)
            {
                int32_t sum = 0;
                uint16_t count = 0;
                for (uint16_t i = 0; i < NOISE_WINDOW; i++)
                {
                    if (noise_buf[i] < ENTRY_THRESHOLD)
                    {
                        sum += noise_buf[i];
                        count++;
                    }
                }
                noise_floor = count > 0 ? sum / count : ENTRY_THRESHOLD;
                noise_index = 0;
            }
        }

        // 4. Spike Detection with Dual Thresholds
        if (lockout)
        {
            // Check exit condition
            if (abs_residual < EXIT_THRESHOLD && (current_time - last_spike_time) > REFRACTORY_MS)
            {
                lockout = false;
            }
        }
        else
        {
            if (abs_residual > ENTRY_THRESHOLD)
            {
                spike_width++;

                // Track peak height
                if (abs_residual > peak_height)
                {
                    peak_height = abs_residual;
                }

                // Validate spike
                if (spike_width >= SPIKE_MIN_WIDTH)
                {
                    spike_count++;
                    last_spike_time = current_time;
                    lockout = true;
                    debugLog("Valid spike: " + String(peak_height));
                    peak_height = 0;
                    spike_width = 0;
                }
            }
            else
            {
                // Reset if below entry threshold
                spike_width = 0;
                peak_height = 0;
            }
        }

        buf_index++;
    }

    debugLog("Final spike count: " + String(spike_count));
    heartBeats = spike_count * 3.65; // 0.35 for false positives
    debugLog("Heart beats per minute: " + String(heartBeats));
    started = false;
    vTaskDelete(NULL);
}

void start()
{
    initAcc();
    rM.SBMA.enableAccel();
    heartRateHandle = NULL;

    xTaskCreate(
        heartrateHandle,
        "heartrateMonitor",
        5000,
        NULL,
        24,
        &heartRateHandle);
}

uint16_t lastHeight = 0;
void initHeartMonitor()
{

    uint16_t h;
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    dis->setCursor(0, 1);
    String menuName = "Heart rate monitor";
    getTextBounds(menuName, NULL, NULL, NULL, &h);
    maxHeight = h + 3;
    uint16_t currentHeight = maxHeight;

    writeLine(menuName, 1, &currentHeight);

    writeLine("Not for medical use", 1, &currentHeight);

    writeLine("Place device on heart", 1, &currentHeight);

    writeLine("Then click menu", 1, &currentHeight);

    writeLine("It will start in 3s", 1, &currentHeight);

    writeLine("It takes 15s", 1, &currentHeight);

    writeLine("Try to breath slowly", 1, &currentHeight);
    lastHeight = currentHeight + 30;
    disUp(true);

    started = false;
    startMillHeart = 0;
    heartBeats = 0;
}

void pineapple() {
    writeImageN(185, 185, getImg("heartmonitor/pineapple"));
}

void loopHeartMonitor()
{
    buttonState btn = useButton();
    switch (btn)
    {
    case Menu:
    {
        if (started == false)
        {
            started = true;
            start();
        }
    }
    }

    if (started == true)
    {
        if (startMillHeart != 0)
        {
            delayTask(700); // Priority the other task
            int16_t time = 15 - (millisBetter() - startMillHeart) / 1000;
            if(time > 15) {
                time = 15;
            }
            if(time > 1) {
                writeTextCenterReplaceBack("Time left: " + String(time), lastHeight);
                pineapple();
                dUChange = true;    
            }
        }
    }
    else
    {
        if (heartBeats != 0)
        {
            writeTextCenterReplaceBack(String("= ") + String(heartBeats) + String("bpm"), lastHeight);
            pineapple();
            heartBeats = 0;
            dUChange = true;
            vibrateMotor(VIBRATION_ACTION_TIME);
        }
    }

    resetSleepDelay();
    disUp();
}

void exitHeartMonitor()
{
    if (heartRateHandle != NULL)
    {
        eTaskState state = eTaskGetState(heartRateHandle);
        if (state == eRunning)
        {
            vTaskDelete(heartRateHandle);
        }
    }
    heartRateHandle = NULL;

    rM.SBMA.disableAccel();
}

#endif