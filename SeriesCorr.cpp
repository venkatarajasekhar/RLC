#include "../timeseries/TimeSeries.h"

double stdDist(vector<Element> &standardized_points, TimeSeries &t, int os, int len, double range)
{
	double object_sum_x2 = 0, object_sum_x = 0;
	for(int i = t.len-1; i >= t.len-len; i--) {
        object_sum_x += t.x[i];
        object_sum_x2 += t.x[i]*t.x[i];
    }
    double mean = (object_sum_x) / len;
    double stdv = (object_sum_x2)/len - mean*mean;
	if(stdv <= 0.0) {
        return 0.0;
    }
	else {
		stdv =sqrt(stdv);
	}
	double dist_so_far = 0.0;
    for(int i = 0; i < standardized_points.size(); i++) {
		int object_series_id = standardized_points[i].id + t.len - len;
		double value_o = (t.x[object_series_id]-mean)/stdv;
		double value_q = standardized_points[i].dist;
		dist_so_far += pow(value_o-value_q,2);
        //if(dist_so_far < range) {
        //    return 999999.0;
        //}
    }
    return dist_so_far;
}


double stdCorr(TimeSeries &query, int query_os, int current_len, TimeSeries &oss, int object_os, double stop_value)
{
	vector<Element> normalized_points;
	double q_mean = query.mean(query_os,current_len);
	double q_stdv = query.stdv(query_os,current_len);
    for(int s = query_os; s <= query_os+current_len-1; s++) {
        Element temp;
        temp.id = s-query_os;
        temp.dist = (query.x[s] - q_mean)/q_stdv;
        normalized_points.push_back(temp);
    }
    sort(normalized_points.begin(),normalized_points.end(),compareElemAbosulteDesc);
	double dist = stdDist(normalized_points, oss, object_os, current_len, stop_value);
	//dist = dist*dist;
	double corr = 1-dist/(2*current_len);
	return corr;
}

vector<double> standardize(TimeSeries &ts, int os, int len)
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

double rawCorr(TimeSeries &query, int query_os, int current_len, TimeSeries &T, int object_os )
{
	double sum_qq=0.0, sum_oo=0.0, sum_qo=0.0, corr=-1.0;
	for(int t = 0; t <= current_len-1; t++){
		double q_avg=query.mean(query_os,current_len);
		double i = query.x[query_os+t] - q_avg;
		sum_qq += i*i;
		double o_avg=T.mean(object_os,current_len);
		double j =T.x[object_os+t]-o_avg;
		sum_oo +=j*j;
		sum_qo += i*j;
		double corr_cur = sum_qo/(sqrt(sum_qq)*sqrt(sum_oo));
		if(corr_cur>corr)
		{
			corr=corr_cur;
		}

	}
	return corr;
}