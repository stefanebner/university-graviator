//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	PrecisionTimer
//	a more exact Timer than the old class we had
//	using LONGLONG and Queryperformancecounter
//
//*********************************************************

#ifndef PRECISION_TIMER_H
#define PRECISION_TIMER_H

#include <Windows.h>
#include <cassert>

class PrecisionTimer
{
public:
	PrecisionTimer(double updaterate);
	~PrecisionTimer(void) {}

	void start();
	void setUpdateRate(double updaterate);
	bool readyForNextUpdate();
	void update();
	double getTimeElapsed();
	double getCurrentTime();
	double getTimeTillUpdate();
	bool getStarted();
	void smoothUpdatedOn();
	void smoothUpdatedOff();

private:
	LONGLONG mCurrentTime, mLastTime, mLasttimeInTimeElapsed;
	LONGLONG mNextTime, mStartTime, mCountFrequency;
	double mUpdateRate;

	double mTimeElapsed;
	double mLastTimeElapsed;
	double mTimeScale;

	bool mStarted;
	bool mSmoothUpdates;


};

#endif