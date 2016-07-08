#include <stdio.h>
#include <time.h>
#include "RunTime.h"

RunTime::RunTime(){
	this->nTimeStart=0;
	this->nTimeStop=0;
	this->totalElapsedTime=0;
	this->lastElapsedTime=0;
}

RunTime::~RunTime(){
}

void RunTime::Start(){
	this->nTimeStart=clock();
	this->lastElapsedTime=this->nTimeStart;
}

void RunTime::Stop(){
	this->nTimeStop=clock();
	this->totalElapsedTime=static_cast<double>(nTimeStop-nTimeStart)/CLOCKS_PER_SEC;
}

void RunTime::ShowRunTime(int num_loop){
	Stop();
	printf("The total cost of time is: %f second\n",totalElapsedTime/num_loop);
}

double RunTime::GetElapsedTime(){
	nTimeStop=clock();
	double elapse=static_cast<double>(nTimeStop-lastElapsedTime)/CLOCKS_PER_SEC;
	lastElapsedTime=nTimeStop;
	return elapse;
}