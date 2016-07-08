#include "../utilities/def.h"
#include "TimeSeries.h"

void StatisCache::updateCacheStatis(Statis statis) {
    if(capacity == 0) {
        return;
    }
    if(cache.size() < capacity) {
        cache.push_back(statis);
    }else {
        // delete the statis with least use recently
        int id = 0;
        int least_so_far = 99999;
        for(int i = 0; i < cache.size();i++) {
            if(cache[i].use_count < least_so_far) {
                least_so_far = cache[i].use_count;
                id = i;
            }
        }
        // replace
        *(cache.begin()+id)=statis;
       // printf("the cache has been update here\n");
    }
}

bool StatisCache::getCacheStatis(int os,double &sumX, double &sumX2) 
{
    bool isInCache = false;
    for(int i = 0; i < cache.size(); i++) {
        if(os >= cache[i].os && os  <= (cache[i].os + cache[i].skip_k - 1)) {
            isInCache = true;
            sumX=cache[i].sumX[os-cache[i].os];
            sumX2=cache[i].sumX2[os-cache[i].os];
            cache[i].use_count++;
        }
    }
    return isInCache;
}

void StatisCache::reset() {
    this->cache.clear();
}

TimeSeries::TimeSeries(){
	this->len=0;
	this->paa_size=0;
	this->paa_count=0;
	osBinaryFile = "D:\\vc++\\LCS_LAG\\data\\object_timeseries_bin";
	qsBinaryFile = "D:\\vc++\\LCS_LAG\\data\\query_timeseries_bin";
	osTxtFile = "D:\\vc++\\LCS_LAG\\data\\object_timeseries.txt";
	qsTxtFile = "D:\\vc++\\LCS_LAG\\data\\query_timeseries.txt";

	realQsBinaryFile = "D:\\vc++\\LCS_LAG\\real_data\\test1.dat";
	realOsBinaryFile = "D:\\vc++\\LCS_LAG\\real_data\\test2.dat";
}

TimeSeries::~TimeSeries(){

}

void TimeSeries::init(vector<double> &temp) {
    this->x.clear();
    this->x.insert(this->x.end(),temp.begin(),temp.end());
    this->len = this->x.size();
    this->statis_len = -1;
}

void TimeSeries::init_1(vector<double> &temp){
	this->x.clear();
	this->x.insert(this->x.end(),temp.begin(),temp.end());
	this->len = this->x.size();
	this->sumX.resize(len);
	this->sumX2.resize(len);

    // 
    this->statis_len = -1;

	computeX();
}

void TimeSeries::computeX(){
	sumX[0] = x[0];
	sumX2[0] = x[0]*x[0];
	for( int i = 1; i < len; i++ ){
		sumX[i] = x[i] + sumX[i-1];
		sumX2[i] = x[i]*x[i] + sumX2[i-1];
	}
}

double TimeSeries::mean(int os, int len){
	if( len == 0 ){
		return 0.0f;
	}
	return (sumX[os+len-1] - sumX[os] + x[os])/static_cast<double>(len); 
}

double TimeSeries::stdv(int os, int len){
	double mean = this->mean(os,len);
	double stdv2 = (sumX2[os+len-1] - sumX2[os] + x[os]*x[os])/len - mean*mean;
	if(stdv2 <= 0)
		return 0;
	else
		return sqrt(stdv2);
}

// skip_k
void TimeSeries::init_k(vector<double> &temp, int skip_k) {
    this->x.clear();
    this->x.insert(this->x.end(),temp.begin(),temp.end());
    this->len = this->x.size();
    int len_statis = (this->len + skip_k - 1)/skip_k;
    this->sumX.resize(len_statis);
    this->sumX2.resize(len_statis);

    computeX_k(skip_k);
}

void TimeSeries::computeX_k(int skip_k) {
    sumX[0] = x[0];
    sumX2[0] = x[0]*x[0];
    int len_statis = this->sumX.size();
    for(int i = 1; i < len_statis; i++) {
        double temp_x = sumX[i-1];
        double temp_x2 = sumX2[i-1];
        for(int j = 1; j <= skip_k; j++ ) {
            temp_x += x[(i-1)*skip_k+j];
            temp_x2 += x[(i-1)*skip_k+j]*x[(i-1)*skip_k+j];
        }
        sumX[i] = temp_x;
        sumX2[i] = temp_x2;
    }
}

