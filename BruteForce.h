#ifndef _BRUTEFORCE_H_
#define _BRUTEFORCE_H_

#include "../utilities/def.h"
#include "../utilities/RunTime.h"
#include "../timeseries/TimeSeries.h"


vector<Result> SOTA(vector<TimeSeries> &T, TimeSeries &query, int topk, int); 
vector<Result> FixedSolutionWithLag(vector<TimeSeries> &T, TimeSeries &query, int topk, int); 
vector<Result> Prog_RLC(vector<TimeSeries> &T, TimeSeries &query, int topk, int); 
vector<Result> Appro_RLC(vector<TimeSeries> &T, TimeSeries &query, int topk, int); 
#endif