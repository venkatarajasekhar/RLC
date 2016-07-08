#include "BruteForce.h"
#include "../utilities/SeriesCorr.h"
#include <math.h>
#include "../utilities/Tools.h"



vector<Result> SOTA(vector<TimeSeries> &T, TimeSeries &query, int top_K, int subseries_len_min) {
    vector<Result> results;
	for(int t_id = 0; t_id < T.size(); t_id++) {
		for(int os = query.len-subseries_len_min; os>=0; os--){ 
			int end = query.len - 1;
			int winLen = end - os+1;
			int lag_max = winLen/2;
			for(int lag=0; lag<=lag_max; lag++){ 
				double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
				for(int i=os; i<=query.len - 1-lag; i++){
					sum_q += query.x[i];
				    sum_qq += query.x[i]*query.x[i];
					sum_o += T[t_id].x[i+lag];
				    sum_oo += T[t_id].x[i+lag]*T[t_id].x[i+lag];
				    sum_qo += query.x[i] * T[t_id].x[i+lag];
				}
				int cur_len = query.len - os -lag;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
				Tools::putIfLtMin(results, Result(t_id,os,cur_len,query.len-cur_len, cur_len, corre,query.len-os), top_K);
			}
		}

	}

	Tools::sortResult(results);
	return results;
}


//fixed solution
vector<Result> FixedSolutionWithLag(vector<TimeSeries> &T, TimeSeries &query, int top_K, int subseries_len_min) {
    vector<Result> results;
	for(int t_id = 0; t_id < T.size(); t_id++) {

			int os = query.len-subseries_len_min;
			int end = query.len - 1;
			int winLen = end - os+1;
			int lag_max = winLen/2;
			for(int lag=0; lag<=lag_max; lag++){ 
				double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
				for(int i=os; i<=query.len - 1-lag; i++ ){
					sum_q += query.x[i];
				    sum_qq += query.x[i]*query.x[i];
					sum_o += T[t_id].x[i+lag];
				    sum_oo += T[t_id].x[i+lag]*T[t_id].x[i+lag];
				    sum_qo += query.x[i] * T[t_id].x[i+lag];
				}
				int cur_len = query.len - os -lag;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);

				Tools::putIfLtMin(results, Result(t_id,os,cur_len,query.len-cur_len, cur_len, corre,query.len-os), top_K);
				
			}

	}

	Tools::sortResult(results);
	return results;
}



vector<Result> Prog_RLC(vector<TimeSeries> &T, TimeSeries &query, int top_K, int subseries_len_min) {
    vector<Result> results;
	for(int t_id = 0; t_id < T.size(); t_id++) {
	int startPosForQs = query.len-subseries_len_min;
	int end = query.len - 1;
	int winLen = end - startPosForQs + 1;
	int lag_max = winLen/2;
	
		for(int lag=0; lag<=lag_max; lag++){
			double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
			int w=startPosForQs;
			for(int i=w; i<=query.len - 1-lag; i++ ){  
						sum_q += query.x[i];
				        sum_qq += query.x[i]*query.x[i];
				        sum_o += T[t_id].x[i+lag];
				        sum_oo += T[t_id].x[i+lag]*T[t_id].x[i+lag];
				        sum_qo += query.x[i] * T[t_id].x[i+lag];
					}
					int cur_len = query.len -w -lag;
				    double q_mean = sum_q/cur_len;
				    double o_mean = sum_o/cur_len;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				    double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				    double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				    double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
				    Tools::putIfLtMin(results, Result(t_id,w,cur_len,w+lag, cur_len, corre,query.len-w), top_K);
            w=w-1;
			for(; w>=0; w--) {	

				sum_q += query.x[w];
				sum_qq += query.x[w]*query.x[w];
				sum_o += T[t_id].x[w+lag];
				sum_oo += T[t_id].x[w+lag]*T[t_id].x[w+lag];
				sum_qo += query.x[w] * T[t_id].x[w+lag];
				int cur_len = query.len - w-lag;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,w,cur_len,w+lag, cur_len, corre,query.len-w), top_K);
			}
		}

	startPosForQs--;

	for(int os_sec=startPosForQs; os_sec>=0; os_sec--)
	{

	int winLen_1 = query.len -os_sec;
	int lag_max_1 = winLen_1/2;
	double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
	for(int lag_1 = lag_max+1; lag_1<=lag_max_1; lag_1++){
		
		for(int i=os_sec; i<=query.len - 1-lag_1; i++ ){
			sum_q += query.x[i];
			sum_qq += query.x[i]*query.x[i];
			sum_o += T[t_id].x[i+lag_1];
			sum_oo += T[t_id].x[i+lag_1]*T[t_id].x[i+lag_1];
			sum_qo += query.x[i] * T[t_id].x[i+lag_1];

		}
		int cur_len = winLen_1 -lag_1;
	    double q_mean = sum_q/cur_len;
		double o_mean = sum_o/cur_len;
		double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
		double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
		double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
        Tools::putIfLtMin(results, Result(t_id,os_sec,cur_len,os_sec+lag_1, cur_len, corre,query.len-os_sec), top_K);

	   lag_max+=1;

	   
		for(int os=os_sec-1; os>=0; os--){
			int winLen_2 = query.len -os;
			int lag_2 = (winLen_2)/2;
			if(lag_2 >= lag_max_1){
				sum_q += query.x[os];
				sum_qq += query.x[os]*query.x[os];
				sum_o += T[t_id].x[os+lag_1];
				sum_oo += T[t_id].x[os+lag_1]*T[t_id].x[os+lag_1];
				sum_qo += query.x[os] * T[t_id].x[os+lag_1];
				int cur_len = winLen_2-lag_1;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,os,cur_len,os+lag_1, cur_len, corre,query.len-os), top_K);

			}

		}


	}
	
	
	}
	}



	Tools::sortResult(results);
	return results;
}













