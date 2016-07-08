#ifndef _TIME_SERIES_H
#define _TIME_SERIES_H

#include "../utilities/def.h"

class InnerGroupPAA {
public:
    InnerGroupPAA() {};
    ~InnerGroupPAA() {};

	void Copy(const InnerGroupPAA &input) {
		int dim = paa_max.size();
		for(int i = 0; i < dim; i++) {
			this->paa_max[i] = input.paa_max[i];
			this->paa_min[i] = input.paa_min[i];
		}
	}

	double Corr(const InnerGroupPAA &input, int winLen) {
		double dist = EuclideanDist(input);
		//dist = dist*dist;
		double corr = 1 - dist/(2.0*winLen);
		return corr;
	}

    double EuclideanDist(const InnerGroupPAA &input) {
		int dim = paa_max.size();
		double dist = 0.0;
		for( int i = 0; i < dim; i++ ) {
			if( input.paa_min[i] > this->paa_max[i] ) {
				dist += pow(input.paa_min[i]-this->paa_max[i],2);
			}
			if( input.paa_max[i] < this->paa_min[i] ) {
				dist += pow(input.paa_max[i]-this->paa_min[i],2);
			}
		} 
		//return sqrt(dist);
		return dist;
	};

    double MaxDist(const InnerGroupPAA &input) {
        int dim = paa_max.size();
        double r, dist=0.0;
        for(int i = 0; i < dim; i++) {
            r=0;
            if( fabs(input.paa_max[i] - this->paa_min[i] ) > fabs(this->paa_max[i]-input.paa_min[i]) ) {
                r = fabs(input.paa_max[i] - this->paa_min[i]);
            }else {
                r =  fabs(this->paa_max[i]-input.paa_min[i]);
            }
            dist += r*r;
        }        
        return sqrt(dist);
    };

	void InterSection(InnerGroupPAA &input_1, InnerGroupPAA &input_2) {
		for(int i = 0; i < input_2.paa_max.size(); i++) {
			if(input_1.paa_max[i] < input_2.paa_max[i]) {
				this->paa_max[i] = input_1.paa_max[i];
			} else {
				this->paa_max[i] = input_2.paa_max[i];
			}

			if(input_1.paa_min[i] > input_2.paa_min[i]) {
				this->paa_min[i] = input_1.paa_min[i];
			} else {
				this->paa_min[i] = input_2.paa_min[i];
			}
		}
	};

	void UnionSection(InnerGroupPAA &input_1, InnerGroupPAA &input_2) {
		for(int i = 0; i < input_2.paa_max.size(); i++) {
			if(input_1.paa_max[i] < input_2.paa_max[i]) {
				this->paa_max[i] = input_2.paa_max[i];
			} else {
				this->paa_max[i] = input_1.paa_max[i];
			}

			if(input_1.paa_min[i] > input_2.paa_min[i]) {
				this->paa_min[i] = input_2.paa_min[i];
			} else {
				this->paa_min[i] = input_1.paa_min[i];
			}
		}
	}

	void init(int dim){
		paa_max.clear();
		paa_min.clear();
		paa_max.resize(dim,-999999);
		paa_min.resize(dim,999999);
	};

        
public:
    vector<double> paa_max;
    vector<double> paa_min;
};

class Statis {
public:
    Statis() {};
    ~Statis() {};
    Statis(int i_os, int i_skip_k, int i_use_count) {
        this->os = i_os*i_skip_k;
        this->skip_k = i_skip_k;
        this->use_count = i_use_count;
    };

public:
    int os;
    int skip_k;
    int use_count;
    vector<double> sumX;
    vector<double> sumX2;
};

class StatisCache {
public:
    StatisCache() {}; 
    ~StatisCache() {};

    bool getCacheStatis(int os, double &sumX, double &sumXY);
    void updateCacheStatis(Statis statis);
    void reset();

public:
    int capacity;
    vector<Statis> cache;
};

class TimeSeries{
public:
	TimeSeries();
	~TimeSeries();

	void init(vector<double> &temp);
    void init_1(vector<double> &temp);

	void normalizePAA(int os, int len, int dim);

    double Diff_normalizedPAA(int os, int len, int dim);
    
	void innerPAA(int r_start, int r_end, int a_start, int a_end, vector<double> &paa);

	void computeX();
    
	double mean(int os,int len);
	double stdv(int os,int len);

    // skip k
    void init_k(vector<double> &temp, int skip_k);
    void computeX_k(int skip_k);
    double mean_k(int os, int len, int skip_k);
    double stdv_k(int os, int len, int skip_k);

    // skip k with cache
    void get_statis_x_k_cache(int os, int &skip_k, double &sumX, double &sumX2,int &access_count);

    void mean_stdv_k_cache(int os, int len, int &skip_k, double &sumX, double &sumX2, int &access_count);

	
    double NeuclideanDistEarlyTerminationWithLag(vector<Element> &sub_query,int len, double range);

   //左右端都不固定，带lag
	void buildInnerGroupPaaAsQuery(int len_min, int w, int paa_size);
	void InnerGroupPaaAsQuery(int start, int w,  InnerGroupPAA &temp);

	//右端固定, 不带lag
	void buildInnerGroupPaaAsObject(int len_min, int w, int paa_size);
	void InnerGroupPaaAsObject(int start, int w, InnerGroupPAA &temp);

    //double 

	void writeCSV(std::string filename);
	void readCSV(std::string filename);
	
	void TimeSeries::addMapping(int id, int start, int);

	
	//double std_of_sequence();
public:
	int len;
	int paa_size;
	int paa_count;
	vector<double> x;
	vector<double> paa_x;
	vector<double> sumX;
	vector<double> sumX2;   
	vector<InnerGroupPAA> innerGroupPaaAsQuery;
	vector<InnerGroupPAA> innerGroupPaaAsObject;
	vector<int> innerGroupPAA_OFFSET;

    
	// For Index Optimization
    vector< vector<InnerGroupPAA> > innerG;
	vector< vector<int> > innerG_Len;  

	map<int, int> idPosMappings;
	map<int, int> idWMappings;

	InnerGroupPAA temp_paa;
	double stop;

    // stais
    int statis_len;
    int statis_os;
    double statis_start_x;
    double statis_start_x2;
    double statis_end_x;
    double statis_end_x2;
    // cache
    StatisCache statiscache;

	//path
	string osBinaryFile;
	string qsBinaryFile;
	string osTxtFile;
	string qsTxtFile;
	string realQsBinaryFile;
	string realOsBinaryFile;
};

#endif