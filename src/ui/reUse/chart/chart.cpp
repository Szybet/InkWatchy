#include "chart.h"
#include "Grafici.h"

Grafici plot{*dis};

const ColorMapArray<2> really_black{
    black,
    black};

#define X_MINIMUM_VALUE_OFFSET -0.5
#define X_MAX_VALUE_OFFSET -0.5
#define CELL_VERTICAL_COUNT 6
#define WALL_VALUE_OFFSET float(0.08)

// Values in data1 need to be higher that in data2 in the same index
void showDoubleDataBarChart(float *data1Max, float *data2Min, uint dataCount, String chartName)
{
  setNativeScreenColors();
  dis->fillScreen(SCWhite);
  float highNum = findHighest(data1Max, dataCount);
  float lowNum = findLowest(data2Min, dataCount);

  float offsetMin = WALL_VALUE_OFFSET;
  float offsetMax = WALL_VALUE_OFFSET;
  if (dataCount == 1)
  {
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

  dis->setCursor(1, 1);
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
  resetScreenColors();
}

void showChart(float *data, uint dataCount, String chartName)
{
  setNativeScreenColors();
  dis->fillScreen(SCWhite);
  float highNum = findHighest(data, dataCount);
  float lowNum = findLowest(data, dataCount);

  float offsetMin = WALL_VALUE_OFFSET;
  float offsetMax = WALL_VALUE_OFFSET;
  if (dataCount == 1)
  {
    offsetMax = 0.0;
    offsetMin = 0.5;
  }
  DataArray<float> dataArr{data, dataCount, {lowNum - offsetMin, highNum + offsetMax}};

  DataLinear x{dataCount, {0.0 + X_MINIMUM_VALUE_OFFSET, float(dataCount + X_MAX_VALUE_OFFSET)}};

  plot.set_color_map(really_black);
  PlotOptions opts = plot_options.bar_filled(true).set_axis(CELL_VERTICAL_COUNT, dataCount, black);
  Window size = Window({0.1, 0.999}, {0.005, 0.89});

  plot.bar(x, dataArr, black, size, opts);

  dis->setCursor(1, 1);
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
  resetScreenColors();
}

String formatFloat(float num)
{
  String s = String(num);
  int dotZero = s.indexOf(".0");
  if (dotZero != -1)
    s = s.substring(0, dotZero);
  if (s.indexOf(".") != -1)
  {
    while (s.endsWith("0"))
    {
      s.remove(s.length() - 1);
    }
  }

  // Split
  // Horrible
  if (s.length() >= 3 && ((isdigit(s.charAt(0)) && isdigit(s.charAt(1)) && s.charAt(2) == '.') || (isdigit(s.charAt(0)) && s.charAt(1) == '.' && isdigit(s.charAt(2)))))
  {
    s = s.substring(0, 3) + "\n" + s.substring(3);
  }
  else
  {
    if (s.length() > 2)
    {
      s = s.substring(0, 2) + "\n" + s.substring(2);
    }
    else
    {
      s += "\n";
    }
  }

  // Ensure second line length
  if (s.indexOf('\n') != -1 && s.substring(s.indexOf('\n') + 1).length() > 2)
    s = s.substring(0, s.indexOf('\n') + 2);
  return s;
}

#define SIDE_TEXT_OFFSET_Y 40
#define INIT_REPAIR 10
#define NEW_LINE_OFFSET 8

void showOneString(float one)
{
  String str = formatFloat(one);
  debugLog("Str to format show: " + str);
  int newlineIndex = str.indexOf('\n');
  int16_t cursorX = dis->getCursorX();
  int16_t cursorY = dis->getCursorY();
  if (newlineIndex != -1)
  {
    String line1 = str.substring(0, newlineIndex);
    String line2 = str.substring(newlineIndex + 1);
    dis->setCursor(cursorX, cursorY);
    dis->print(line1);
    dis->setCursor(cursorX, cursorY + NEW_LINE_OFFSET);
    dis->print(line2);
  }
  else
  {
    dis->setCursor(cursorX, cursorY);
    dis->print(str);
  }
}

void showOne(float one)
{
  dis->setCursor(0, 200 - INIT_REPAIR);
  showOneString(one);
}

void showSideText(float *bothList, int bothSize)
{
  sortList(bothList, bothSize);
  dis->setFont(getFont("dogicapixel4"));
  if (bothSize == 1)
  {
    showOne(bothList[0]);
    return;
  }

  float max = bothList[0];
  float min = bothList[0];
  for (int i = 1; i < bothSize; i++)
  {
    if (bothList[i] > max)
    {
      max = bothList[i];
    }
    if (bothList[i] < min)
    {
      min = bothList[i];
    }
  }

  if (min == max)
  {
    showOne(min);
    return;
  }

  float newList[5];
  for (int i = 0; i < 5; i++)
  {
    newList[i] = min + ((max - min) * (i / 4.0f));
  }

  for (int i = 0; i < 5; i++)
  {
    dis->setCursor(0, (200 - (SIDE_TEXT_OFFSET_Y * i)) - INIT_REPAIR);
    showOneString(newList[i]);
  }
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
