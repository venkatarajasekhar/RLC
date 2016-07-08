#include "Tools.h"
#include <algorithm>


void Tools::readTS(vector<TimeSeries> &TS, int length_of_series, std::string FileName){
	ifstream data(FileName.c_str(),ios::binary);
	vector<double> ts(length_of_series,0.0);
	vector<TimeSeries>::iterator it;
	for( it = TS.begin(); it != TS.end(); it++){
		data.read((char*)&ts[0],sizeof(double)*length_of_series);
		it->init_1(ts);
	}
	data.close();
}

void Tools::writeTS(vector<TimeSeries> &TS, std::string FileName){
	ofstream data(FileName.c_str(),ios::binary);
	vector<TimeSeries>::iterator it;
	for( it = TS.begin(); it != TS.end(); it++){
		for (int i = 0; i < it->len; i++)
		{
			data<<it->x[i]<<' ';
		}
		data<<std::endl<<std::endl;
	}
	data.close();
}

void Tools::write_standard_TS(vector<TimeSeries> &TS, std::string FileName) {
	ofstream data(FileName.c_str(),ios::binary);
	vector<TimeSeries>::iterator it;
	for( it = TS.begin(); it != TS.end(); it++){
		for (int i = 0; i < it->len; i++)
		{
			double standard_x = (it->x[i]/it->mean(0,it->len))/(it->stdv(0, it->len));
			data<<standard_x<<' ';
		}
		data<<std::endl<<std::endl;
	}
	data.close();
}

void Tools::readQs(vector<TimeSeries> &TS, int length_of_series, std::string FileName) {
    ifstream data(FileName.c_str(), ios::binary);
    vector<double> ts(length_of_series,0.0);
    vector<TimeSeries>::iterator it;
    for(it = TS.begin(); it != TS.end(); it++) {
       data.read((char*)&ts[0],sizeof(double)*length_of_series);
       it->init_1(ts);
    }
    data.close();
}



int Tools::GetJumpLength(int length, double stop, int num_of_ids) {
    int jump;
    int length_of_jump_range = ceil(double(length)*double(1.0-stop));//-int(double(length)*double(stop-1.0));
    vector<int> candidate;
    for(int jump = 2; jump < length_of_jump_range; jump++) {
        //
        int num_of_ids_using_jump = 0;
        for(int len = length; len > length*stop; len = len - jump) {
            int num_subsequence = length - len + 1;
            int num_reference = num_subsequence / jump + 1;
            num_of_ids_using_jump += num_reference;
        }
        candidate.push_back(num_of_ids_using_jump);
    }

    int largest_diff_of_ids = 99999999;//std::numeric_limits<int>::max();
       
    jump = 2;
    for(int i = 0; i < candidate.size(); i++) {
        if( abs(candidate[i] - num_of_ids) < largest_diff_of_ids ) {
            largest_diff_of_ids = abs(candidate[i]-num_of_ids);
            jump=i+2;
        }
    }
    return jump;
}

void Tools::putIfLtMin(vector<Result>& candidate, Result c, int topK) {
	if (candidate.size()<topK) {
		candidate.push_back(c);
		return;
	}

	double min = 10000000;
	int minIdx = 0;
	int i = 0;
	while(i < candidate.size()) {
		if (candidate[i].m_corr < min) {
			min = candidate[i].m_corr;
			minIdx = i;
		}
		i++;
	}

	if (c.m_corr > min)
		candidate[minIdx] = c;
}

double Tools::getMin(vector<Result>& candidate, int topK) {
	if (candidate.size() < topK) {
		return -1000000;
	}
	double min = 1.00;
	for (int i=0; i<candidate.size(); i++) {
		if (candidate.at(i).m_corr < min)
			min = candidate.at(i).m_corr;
	}
	return min;
}

inline bool compareResultAbosulteDesc(const Result &a, const Result &b) { return fabs(a.m_corr) > fabs(b.m_corr); };
void Tools::sortResult(vector<Result>& candidate) {
	sort(candidate.begin(), candidate.end(), compareResultAbosulteDesc);
}