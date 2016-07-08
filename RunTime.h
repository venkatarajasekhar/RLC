#ifndef _RUN_TIME_H_
#define _RUN_TIME_H_

class RunTime{
public:
	RunTime();
	~RunTime();

	void Start();
	void Stop();
	void ShowRunTime(int num_loop);
	double GetElapsedTime();

private:
	long nTimeStart;
	long nTimeStop;
	double totalElapsedTime;
	double lastElapsedTime;
};

#endif