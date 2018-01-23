#include "../server/ServerEnergyManager.h"

#define LUA LuaStateManager::getInstance()

ServerEnergyManager::ServerEnergyManager(void)
{
	mTimerRegeneration = new PrecisionTimer(0.025);
	mTimerRegeneration->start();
    updateVariables();
    mCurrentEnergy = mMaxEnergy;
}

ServerEnergyManager::~ServerEnergyManager(void)
{
    delete mTimerRegeneration;
}

bool ServerEnergyManager::add( const char* type )
{
    if (mShotEnergy <= mCurrentEnergy && type == "shotPoint")
    {

        mCurrentEnergy -= mShotEnergy;
        return true;
    } 
    else if (mSteeringEnergy <= mCurrentEnergy && type == "steeringPoint")
    {

        mCurrentEnergy -= mSteeringEnergy;
        return true;
    } 
    else
    {
        return false;
    }
}

bool ServerEnergyManager::check(const char* type)
{
    if (mShotEnergy <= mCurrentEnergy && type == "shotPoint")
        return true;
    else if (mSteeringEnergy <= mCurrentEnergy && type == "steeringPoint")
        return true;
    else
        return false;
}

vec3f ServerEnergyManager::getStatus()
{
	return vec3f(mCurrentEnergy, mMaxEnergy, mRegenerationrate);
}

void ServerEnergyManager::update(float distance)
{
	//live balancing
	updateVariables();

	if(mTimerRegeneration->readyForNextUpdate())
	{
        mTimerRegeneration->update();
        float distanceBonus = (mRegenerationrate / distance)
            * LUA.getFloat("killRange") * LUA.getFloat("energyBonusModificator") ;

        mCurrentEnergy +=
            (mRegenerationrate  + (distanceBonus * distanceBonus))
            * (float)mTimerRegeneration->getTimeElapsed();
		if (mCurrentEnergy >= mMaxEnergy)
		{
			mCurrentEnergy = mMaxEnergy;
		}
	}	
}

void ServerEnergyManager::reset()
{
    updateVariables();
    mCurrentEnergy = mMaxEnergy;
}

void ServerEnergyManager::updateVariables()
{
	mMaxEnergy = LUA.getFloat("energyMax");
	mShotEnergy = LUA.getFloat("energyShot");
	mSteeringEnergy = LUA.getFloat("energySteering");
	mRegenerationrate = LUA.getFloat("energyRegeneration");
    mTimerRegeneration->setUpdateRate(LUA.getFloat("energyRegenerationTime"));
    
}

