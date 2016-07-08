#ifndef _DATA_GENERATOR_H_
#define _DATA_GENERATOR_H_

#include <boost/random.hpp>
#include <vector>

// AN - Arithmetic Normal
// LN - Log Normal
enum RandType {AN, LN, AL, LL,RW};

class DataGenerator{
private:
	static double RandomD(double min, double max);
	static double avgVolatility;
	static RandType type;

public:
	DataGenerator();
	~DataGenerator();

	static void genTS(std::vector<double> &temp,int len);
	static void genAllTS(std::string filename, int num, int len);
	static void setType(RandType t);
	static void setAvgVolatility(double d);
};

#endif