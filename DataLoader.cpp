#include "DataLoader.h"

void loadTimeSeries(vector<TimeSeries> &series, string filepath)
{
	vector<double> ve;
	TimeSeries ts;
	ifstream is(filepath);
	double value;

	while (is>>value)
	{
		ve.push_back(value);
	}
	ts.init_1(ve);
	series.push_back(ts);
}

void loadTimeSeries(vector<TimeSeries> &series, int length, string filepath)
{
	vector<double> ve;
	ifstream is(filepath);
	double value;
	int curlen = 0;

	while (is>>value)
	{
		ve.push_back(value);
		curlen++;
		if (curlen == length) {
			TimeSeries ts;
			ts.init_1(ve);
			series.push_back(ts);

			curlen = 0;
			ve.clear();
		}
	}
}


void loadTimeSeries(vector<TimeSeries> &series, int length, string filepath, int number)
{
	vector<double> ve;
	ifstream is(filepath);
	double value;
	int curlen = 0;
	int number_count = 0;

	while (is>>value)
	{
		ve.push_back(value);
		curlen++;
		if (curlen == length) {
			TimeSeries ts;
			ts.init_1(ve);
			series.push_back(ts);

			curlen = 0;
			ve.clear();
			number_count++;
		}
		if (number_count==number)
			break;
	}
}
