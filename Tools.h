#ifndef _TOOLS_H_
#define _TOOLS_H_
#pragma once  

#include "../timeseries/TimeSeries.h"
#include "hilbert.h"

class TimeSeries;
class Element;

class Tools {
public:
    static void readTS(vector<TimeSeries> &T, int length_of_series, std::string FileName);
	static void writeTS(vector<TimeSeries> &T, std::string FileName);
	static void write_standard_TS(vector<TimeSeries> &T, std::string FileName);

    static void readQs(vector<TimeSeries> &T, int length_of_series, std::string FileName);
    static void readTS_k(vector<TimeSeries> &T, std::string FileName, int skip_k);
   

    static int GetJumpLength(int length, double stop, int num_of_ids);
    static int GetPAAListSize(int length, double stop, int num_of_ids);
	
	static void putIfLtMin(vector<Result>& candidate, Result c, int);

	static double getMin(vector<Result>& candidate, int topK);

	static void sortResult(vector<Result>& candidate);
};


#endif