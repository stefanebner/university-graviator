//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Timer
//	Basic Timer class
//	soon obsolete due to precision timer
//
//*********************************************************

#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class Timer
{
    private:
        clock_t started;
        clock_t stoped;
        bool isStarted;
        bool isStoped;
        
    public:
        Timer();
        bool start();
        bool stop();
        
        float getTime();
        unsigned int getTimeMilliSec();
        
        void reset();
        void restart();
};

#endif
