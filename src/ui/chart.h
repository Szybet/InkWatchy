#ifndef CHART_H
#define CHART_H

#include "../defines/defines.h"

void showDoubleDataBarChart(float* data1Max, float* data2Min, uint dataCount, String chartName);
void showChart(float *data, uint dataCount, String chartName);
void showSideText(float *bothList, int bothSize);

#endif
