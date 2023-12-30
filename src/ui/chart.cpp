#include "chart.h"
#include "Grafici.h"

Grafici plot{display};

const ColorMapArray<2> really_black{
    black,
    black};

#define X_MINIMUM_VALUE_OFFSET -0.5
#define X_MAX_VALUE_OFFSET -0.5
#define CELL_VERTICAL_COUNT 6
#define WALL_VALUE_OFFSET float(0.08)
#define SIDE_TEXT_OFFSET 30
#define SIDE_TEXT_REPAIR 14.5

// Values in data1 need to be higher that in data2 in the same index
void showDoubleDataBarChart(float *data1Max, float *data2Min, uint dataCount, String chartName)
{
  display.fillScreen(GxEPD_WHITE);
  float highNum = findHighest(data1Max, dataCount);
  float lowNum = findLowest(data2Min, dataCount);

  float offsetMin = WALL_VALUE_OFFSET;
  float offsetMax = WALL_VALUE_OFFSET;
  if(dataCount == 1) {
    offsetMax = 0.0;
    offsetMin = 0.5;
  }

  DataArray<float> data1Arr{data1Max, dataCount, {lowNum - offsetMin, highNum + offsetMax}};
  DataArray<float> data2Arr{data2Min, dataCount, {lowNum - offsetMin, highNum + offsetMax}};

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
  if (containsBelowChar(chartName) == true)
  {
    y = y - 3;
  }
  writeTextCenterReplaceBack(chartName, y);

  int bothSize = dataCount * 2;
  float bothList[bothSize];

#if DEBUG
  for (int i = 0; i < dataCount; i++)
  {
    debugLog("for i" + String(i) + " in max equals: " + String(data1Max[i]));
    debugLog("for i" + String(i) + " in min equals: " + String(data2Min[i]));
  }
#endif

  concatenateFloatLists(data1Max, dataCount, data2Min, dataCount, bothList);
  
  showSideText(bothList, bothSize);

  disUp(true);
  debugLog("Finished printing things");
}

void showChart(float *data, uint dataCount, String chartName)
{
  display.fillScreen(GxEPD_WHITE);
  float highNum = findHighest(data, dataCount);
  float lowNum = findLowest(data, dataCount);

  float offsetMin = WALL_VALUE_OFFSET;
  float offsetMax = WALL_VALUE_OFFSET;
  if(dataCount == 1) {
    offsetMax = 0.0;
    offsetMin = 0.5;
  }
  DataArray<float> dataArr{data, dataCount, {lowNum - offsetMin, highNum + offsetMax}};

  DataLinear x{dataCount, {0.0 + X_MINIMUM_VALUE_OFFSET, float(dataCount + X_MAX_VALUE_OFFSET)}};

  plot.set_color_map(really_black);
  PlotOptions opts = plot_options.bar_filled(true).set_axis(CELL_VERTICAL_COUNT, dataCount, black);
  Window size = Window({0.1, 0.999}, {0.005, 0.89});

  plot.bar(x, dataArr, black, size, opts);

  display.setCursor(1, 1);
  setTextSize(1);
  uint16_t y;
  getTextBounds(chartName, NULL, NULL, NULL, &y);
  if (containsBelowChar(chartName) == true)
  {
    y = y - 3;
  }
  writeTextCenterReplaceBack(chartName, y);

  int bothSize = dataCount;
  float bothList[bothSize];

  for (int i = 0; i < dataCount; i++)
  {
    bothList[i] = data[i];
    debugLog("for i" + String(i) + " in data equals: " + String(data[i]));
  }

  showSideText(bothList, bothSize);

  disUp(true);
  debugLog("Finished printing things");
}

void showSideText(float *bothList, int bothSize) {

  sortList(bothList, bothSize);

  debugLog(String(bothList[0]));
  debugLog(String(bothList[bothSize - 1]));

  float stepper = float((bothSize)) / float(CELL_VERTICAL_COUNT);
  if (int(round(stepper)) == 0)
  {
    stepper = 1; // We can't divide by zero!
  }

  debugLog("Stepper: " + String(stepper));
  debugLog("bothSize: " + String(bothSize));

  int valuesCount = -1;
  float values[CELL_VERTICAL_COUNT] = {0};
  for (float i = 0; i < bothSize; i += stepper)
  {
    debugLog("Iterating: " + String(i));
    // Insanity
    int ti = int(floor(i));
    if (String(bothList[ti]) != "0.00" && String(bothList[ti]) != "-0.00" && String(bothList[ti]) != "0" && String(bothList[ti]) != "-0")
    {
      bool contains = false;
      for (int j = 0; j < valuesCount + 1; j++)
      {
        if (values[j] == bothList[ti])
        {
          contains = true;
        }
      }
      if (contains == false)
      {
        valuesCount = valuesCount + 1;
        debugLog("Value to show is: " + String(bothList[ti]) + " at value: " + String(valuesCount));
        values[valuesCount] = bothList[ti];
      }
      else
      {
        debugLog("duplicate");
      }
    }
    else
    {
      debugLog("Weird 0");
    }
  }
  valuesCount = valuesCount + 1; // To show actually the count
  sortList(values, valuesCount);

#if DEBUG
  for (int i = 0; i < valuesCount; i++)
  {
    debugLog("for i: " + String(i) + " is: " + String(values[i]));
  }
#endif

  debugLog("valuesCount: " + String(valuesCount));
  debugLog("Highest value: " + String(values[valuesCount - 1]));
  float currentHeightDown = display.height() - 23;
  float currentHeightUp = SIDE_TEXT_OFFSET;

  float offset = (display.height() - (SIDE_TEXT_OFFSET - SIDE_TEXT_REPAIR)) / valuesCount ;
  bool down = false;
  display.setFont();
  for (float i = 0; i < float(valuesCount) / 2; i += 0.5)
  {
    int ji = int(floor(i));
    debugLog("i: " + String(i));
    debugLog("ji: " + String(ji));

    if (down == true)
    {
      display.setCursor(0, round(currentHeightDown));
    }
    else
    {
      display.setCursor(0, round(currentHeightUp));
    }

    String number;
    if (down == true)
    {
      number = String(values[ji]);
    }
    else
    {
      number = String(values[valuesCount - ji - 1]);
    }
    debugLog("Number is:" + number);
    debugLog("Down is: " + BOOL_STR(down));

    int indexOfDecimal = number.indexOf(".");
    if(number.length() - 1 > indexOfDecimal && number[indexOfDecimal + 1] == '0') {
      number.remove(indexOfDecimal, number.length() - indexOfDecimal);
    }

    if (number.length() > 3)
    {
      /*
      if (isDecimalZero(bothList[i]) == true && String(int(bothList[i])).length() <= 3)
      {
        display.print(String(int(bothList[i])));
      }
      else
      */
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

    if (down == true)
    {
      currentHeightDown = currentHeightDown - offset;
    }
    else
    {
      currentHeightUp = currentHeightUp + offset;
    }
    down = !down;
  }

  if(values[0] == 0.0) {
    display.setCursor(0, round(currentHeightUp));
    display.print("0");
  }

  setFont(font);
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
