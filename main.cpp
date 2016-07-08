

#include "stdafx.h"

#include "../utilities/DataGenerator.h"
#include "../methods/BruteForce.h"
//#include "../methods/TmbrAlgorithm.h"
#include "../utilities/Tools.h"
#include "../utilities/SeriesCorr.h"
#include "../utilities/DataLoader.h"

string osBinaryFile = "D:\\vc++\\LCS_LAG\\data\\object_timeseries_bin";
string qsBinaryFile = "D:\\vc++\\LCS_LAG\\data\\query_timeseries_bin";

string osTxtFile = "D:\\vc++\\LCS_LAG\\data\\object_timeseries.txt";
string qsTxtFile = "D:\\vc++\\LCS_LAG\\data\\query_timeseries.txt";

string realQsBinaryFile = "D:\\vc++\\LCS_LAG\\real_data\\4000_100_1037_10.qdt.txt";
string realOsBinaryFile = "D:\\vc++\\LCS_LAG\\real_data\\4000_1037_10.dt.txt";

int length_of_series = 1037;
int subseries_len_min  = 63;
vector<TimeSeries> oss;
vector<TimeSeries> qss;

int num_of_series = 10;
int top_k = 3;
int num_loop =1;

void loadTS()
{
	loadTimeSeries(oss, length_of_series, realOsBinaryFile,num_of_series);
	loadTimeSeries(qss, length_of_series, realQsBinaryFile,1);
}

void genTS()
{
	DataGenerator::genAllTS(osBinaryFile.c_str(), num_of_series, length_of_series);
	DataGenerator::genAllTS(qsBinaryFile.c_str(), 1, length_of_series);
}

void readTS()
{
	oss.resize(num_of_series);
	qss.resize(1);
	Tools::readTS(oss, length_of_series, osBinaryFile);
	Tools::readQs(qss, length_of_series, qsBinaryFile);
}

void saveTSInfo()
{
	Tools::writeTS(oss, osTxtFile);
	Tools::writeTS(qss, qsTxtFile);
}

void show_details(TimeSeries &query, vector<Result> &result)
{
	for (int i=0; i<result.size(); i++)
	{
		Result re = result[i];
			
		printf("query subseries: ");
		for (int j=re.m_os; j<re.m_os+re.m_len; j++)
		{
			printf("%e ",query.x[j]);
		}
		printf("\n");

		printf("object subseries: ");
		for (int j=re.m_object_os; j<re.m_object_os+re.m_object_len; j++)
		{
			printf("%e ", oss[re.m_id].x[j]);
		}
		printf("\n");
			
		printf("query subseries after normalization: ");
		vector<double> temp = standardize(query, re.m_os, re.m_len);
		for (int j=0; j<temp.size(); j++)
		{
			printf("%e ", temp[j]);
		}
		printf("\n");

		printf("object subseries after normalization: ");
		temp = standardize(oss[re.m_id], re.m_object_os, re.m_object_len);
		for (int j=0; j<temp.size(); j++)
		{
			printf("%e ", temp[j]);
		}
		printf("\n");
		printf("\n");
	}
}

void baseline_query()
{
	vector<Element> QueryResult;
    vector<TimeSeries>::iterator it;
	RunTime rt; rt.Start();

		
	  for(int i1=1; i1<=num_loop; i1++){
		vector<Result> result = SOTA(oss,*qss.begin(),top_k, subseries_len_min);
		printf("SOTA£º\n");
		printf("que_os  que_len  obj_id  obj_os  obj_len  correlation  window  lag\n");
		for (vector<Result>::iterator ite = result.begin(); ite != result.end(); ite++)
		{
			printf("%-6d  %-7d  %-6d  %-6d  %-7d  %-11g  %-6d  %d\n", ite->m_os, ite->m_len, ite->m_id, ite->m_object_os, ite->m_object_len, ite->m_corr, ite->m_wlen, ite->m_maxlag);
		}
	  }
		rt.ShowRunTime(num_loop);
		printf("\n\n");


		vector<Element> QueryResult_1;
    vector<TimeSeries>::iterator it_1;
	  
		RunTime rt_1; rt_1.Start();
		for(int i2=1; i2<=num_loop; i2++){
		vector<Result> result_1 = Prog_RLC(oss,*qss.begin(),top_k, subseries_len_min);
		printf("Prog_RLC£º\n");
		printf("que_os  que_len  obj_id  obj_os  obj_len  correlation  window  lag\n");
		for (vector<Result>::iterator ite = result_1.begin(); ite != result_1.end(); ite++)
		{
			printf("%-6d  %-7d  %-6d  %-6d  %-7d  %-11g  %-6d  %d\n", ite->m_os, ite->m_len, ite->m_id, ite->m_object_os, ite->m_object_len, ite->m_corr, ite->m_wlen, ite->m_maxlag);
		}
	  }
		rt_1.ShowRunTime(num_loop);
		printf("\n\n");





	vector<Element> QueryResult_4;
    vector<TimeSeries>::iterator it_4;
		RunTime rt_4; rt_4.Start();
		for(int i3=1; i3<=num_loop; i3++){
		vector<Result> result_4 = Appro_RLC(oss,*qss.begin(),top_k, subseries_len_min);
		printf("Appro_RLC£º\n");
		printf("que_os  que_len  obj_id  obj_os  obj_len  correlation  window  lag\n");
		for (vector<Result>::iterator ite = result_4.begin(); ite != result_4.end(); ite++)
		{
			printf("%-6d  %-7d  %-6d  %-6d  %-7d  %-11g  %-6d  %d\n", ite->m_os, ite->m_len, ite->m_id, ite->m_object_os, ite->m_object_len, ite->m_corr, ite->m_wlen, ite->m_maxlag);
		}
		}
		rt_4.ShowRunTime(num_loop);
		printf("\n\n");




		vector<Element> QueryResult_5;
        vector<TimeSeries>::iterator it_5;

		RunTime rt_5; rt_5.Start();
		for(int i5=1; i5<=num_loop; i5++){
		vector<Result> result_5 = FixedSolutionWithLag(oss,*qss.begin(),top_k, subseries_len_min);
		printf("Fixed solution£º\n");
		printf("que_os  que_len  obj_id  obj_os  obj_len  correlation  window  lag\n");
		for (vector<Result>::iterator ite = result_5.begin(); ite != result_5.end(); ite++)
		{
			printf("%-6d  %-7d  %-6d  %-6d  %-7d  %-11g  %-6d  %d\n", ite->m_os, ite->m_len, ite->m_id, ite->m_object_os, ite->m_object_len, ite->m_corr, ite->m_wlen, ite->m_maxlag);
		}
		}
		rt_5.ShowRunTime(num_loop);
		printf("\n\n");
}



int _tmain(int argc, _TCHAR* argv[])
{
	loadTS();   // real data
	//genTS(); // synthetic data
	//readTS();  // synthetic data
	baseline_query();
}