double TimeSeries::mean_k(int os, int len, int skip_k) {
    if(len == 0) {
        return 0.0f;
    }
    int r_start = os/skip_k;
    int r_end = (os + len - 1)/skip_k;
    double sum_start = this->sumX[r_start];
    for(int i = (r_start*skip_k)+1; i <= os; i++) {
        sum_start += this->x[i];
    }
    double sum_end = this->sumX[r_end];
    for(int i = (r_end*skip_k)+1; i <= (os+len-1); i++) {
        sum_end += this->x[i];
    }

    return (sum_end - sum_start + x[os])/static_cast<double>(len);
}

double  TimeSeries::stdv_k(int os, int len, int skip_k) {
    double mean = this->mean_k(os,len,skip_k);

    int r_start = os/skip_k;
    int r_end = (os + len - 1)/skip_k;
    double sum_start = this->sumX2[r_start];
    for(int i = (r_start*skip_k)+1; i <= os; i++) {
        sum_start += (this->x[i]*this->x[i]);
    }
    double sum_end = this->sumX2[r_end];
    for(int i = (r_end*skip_k)+1; i <= (os+len-1); i++) {
        sum_end += (this->x[i]*this->x[i]);
    }
    double stdv2 = (sum_end - sum_start + x[os]*x[os])/len - mean*mean;
    if(stdv2 <= 0) {
        return 0;
    }else {
        return sqrt(stdv2);
    }
    
}

// skip k with cache
void TimeSeries::get_statis_x_k_cache(int os, int &skip_k, double &sumX, double &sumX2,int &access_count) {
    if(statiscache.getCacheStatis(os,sumX,sumX2) == true) {
        return;
    }
    // get the statis and update the cache

    access_count++;

    int r_start = os/skip_k;
    Statis statis(r_start,skip_k,0);

    double t_sumX = this->sumX[r_start];
    double t_sumX2 = this->sumX2[r_start];
    statis.sumX.push_back(t_sumX);
    statis.sumX2.push_back(t_sumX2);

    for(int i = r_start*skip_k+1; i < (r_start+1)*skip_k; i++) {
        t_sumX += this->x[i];
        t_sumX2 += this->x[i]*this->x[i];
        statis.sumX.push_back(t_sumX);
        statis.sumX2.push_back(t_sumX2);
    }

    this->statiscache.updateCacheStatis(statis);

    sumX=statis.sumX[os-statis.os];
    sumX2=statis.sumX2[os-statis.os];
}
void TimeSeries::mean_stdv_k_cache(int os, int len, int &skip_k, double &mean, double &stdv, int &access_count) {
    int start = os;
    int end = os+len-1;
    double start_x, start_x2;
    get_statis_x_k_cache(os,skip_k,start_x,start_x2,access_count);
    double end_x, end_x2;
    get_statis_x_k_cache(os+len-1,skip_k,end_x,end_x2,access_count);

    mean = (end_x - start_x + x[os])/static_cast<double>(len);

    double stdv2 = (end_x2 - start_x2 + x[os]*x[os])/len - mean*mean;
    if(stdv2 <= 0) {
        stdv = 0.0;
    }else {
        stdv = sqrt(stdv2);
    }  
}

double TimeSeries::NeuclideanDistEarlyTerminationWithLag(vector<Element> &sub_query,int len, double range) {
    // need no cache here!

	double object_sum_x2 = 0, object_sum_x = 0;
	for(int i = this->len-1; i >= this->len-len; i--) {
        object_sum_x += this->x[i];
        object_sum_x2 += this->x[i]*this->x[i];
    }
    double mean = (object_sum_x) / len;
    double stdv = (object_sum_x2)/len - mean*mean;
	if(stdv <= 0.0) {
        return 999999.0;
    }
	else {
		stdv =sqrt(stdv);
	}

	// online normalized the value and calculate its Euclidean distance
    double dist_so_far = 0.0;
    for(int i = 0; i < sub_query.size(); i++) {
		int object_id = sub_query[i].id + this->len - len;
		double _ = (this->x[object_id]-mean)/stdv;
		double __ = sub_query[i].dist;
		dist_so_far += pow(_ - __,2);
        if(dist_so_far > range) {
            return 999999.0;
        }
    }
    return dist_so_far;
}

void TimeSeries::writeCSV(std::string filename){
	ofstream outfile(filename.c_str());
	for( int i = 0; i < len; i++){
		outfile << this->x[i] << endl;
	}
	outfile.close();
}

