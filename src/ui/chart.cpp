#include "chart.h"
#include "Grafici.h"

Grafici plot{display};

const ColorMapArray<2> really_black{
	black,
	black
};

#define X_MINIMUM_VALUE_OFFSET -0.55
#define X_MAX_VALUE_OFFSET -0.5

void showDoubleDataBarChart(float* data1Max, float* data2Min, uint dataCount, String chartName) {
  // wrong! get the max value and then replace datacount in some places
  DataArray<float> data1Arr {data1Max, dataCount, {0, float(dataCount)}};
  DataArray<float> data2Arr {data2Min, dataCount, {0, float(dataCount)}};

  DataLinear x{dataCount, {0.0 + X_MINIMUM_VALUE_OFFSET, float(dataCount + X_MAX_VALUE_OFFSET)}};

  plot.set_color_map(really_black);
  PlotOptions opts = plot_options.bar_filled(true).set_axis(6, dataCount, black);
  Window size = Window({0.1, 0.999}, {0.005, 0.89});

  plot.bar(x, data1Arr, data2Arr, size, opts);

  plot.bar(x, data2Arr, white, size, opts);
}


// Clear example
/*
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
