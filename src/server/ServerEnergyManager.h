//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	EnergyManager
//	each Player has an object of this class to regenerate 
//	and use his energy accordingly
//
//*********************************************************

#ifndef SERVER_ENERGY_MANAGER
#define SERVER_ENERGY_MANAGER

#include "LuaStateManager.h"
#include "../src/vec3f.h"
#include "../src/PrecisionTimer.h"

class ServerEnergyManager
{
public:
	ServerEnergyManager();
	~ServerEnergyManager(void);
	bool add(const char* type);
	bool check(const char* type);
	vec3f getStatus();
	void update(float distance);
	void reset();

private:
    void updateVariables();
	float mMaxEnergy;
	float mCurrentEnergy;
	float mShotEnergy;
	float mSteeringEnergy;
	float mRegenerationrate;
    PrecisionTimer* mTimerRegeneration;
};
#endif