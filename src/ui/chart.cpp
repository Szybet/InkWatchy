#include "chart.h"
#include "Grafici.h"

Grafici plot{display};

const ColorMapArray<2> really_black{
    black,
    black};

#define X_MINIMUM_VALUE_OFFSET -0.5
#define X_MAX_VALUE_OFFSET -0.5
#define CELL_VERTICAL_COUNT 6

// Values in data1 need to be higher that in data2 in the same index
void showDoubleDataBarChart(float *data1Max, float *data2Min, uint dataCount, String chartName)
{
  display.fillScreen(GxEPD_WHITE);
  float highNum = findHighest(data1Max, dataCount);
  float lowNum = findLowest(data2Min, dataCount);

  DataArray<float> data1Arr{data1Max, dataCount, {lowNum, highNum}};
  DataArray<float> data2Arr{data2Min, dataCount, {lowNum, highNum}};

  DataLinear x{dataCount, {0.0 + X_MINIMUM_VALUE_OFFSET, float(dataCount + X_MAX_VALUE_OFFSET)}};

  plot.set_color_map(really_black);
  PlotOptions opts = plot_options.bar_filled(true).set_axis(CELL_VERTICAL_COUNT, dataCount, black);
  Window size = Window({0.1, 0.999}, {0.005, 0.89});

  plot.bar(x, data1Arr, data2Arr, size, opts);
  plot.bar(x, data2Arr, white, size, opts);

  display.setCursor(1, 1);
  setTextSize(1);
  uint16_t y;
  getTextBounds(chartName, NULL, NULL, NULL, &y);
  writeTextCenterReplaceBack(chartName, y - 3);

  int bothSize = dataCount * 2;
  float bothList[bothSize];

#if DEBUG
  for (int i = 0; i < dataCount; i++)
  {
    debugLog("for i" + String(i) + " in max equals: " + String(data1Max[i]));
    debugLog("for i" + String(i) + " in min equals: " + String(data1Max[i]));
  }
#endif

  concatenateFloatLists(data1Max, dataCount, data2Min, dataCount, bothList);
  sortList(bothList, bothSize);

  debugLog(String(bothList[0]));
  debugLog(String(bothList[bothSize - 1]));

  int stepper = ceil(float((bothSize - 1)) / float(CELL_VERTICAL_COUNT));

  // To make sure good ommount of text is shown
  while (bothSize / stepper < CELL_VERTICAL_COUNT)
  {
    bothSize = bothSize + 1;
  }

  debugLog("Stepper: " + String(stepper));
  debugLog("bothSize: " + String(bothSize));

  display.setFont();
  float currentHeight = display.height() - 23;
  for (int i = 0; i < bothSize - 1; i += stepper)
  {
    display.setCursor(0, round(currentHeight));
    debugLog("i: " + String(i));
    String number = String(bothList[i]);
    if(number[1] == '.') {
      number.remove(number.length() - 1);
      if(number[2] == '0') {
        number.remove(number.length() - 1);
        number.remove(number.length() - 1);
      }
    }
    if (number.length() > 3)
    {
      if (isDecimalZero(bothList[i]) == true && String(int(bothList[i])).length() <= 3)
      {
        display.print(String(int(bothList[i])));
      }
      else
      {
        String part1 = number.substring(0, 3);
        String part2 = number.substring(3);
        if (part2.length() > 3)
        {
          part2 = part2.substring(0, 3);
        }
        display.println(part1);
        display.print(part2);
      }
    }
    else
    {
      display.print(number);
    }

    currentHeight = currentHeight - ((display.height() / (CELL_VERTICAL_COUNT + 1)) + 1.3);
  }
  setFont(font);

  disUp(true);
  debugLog("Finished printing things");
}

// Clear example
/*

// float cambridge_max_temp_22[] = {68.23, 29.87, 42.11, -20.5, 10.78, 50.32, 75.44, 84.69, 9.14, 61.98, 17.82, 98.63};
// float cambridge_min_temp_22[] = {61.42, 19.34, 26.08, -30.72, 6.91, 45.77, 62.46, 77.85, 3.74, 47.53, 14.25, 90.04};

float cambridge_max_temp_22[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
float cambridge_min_temp_22[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0};

DataArray<float> max_temps{cambridge_max_temp_22, 12, {0, 12}};
DataArray<float> min_temps{cambridge_min_temp_22, 12, {0, 12}};

DataLinear x{12, {-0.55, 11.5}};

void showChart()
{
  //plot.clear_screen(); // for now this writes with white color on black background so this is needed - add your color to make it work
  plot.set_color_map(really_black);

  // set barplot properties
  auto opts = plot_options.bar_filled(true).set_axis(6, 12, black);
  //opts.thickness(0.5);

  // bar plot max temperatures using temperature as a color
  full_screen.set_boundaries({0.1, 0.999}, {0.005, 0.89});
  //plot.bar(x, max_temps, black, full_screen, opts);
  plot.bar(x, max_temps, min_temps, full_screen, opts);

  // bar plot min temperatures using black as a color (mask)
  plot.bar(x, min_temps, white, full_screen, opts);
}
*/
