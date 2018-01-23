
#include "timer.h"

Timer::Timer() {
    isStarted = false;
    isStoped = false;
}
bool Timer::start()
{
    if(!isStarted)
    {
        started = clock();
        isStarted = true;
        
        return true;
    }
    
    return false;
}

bool Timer::stop()
{
    if(isStarted && !isStoped)
    {
        stoped = clock();
        isStoped = true;
        
        return true;
    }
    
    return false;
}

float Timer::getTime()
{
    if (isStarted && isStoped)
        return (float)((stoped - started) / CLOCKS_PER_SEC);
    else if (isStarted)
        return (float)((clock() - started) / CLOCKS_PER_SEC);
    else
        return 0.0;
}

unsigned int Timer::getTimeMilliSec()
{
    if (isStarted && isStoped)
        return (unsigned int)((stoped - started) / (CLOCKS_PER_SEC / 1000));
    else if (isStarted)
        return (unsigned int)((clock() - started) / (CLOCKS_PER_SEC / 1000));
    else
        return 0;
}

void Timer::reset()
{
    isStarted = false;
    isStoped = false;
}

void Timer::restart()
{
    reset();
    start();
}