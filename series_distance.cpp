#include "../timeseries/TimeSeries.h"

double standardized_distance(vector<Element> &standardized_points, TimeSeries &t, int os, int len, double range)
{
	double object_sum_x2 = 0, object_sum_x = 0;
	for(int i = t.len-1; i >= t.len-len; i--) {
        object_sum_x += t.x[i];
        object_sum_x2 += t.x[i]*t.x[i];
    }
    double mean = (object_sum_x) / len;
    double stdv = (object_sum_x2)/len - mean*mean;
	if(stdv <= 0.0) {
        return 999999.0;
    }
	else {
		stdv =sqrt(stdv);
	}
	double dist_so_far = 0.0;
    for(int i = 0; i < standardized_points.size(); i++) {
		int object_series_id = standardized_points[i].id + t.len - len;
		double value_s = (t.x[object_series_id]-mean)/stdv;
		double value_t = standardized_points[i].dist;
		dist_so_far += pow(value_s-value_t,2);
        if(dist_so_far > range) {
            return 999999.0;
        }
    }
    return dist_so_far;
}

double standardized_distance(TimeSeries &query, int query_os, int query_len, TimeSeries &object_series, int object_os, int object_len, double stop_value)
{
	vector<Element> normalized_points;
    for(int s = query_os; s <= query_os+query_len-1; s++) {
        Element temp;
        temp.id = s-query_os;
        temp.dist = (query.x[s] - query.mean(query_os,query_len) )/query.stdv(query_os,query_len);
        normalized_points.push_back(temp);
    }
    sort(normalized_points.begin(),normalized_points.end(),compareElemAbosulteDesc);
	return standardized_distance(normalized_points, object_series, object_os, object_len, stop_value);
}

vector<double> standardized_series(TimeSeries &ts, int os, int len)
{
	vector<double> normalized_points;
	double sum_x2 = 0;
	double sum_x = 0;
	for (int i=os; i<os+len; i++)
	{
		sum_x2 += ts.x[i]*ts.x[i];
		sum_x += ts.x[i];
	}
	double mean = sum_x/len;
	double stdv = sum_x2/len-mean*mean;
	for(int s = os; s <= os+len-1; s++) {
        double dist = (ts.x[s] - mean )/stdv;
        normalized_points.push_back(dist);
    }
	return normalized_points;
}

double orignal_distance(TimeSeries &s, int s_os, int s_len, TimeSeries &t, int t_os, int t_len)
{
	return 0.0;
}