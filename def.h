#ifndef DEF_H
#define DEF_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <queue>
#include <time.h>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;

/**********************************/
/* Global Definition
/**********************************/
#define MAX_FLOAT numeric_limits<float>::max()
#define MAX_INT numeric_limits<int>::max()
#define MAX_UNSIGNEDINT numeric_limits<unsigned int>::max()

#ifdef WIN32
#define drand48() (double)rand()/(double)RAND_MAX
//#define drand48() MTRand53()()
#endif



#define SRAND

#define EPSINON 0.0001



class Element
{
public:
    Element() {};
	Element( int id, double dist){
		this->id = id;
		this->dist = dist;
	}
    double dist;
    int id;

    bool operator<(const Element &input) const {
        return this->dist<input.dist;
    }

    bool operator>(const Element &input) const {
        return this->dist>input.dist;
    }
};

class CompareElement{
public:
    bool operator()(Element &E1,Element &E2) {
        if(E1.dist < E2.dist) 
            return true;
        else
            return false;
         
    }
};

inline bool compareElemDesc(const Element &a, const Element &b) { return a.dist>b.dist; };
inline bool compareElemAbosulteDesc(const Element &a, const Element &b) { return fabs(a.dist) > fabs(b.dist); };
inline bool compareElemInsc(const Element &a, const Element &b) { return a.dist<b.dist; };
inline bool compareElemIDs(const Element &a, const Element &b) { return a.id<b.id; };
inline bool compareElemEqual(const Element &a, const Element &b) { return a.id==b.id; };


class Result
{
	public:
	Result() {};
	Result(int id,int os, int len, int object_os, int object_len, double dist, int wlen) {
		m_id = id; m_os = os; m_len = len;
		m_object_os = object_os; m_object_len = object_len;
		m_corr = dist;
		m_wlen=wlen;
		m_maxlag =object_os - os;

	}; 

	void operator=(const Result &input){
		m_id = input.m_id;
		m_os =input.m_os;
		m_len = input.m_len;
		m_object_os =input.m_object_os;
		m_object_len = input.m_object_len;
		m_corr = input.m_corr;
		m_wlen = input.m_wlen;
	    m_maxlag = input.m_maxlag;

	};
	bool operator==(const Result &input) {
		if( m_id == input.m_id && m_os == input.m_os && m_len == input.m_len && 
			m_object_os == input.m_object_os && 
			m_object_len == input.m_object_len && 
			m_corr == input.m_corr) {
			return true;
		}
		else {
			return false;
		}
	}

	int m_id;          //  Result TimeSeries ID
	int m_os;          //  Result Subsequence Offset
	int m_len;         //  Result Subsequence Length
	
	double m_corr;

	int m_object_os;
	int m_object_len;
	int m_wlen;
	int m_maxlag;
};

inline bool compareResultAsc(const Result &a, const Result &b) { return a.m_corr < b.m_corr; };
inline bool compareResultDesc(const Result &a, const Result &b) { return a.m_corr > b.m_corr; };

class PartiResult {
public:
	PartiResult() {
		m_start_x = 0;
		m_start_y = 0;
		m_end_x = 0;
		m_end_y = 0;
		m_num_of_groups = 0;
	};
	~PartiResult() {};
	PartiResult(int start_x, int start_y, int end_x, int end_y, int num_of_groups) {
		m_start_x = start_x;
		m_start_y = start_y;
		m_end_x = end_x;
		m_end_y = end_y;
		m_num_of_groups = num_of_groups;
	};
	
	void init(int start_x, int start_y, int end_x, int end_y, int num_of_groups) {
		m_start_x = start_x;
		m_start_y = start_y;
		m_end_x = end_x;
		m_end_y = end_y;
		m_num_of_groups = num_of_groups;
	};

public:
	int m_start_x;
	int m_start_y;
	int m_end_x;
	int m_end_y;
	int m_num_of_groups;
};

class DPSubResult_2 {
public:
	DPSubResult_2() {
	};
	~DPSubResult_2() {};

	DPSubResult_2(int start_x, int start_y, int end_x, int end_y, int num_of_groups, long long pa) {
		m_start_x = start_x;
		m_start_y = start_y;
		m_end_x = end_x;
		m_end_y = end_y;
		m_num_of_groups = num_of_groups;
		m_p_ability = pa;
	};
	
	void init(int start_x, int start_y, int end_x, int end_y, int num_of_groups, long long pa) {
		m_start_x = start_x;
		m_start_y = start_y;
		m_end_x = end_x;
		m_end_y = end_y;
		m_num_of_groups = num_of_groups;
		m_p_ability = pa;
	};

public:
	int m_start_x;
	int m_start_y;
	int m_end_x;
	int m_end_y;
	int m_num_of_groups;
	long long m_p_ability;

	PartiResult L_result;
	PartiResult R_result;

	vector<Result> Group_Result;          // Result: ID,Offset,Length 
};

class DPResult
{
public:
	DPResult() {};
	~DPResult() {};

	long long checkDPSubResult(int start_x, int start_y, int end_x, int end_y,int num_of_groups) 
	{
		for(int i = 0; i < m_result.size(); i++) {
			if(m_result[i].m_start_x == start_x && m_result[i].m_start_y == start_y && m_result[i].m_end_x == end_x && m_result[i].m_end_y == end_y && m_result[i].m_num_of_groups == num_of_groups ) {
				return m_result[i].m_p_ability;
			}
		}
		return -2; 
	}

	void addDPSubResult(DPSubResult_2 &subResult) {
		m_result.push_back(subResult);
	}

	DPSubResult_2 getDPSubResult_2(int start_x, int start_y, int end_x, int end_y,int num_of_groups) {
		for(int i = 0; i < m_result.size(); i++) {
			if(m_result[i].m_start_x == start_x && m_result[i].m_start_y == start_y && m_result[i].m_end_x == end_x && m_result[i].m_end_y == end_y && m_result[i].m_num_of_groups == num_of_groups ) {
				return m_result[i];
			}
		}
		return DPSubResult_2(0,0,0,0,0,0);
	}
public:
	vector<DPSubResult_2> m_result;
};

#endif