
#include "PrecisionTimer.h"

#include <iostream>
using namespace std;

PrecisionTimer::PrecisionTimer(double updaterate):
					mLastTime(0),
					mLasttimeInTimeElapsed(0),
					mCountFrequency(0),
					mStarted(false),
					mStartTime(0),
					mLastTimeElapsed(0.0),
					mSmoothUpdates(false),
					mUpdateRate(updaterate)

{
	QueryPerformanceFrequency( (LARGE_INTEGER*) &mCountFrequency),
	mTimeScale = 1.0/mCountFrequency;
}


void PrecisionTimer::start()
{
	mStartTime = true;
	mTimeElapsed = 0.0;

	QueryPerformanceCounter( (LARGE_INTEGER*) &mLastTime);

	mStartTime = mLasttimeInTimeElapsed = mLastTime;
	mNextTime = mLastTime + mUpdateRate / mTimeScale;
}


void PrecisionTimer::setUpdateRate(double updaterate)
{
    mUpdateRate = updaterate;
}


double PrecisionTimer::getTimeElapsed()
{
	mLastTimeElapsed = mTimeElapsed;

	QueryPerformanceCounter( (LARGE_INTEGER*) &mCurrentTime);
	mTimeElapsed = (mCurrentTime - mLasttimeInTimeElapsed) * mTimeScale;
	mLasttimeInTimeElapsed = mCurrentTime;

	const double smooth = 5.0;

	if(mSmoothUpdates)
	{
		if(mTimeElapsed < (mLastTimeElapsed * smooth))
		{
			return mTimeElapsed;
		}
		else
		{
			return 0.0;
		}
	}
	else
	{
		return mTimeElapsed;
	}
}

double PrecisionTimer::getCurrentTime()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &mCurrentTime);

	return (mCurrentTime - mStartTime) * mTimeScale;
}

double PrecisionTimer::getTimeTillUpdate()
{
    QueryPerformanceCounter( (LARGE_INTEGER*) &mCurrentTime);

    return (mNextTime - mCurrentTime) * mTimeScale;
}

bool PrecisionTimer::getStarted()
{
	return mStarted;
}

void PrecisionTimer::smoothUpdatedOn()
{
	mSmoothUpdates = true;
}

void PrecisionTimer::smoothUpdatedOff()
{
	mSmoothUpdates = false;
}

bool PrecisionTimer::readyForNextUpdate()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &mCurrentTime);

	if(mCurrentTime > mNextTime)
		return true;

	return false;
}

void PrecisionTimer::update()
{
    mTimeElapsed = (mCurrentTime - mLastTime) * mTimeScale;
    mLastTime = mCurrentTime;

    mNextTime = mCurrentTime + mUpdateRate / mTimeScale;
}