void TimeSeries::readCSV(std::string filename){
    ifstream infile(filename.c_str());
    vector<double> temp; double data;
    infile >> data;
    while(!infile.eof()){
        temp.push_back(data);
        infile >> data;
    }
    infile.close();
    init_1(temp);
}

double TimeSeries::Diff_normalizedPAA(int os, int len, int dim) {
    this->paa_count = dim;
    double diff = 0.0;
    if(os == 0) {
        vector<double> cur_paa;
        vector<double> r_cur_paa;
        this->innerPAA(os,os+len,os,os+len-1,cur_paa);
        this->innerPAA(os,os+len,os,os+len,r_cur_paa);
        for(int i = 0; i < dim; i++) {
            if(r_cur_paa[i] - cur_paa[i] >= 0) {
                diff += r_cur_paa[i] - cur_paa[i];
            }else {
                diff += cur_paa[i] - r_cur_paa[i];
            }
        }
        return diff;
    }
    if(os==this->len - len) {
        vector<double> cur_paa;
        vector<double> r_cur_paa;
        this->innerPAA(os-1,os+len-1,os,os+len-1,cur_paa);
        this->innerPAA(os-1,os+len-1,os-1,os+len-1,r_cur_paa);
        for(int i = 0; i < dim; i++) {
            if(r_cur_paa[i] - cur_paa[i] >= 0) {
                diff += r_cur_paa[i] - cur_paa[i];
            }else {
                diff += cur_paa[i] - r_cur_paa[i];
            }
        }
        return diff;
    }
    vector<double> l_cur_paa;
    vector<double> l_r__cur_paa;
    this->innerPAA(os,os+len,os,os+len-1,l_cur_paa);
    this->innerPAA(os,os+len,os,os+len,l_r__cur_paa);
    double l_diff = 0.0;
    for(int i = 0; i < dim; i++) {
          if(l_r__cur_paa[i] - l_cur_paa[i] >= 0) {
                l_diff += l_r__cur_paa[i] - l_cur_paa[i];
          }else {
                l_diff += l_cur_paa[i] - l_r__cur_paa[i];
          }
    }

    vector<double> r_cur_paa;
    vector<double> r_r__cur_paa;
    this->innerPAA(os-1,os+len-1,os,os+len-1,r_cur_paa);
    this->innerPAA(os-1,os+len-1,os-1,os+len-1,r_r__cur_paa);
    double r_diff = 0.0;
    for(int i = 0; i < dim; i++) {
          if(r_r__cur_paa[i] - r_cur_paa[i] >= 0) {
                r_diff += r_r__cur_paa[i] - r_cur_paa[i];
          }else {
                r_diff += r_cur_paa[i] - r_r__cur_paa[i];
          }
    }

    return (l_diff+r_diff)/2;
}

void TimeSeries::innerPAA(int r_start, int r_end, int a_start, int a_end, vector<double> &paa) {
	paa.resize(this->paa_size,0.0);
	int len = r_end - r_start + 1;
	int r_len = a_end - a_start + 1;
	double mean = this->mean(a_start,r_len);
	if( len % this->paa_size == 0 )
	{
		int unit = len / this->paa_size;
		double stdv = this->stdv(a_start,r_len)*sqrt((double)r_len/(double)len);
        if(stdv == 0.0) {
            for(int i = 0; i < this->paa_size; i++) {
                paa[i] = 0.0;
            }
            return;
        }
		for( int i = 0; i < this->paa_size; i++ )
		{
			int s = r_start + i*unit; int e = r_start + (i+1)*unit - 1;
			if( s >= a_start && e <= a_end )
			{
				double paa_mean = this->mean(s,unit);
				paa[i] = (paa_mean - mean)/stdv;
			}else if( s < a_start && e <= a_end && e > a_start)
			{
				double paa_mean = ( mean*(a_start-s) + this->mean(a_start,e-a_start+1)*(e-a_start+1) )/unit;
				paa[i] = (paa_mean - mean)/stdv;
			}else if( e > a_end && s >= a_start && s < a_end)
			{
				double paa_mean = ( mean*(e-a_end) + this->mean(s,a_end-s+1)*(a_end-s+1) )/unit;
				paa[i] = (paa_mean - mean)/stdv;
			}else if( e <= a_start || s >= a_end) 
			{
				//double paa_mean = 0;
				paa[i] = 0.0;
			}
		}
	}else
	{
		int unit = len / this->paa_size + 1;
		double total_len = unit*this->paa_size;
		double stdv = this->stdv(a_start,r_len)*sqrt((double)r_len/(double)total_len);
        if(stdv == 0.0) {
            for(int i = 0; i < this->paa_size; i++) {
                paa[i] = 0.0;
            }
            return;
        }

		for( int i = 0; i < this->paa_size; i++ )
		{
			int s = r_start + i*unit; int e = r_start + (i+1)*unit - 1;
			if( s >= a_start && e <= a_end )
			{
				double paa_mean = this->mean(s,unit);
				paa[i] = (paa_mean - mean)/stdv;
			}else if( s < a_start && e <= a_end && e >= a_start)
			{
				double paa_mean = ( mean*(a_start-s) + this->mean(a_start,e-a_start+1)*(e-a_start+1) )/unit;
				paa[i] = (paa_mean - mean)/stdv;
			}else if(e > a_end && s >= a_start && s <= a_end)
			{
				double paa_mean = ( mean*(e-a_end) + this->mean(s,a_end-s+1)*(a_end-s+1) )/unit;
				paa[i] = (paa_mean - mean)/stdv;
			}else {
				paa[i] = 0.0;
			}
		}
	}
}

