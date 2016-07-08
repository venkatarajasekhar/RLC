#include "DataGenerator.h"
#include "def.h"
#include "random.h"

double DataGenerator::avgVolatility = 0;
RandType DataGenerator::type = RW;

DataGenerator::DataGenerator(){
}

DataGenerator::~DataGenerator(){
}

double DataGenerator::RandomD(double min, double max)
{
	int r = rand();
	return min + (max-min)*static_cast<double>(r) / (static_cast<double>(RAND_MAX)); 
}

void DataGenerator::genAllTS(std::string filename,int num, int len){
	ofstream outfile(filename.c_str(),ios::binary);
	vector<double> ts;
	for(int i=0; i < num; i++){
		genTS(ts,len);
		outfile.write((char*)&*ts.begin(),sizeof(double)*len);
	}
	outfile.close();
}

void DataGenerator::genTS(vector<double> &ts,int len){
	ts.clear();
	ts.push_back(RandomD(-1.0,1.0));

	if (DataGenerator::type==AN) {
        // Arithmetic Normal: Today = Yesterday * (1 + NormalRandom(Mean = 0, StdDev = Volatilitydaily));
        boost::normal_distribution<double> gen(0, DataGenerator::avgVolatility);
        boost::mt19937 mt;
	    for(int i = 1; i < len; i++)
            ts.push_back(*ts.rbegin()*(1+gen(mt)));
    } else if (DataGenerator::type==LN) {
        // Log Normal: Today = Yesterday * Math.exp(NormalRandom(Mean = 0, StdDev = Volatilitydaily))
        boost::normal_distribution<double> gen(0, DataGenerator::avgVolatility);
        boost::mt19937 mt;
	    for(int i = 1; i < len; i++)
            ts.push_back(*ts.rbegin()*exp(gen(mt)));
    } else if (DataGenerator::type==AL) {
        // Arithmetic Levy: Today = Yesterday * (1 + GSLRandomLevy(c = Volatilitydaily / AdjustFactor, Alpha))
	    for(int i = 1; i < len; i++)
            ts.push_back(*ts.rbegin()*(1+GSLRandomLevy(avgVolatility, 1.7)));
    } else if (DataGenerator::type==LL) {
        // Log Levy: Today = Yesterday * Math.exp(GSLRandomLevy(c = Volatilitydaily / AdjustFactor, Alpha))
	    for(int i = 1; i < len; i++)
	            ts.push_back(*ts.rbegin()*exp(GSLRandomLevy(avgVolatility, 1.7)));
    } else if(DataGenerator::type==RW) {
		boost::normal_distribution<double> gen(0, DataGenerator::avgVolatility);
		boost::mt19937 mt;
		for(int i = 1; i < len; i++) {
			ts.push_back(*ts.rbegin() + gen(mt));
		}
	}
}

void DataGenerator::setType(RandType t) {
	type = t;
}
void DataGenerator::setAvgVolatility(double d) {
	avgVolatility = d;
}