vector<Result> Appro_RLC(vector<TimeSeries> &T, TimeSeries &query, int top_K, int subseries_len_min) {
	int omiga=10;
    vector<Result> results;
	for(int t_id = 0; t_id < T.size(); t_id++) {
	int startPosForQs = query.len-subseries_len_min;
	int end = query.len - 1;
	int winLen = end - startPosForQs + 1;
	int lag_max = winLen/2;
	int interp=0;
	int lag=0;
		for(; lag<=lag_max; ){ 
			
			double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
			for(int w=startPosForQs; w>=0; w--) {
				if(w==startPosForQs){
					for(int i=w; i<=query.len - 1-lag; i++ ){
						sum_q += query.x[i];
				        sum_qq += query.x[i]*query.x[i];
				        sum_o += T[t_id].x[i+lag];
				        sum_oo += T[t_id].x[i+lag]*T[t_id].x[i+lag];
				        sum_qo += query.x[i] * T[t_id].x[i+lag];
					}
					int cur_len = query.len -w -lag;
				    double q_mean = sum_q/cur_len;
				    double o_mean = sum_o/cur_len;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				    double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				    double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				    double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
				    Tools::putIfLtMin(results, Result(t_id,w,cur_len,w+lag, cur_len, corre,query.len-w), top_K);
				}
			   else{  

			
				sum_q += query.x[w];
				sum_qq += query.x[w]*query.x[w];
				sum_o += T[t_id].x[w+lag];
				sum_oo += T[t_id].x[w+lag]*T[t_id].x[w+lag];
				sum_qo += query.x[w] * T[t_id].x[w+lag];
				int cur_len = query.len - w-lag;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,w,cur_len,w+lag, cur_len, corre,query.len-w), top_K);
    
			}

			}

			lag=pow(double(2),interp);
			interp++;
			
		}


		
		lag=pow(double(2),interp-1);
		if(lag_max<lag){
			double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
			for(int i=startPosForQs; i<=query.len - 1-lag_max; i++ ){
						sum_q += query.x[i];
				        sum_qq += query.x[i]*query.x[i];
				        sum_o += T[t_id].x[i+lag_max];
				        sum_oo += T[t_id].x[i+lag_max]*T[t_id].x[i+lag_max];
				        sum_qo += query.x[i] * T[t_id].x[i+lag_max];
					}
					int cur_len = query.len -startPosForQs -lag_max;
				    double q_mean = sum_q/cur_len;
				    double o_mean = sum_o/cur_len;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				    double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				    double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				    double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
				    Tools::putIfLtMin(results, Result(t_id,startPosForQs,cur_len,startPosForQs+lag_max, cur_len, corre,query.len-startPosForQs), top_K);

			for(int os=startPosForQs-1; os>=0; os--){

				sum_q += query.x[os];
				sum_qq += query.x[os]*query.x[os];
				sum_o += T[t_id].x[os+lag_max];
				sum_oo += T[t_id].x[os+lag_max]*T[t_id].x[os+lag_max];
				sum_qo += query.x[os] * T[t_id].x[os+lag_max];
				int cur_len = query.len -os -lag_max;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,os,cur_len,os+lag_max, cur_len, corre,query.len-os), top_K);



		}
		
		}



	startPosForQs--;
	
	for(int os_sec=startPosForQs; os_sec>=0; os_sec--)
	{

	int winLen_1 = query.len -os_sec;
	int lag_max_1 = winLen_1/2;
	double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
	
	int lag_1=pow(double(2),interp-1);
	if(lag_max_1 >= lag_1 ){
		
		for(int i=os_sec; i<=query.len - 1-lag_1; i++ ){
			sum_q += query.x[i];
			sum_qq += query.x[i]*query.x[i];
			sum_o += T[t_id].x[i+lag_1];
			sum_oo += T[t_id].x[i+lag_1]*T[t_id].x[i+lag_1];
			sum_qo += query.x[i] * T[t_id].x[i+lag_1];

		}
		int cur_len = winLen_1 -lag_1;
	    double q_mean = sum_q/cur_len;
		double o_mean = sum_o/cur_len;
		double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
		double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
		double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
        Tools::putIfLtMin(results, Result(t_id,os_sec,cur_len,os_sec+lag_1, cur_len, corre,query.len-os_sec), top_K);


	   
		for(int os=os_sec-1; os>=0; os--){
			int winLen_2 = query.len -os;
			int lag_2 = (winLen_2)/2;
			if(lag_2 >= lag_max_1){
				sum_q += query.x[os];
				sum_qq += query.x[os]*query.x[os];
				sum_o += T[t_id].x[os+lag_1];
				sum_oo += T[t_id].x[os+lag_1]*T[t_id].x[os+lag_1];
				sum_qo += query.x[os] * T[t_id].x[os+lag_1];
				int cur_len = winLen_2-lag_1;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,os,cur_len,os+lag_1, cur_len, corre,query.len-os), top_K);

			}

		}
    interp++;
    lag_1=pow(double(2),interp);
	}


			
	if(lag_max_1>lag_max){
			double sum_q=0.0, sum_o=0.0, sum_qq=0.0, sum_oo=0.0, sum_qo=0.0;
			for(int i=os_sec; i<=query.len - 1-lag_max_1; i++ ){
						sum_q += query.x[i];
				        sum_qq += query.x[i]*query.x[i];
				        sum_o += T[t_id].x[i+lag_max_1];
				        sum_oo += T[t_id].x[i+lag_max_1]*T[t_id].x[i+lag_max_1];
				        sum_qo += query.x[i] * T[t_id].x[i+lag_max_1];
					}
					int cur_len = query.len -os_sec -lag_max_1;
				    double q_mean = sum_q/cur_len;
				    double o_mean = sum_o/cur_len;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				    double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				    double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				    double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
				    Tools::putIfLtMin(results, Result(t_id,os_sec,cur_len,os_sec+lag_max_1, cur_len, corre,query.len-os_sec), top_K);

			for(int os=os_sec-1; os>=0; os--){

				sum_q += query.x[os];
				sum_qq += query.x[os]*query.x[os];
				sum_o += T[t_id].x[os+lag_max_1];
				sum_oo += T[t_id].x[os+lag_max_1]*T[t_id].x[os+lag_max_1];
				sum_qo += query.x[os] * T[t_id].x[os+lag_max_1];
				int cur_len = query.len -os -lag_max_1;
				double q_mean = sum_q/cur_len;
				double o_mean = sum_o/cur_len;
				double q_stdv = sqrt(sum_qq/(cur_len) - (sum_q/cur_len)*(sum_q/cur_len));
				double o_stdv = sqrt(sum_oo/(cur_len) - (sum_o/cur_len)*(sum_o/cur_len));
				double corre = (sum_qo - cur_len*q_mean*o_mean) / (cur_len*q_stdv*o_stdv);
                Tools::putIfLtMin(results, Result(t_id,os,cur_len,os+lag_max_1, cur_len, corre,query.len-os), top_K);



		}
			lag_max=lag_max_1+omiga;
		}
	
	
	}
	}



	Tools::sortResult(results);
	return results;
}