void TimeSeries::buildInnerGroupPaaAsQuery(int len_min, int w, int paa_size) {
	this->paa_size = paa_size;
	int id = 0;
	int start = this->len - len_min - w + 1;
	while (start >= 0) {
		 InnerGroupPAA temp;
		 temp.init(paa_size);
		 this->InnerGroupPaaAsQuery(start, w, temp);
		 this->innerGroupPaaAsQuery.push_back(temp);
		 
		 addMapping(id, start, w);
		 id++;

		 if (start >= w) {
			 start -= w;
		 } else if (start > 0) {
			 w = start;
			 start = 0;
		 } else {
			 //break out!
			 start = -1;
		 }
	}
}

void TimeSeries::addMapping(int id, int start, int w) {
	if (idPosMappings.find(id) == idPosMappings.end()) {
		idPosMappings[id] = start;
		idWMappings[id] = w;
	}
}

//左右端都不固定，带lag
void TimeSeries::InnerGroupPaaAsQuery(int start, int w,  InnerGroupPAA &temp) {
	int start_min = start;
	int end_max = this->len - 1;
	for (int i = start; i<start+w; i++) {
		double len = this->len - i;
		int lag = len/2;
		for (; lag>=0; lag--) {
			vector<double> refer;
			int end = this->len-lag-1;
			this->innerPAA(start_min,end_max,i,end,refer);
			for(int j = 0; j < this->paa_size; j++) {
				if(refer[j] > temp.paa_max[j]) {
					temp.paa_max[j] = refer[j];
				}
				if(refer[j] < temp.paa_min[j]) {
					temp.paa_min[j] = refer[j];
				}
			}
		}
	}
}

void TimeSeries::buildInnerGroupPaaAsObject(int len_min, int w, int paa_size)
{
	this->paa_size = paa_size;
	int id = 0;
	int start = this->len - len_min - w + 1; 
	while (start >= 0) {
		 InnerGroupPAA temp;
		 temp.init(paa_size);
		 this->InnerGroupPaaAsObject(start, w, temp);
		 this->innerGroupPaaAsObject.push_back(temp);
		 addMapping(id, start, w);
		 id++;
		 this->paa_count++;

		 if (start >= w) {
			 start -= w;
		 } else if (start > 0) {
			 w = start;
			 start = 0;
		 } else {
			 //break out!
			 start = -1;
		 }
	}
}

//右端固定, 不带lag
void TimeSeries::InnerGroupPaaAsObject(int start, int w, InnerGroupPAA &temp)
{
	int start_min = start;
	int end_max = this->len-1;
	for (int i=start; i<start+w; i++)
	{
		for (int k=i; k<=(start+end_max)/2; k++)
		{
			vector<double> refer;
			int end = this->len-1;
			this->innerPAA(start_min,end_max,k,end,refer);
			for(int j=0; j<this->paa_size; j++) 
			{
				if(refer[j] > temp.paa_max[j]) 
				{
					temp.paa_max[j] = refer[j];
				}
				if(refer[j] < temp.paa_min[j]) 
				{
					temp.paa_min[j] = refer[j];
				}
			}
		}
	}
}