#include "../timeseries/TimeSeries.h"

extern double stdDist(vector<Element> &standardized_points, TimeSeries &t, int os, int len, double range);
extern double stdCorr(TimeSeries &query, int query_os, int current_len, TimeSeries &oss, int object_os, double stop_value);
extern vector<double> standardize(TimeSeries &ts, int os, int len);
extern double rawCorr(TimeSeries &query, int query_os, int current_len, TimeSeries &T, int object_os );