//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	SimpleTypes
//	2 functions we need all over the project
//
//*********************************************************

#ifndef GRAVIATOR_SIMPLE_TYPES
#define GRAVIATOR_SIMPLE_TYPES

#include <pthread.h>
#include <string>
#include <sstream>



//pattern for safe mutex useage
class Synchronized{
public:
	Synchronized(pthread_mutex_t *mutex){
		this->mutex = mutex;
		pthread_mutex_lock(mutex);
	}
	~Synchronized() {
		pthread_mutex_unlock(mutex);
	}
private:
	pthread_mutex_t *mutex;
};


// template to convert a number to a string
template <class T>
inline std::string toString (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}


#endif