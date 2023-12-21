#include "chart.h"
#include "Grafici.h"

Grafici plot{display};

// raw temperature data
// https://www.metoffice.gov.uk/pub/data/weather/uk/climate/stationdata/cambridgedata.txt
float cambridge_max_temp_22[] = {8.4, 10.9, 12.9, 15.1, 18.9, 21.6, 26.1, 26.6, 20.4, 18.3, 12.6, 7.0};
float cambridge_min_temp_22[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};

// create dataset from raw data and set minimum (0) and maximum (30) temperature values
DataArray<float> max_temps{cambridge_max_temp_22, 12, {0, 30}};
DataArray<float> min_temps{cambridge_min_temp_22, 12, {0, 12}};

// x values in [0,11] (position of the bar horizzontal center)
// range in [-0.5,11.5] to fully show the first and last bars
DataLinear x{12, {-0.5, 11.5}};

void showChart()
{
  plot.clear_screen(); // for now this writes with white color on black background so this is needed - add your color to make it work
  plot.set_color_map(temperature);

  // set barplot properties
  auto opts = plot_options.bar_filled(true).set_axis(6, 12, gunpowder);

  // bar plot max temperatures using temperature as a color
 // full_screen.set_boundaries({0.1, 0.999}, {0.005, 0.89});
  plot.bar(x, min_temps, min_temps, full_screen, opts);
}
