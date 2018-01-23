//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Player
//	manages Player
//
//*********************************************************

#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "ServerObject.h"
#include "../src/vec3f.h"
#include "LuaStateManager.h"
#include "../server/ServerEnergyManager.h"
#include "ServerObjectFactory.h"
#include "../PrecisionTimer.h"

#define LUA LuaStateManager::getInstance()

class ServerPlayer : public ServerObject
{
public:
	ServerPlayer();
	~ServerPlayer();
	void update();
	vec3f getForce(vec3f affectedLocation);
	vec3f getEnergy();

    bool canSteerLeft();
    bool canSteerRight();
    bool canShoot();

    void steerLeft(); 
    void steerRight();    
    void shoot();

	void resetEnergy();
	
	void increaseScore();
	unsigned int getScore();
	void resetScore();
	
	void die();
	bool isDead();
	bool shouldRespawn();
	float getRespawnTime();
	
private:
	ServerEnergyManager mServerEnergyManager;
	unsigned int mScore;
	PrecisionTimer *mResawnTimer;
    PrecisionTimer* mTimerShot;
    PrecisionTimer* mTimerSteerLeft;
    PrecisionTimer* mTimerSteerRight;
    
	
};
#endif