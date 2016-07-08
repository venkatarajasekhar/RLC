#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_
#include "../timeseries/TimeSeries.h"

void loadTimeSeries(vector<TimeSeries> &series, string filepath);

void loadTimeSeries(vector<TimeSeries> &series, int length, string filepath);
void loadTimeSeries(vector<TimeSeries> &series, int length, string filepath, int number);

#endif