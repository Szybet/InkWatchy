#include "dailyStepsChartMenu.h"

#if PRECISE_STEP_COUNTING

void showDailyStepsChart()
{
    debugLog("showDailyStepsChart called");
    char dailyStepsDayChoosed[12] = {0};
    int dayIndex = lastMenuSelected.indexOf(".");
    if (dayIndex != -1)
    {
        lastMenuSelected.toCharArray(dailyStepsDayChoosed, 12);
        dailyStepsDayChoosed[11] = '\0';
    }
    else if (String(dailyStepsDayChoosed).indexOf(".") == -1)
    {
        debugLog("Error finding date for daily steps chart");
        overwriteSwitch(textDialog);
        showTextDialog(STEPS_DATE_WRONG, true);
        return;
    }

    String stepsDay = String(dailyStepsDayChoosed);
    debugLog("Showing daily steps for day: " + stepsDay);

    uint16_t preciseSteps[24];
    bufSize stepsData = fsGetBlob(String(dateToUnix(stepsDay)), String(PRECISE_STEP_COUNTING_DIR) + "/");

    if (stepsData.size != sizeof(preciseSteps))
    {
        debugLog("Precise steps data is bad.");
        free(stepsData.buf);
        overwriteSwitch(textDialog);
        showTextDialog(STEPS_CORRUPTED, true);
        return;
    }
    memcpy(&preciseSteps, stepsData.buf, stepsData.size);
    free(stepsData.buf);

    float stepsFloat[24];
    for (u8_t i = 0; i < 24; i++)
    {
        stepsFloat[i] = (float)preciseSteps[i];
    }

    showChart(stepsFloat, 24, STEPS_CHART_TITLE);
    generalSwitch(ChartPlace);
}

void initDailyStepsChartMenu()
{
    int days = 0;
    File root = LittleFS.open(PRECISE_STEP_COUNTING_DIR);
    if (!root)
    {
        debugLog("Failed to open precise step counting directory");
        overwriteSwitch(textDialog);
        showTextDialog(STEPS_NOT_AVAILABLE, true);
        return;
    }

    if (!root.isDirectory())
    {
        debugLog("Not a directory: " + String(PRECISE_STEP_COUNTING_DIR));
        root.close();
        overwriteSwitch(textDialog);
        showTextDialog(STEPS_NOT_AVAILABLE, true);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
            days++;
        file = root.openNextFile();
    }
    root.close();

    if (days == 0)
    {
        overwriteSwitch(textDialog);
        showTextDialog(STEPS_NOT_AVAILABLE, true);
        return;
    }

    long daysUnixList[days] = {0};
    root = LittleFS.open(PRECISE_STEP_COUNTING_DIR);
    file = root.openNextFile();
    u8_t c = 0;
    while (file)
    {
        if (!file.isDirectory())
        {
            String unixTmp = String(file.name());
            daysUnixList[c] = unixTmp.toInt();
            c++;
        }
        file = root.openNextFile();
    }
    root.close();
    std::sort(daysUnixList, daysUnixList + days);
    std::reverse(daysUnixList, daysUnixList + days);

    entryMenu buttons[days];
    for (u8_t i = 0; i < days; i++)
    {
        buttons[i] = {unixToDate(daysUnixList[i]), &emptyImgPack, showDailyStepsChart};
    }

    initMenu(buttons, days, DAILY_STEPS_TITLE);
}

#endif
