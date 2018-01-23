//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Gravitational Objects
//	all objects on the server such as shots and steering 
//	points, they are also automatically deleted after a
//	certain amount of time has passed
//
//*********************************************************

#ifndef SERVER_GRAVITATIONAL_OBJECT_H
#define SERVER_GRAVITATIONAL_OBJECT_H

#include "ServerObject.h"
#include "LuaStateManager.h"
#include "ServerObjectFactory.h"
#include "../src/vec3f.h"

#define LUA LuaStateManager::getInstance()

class ServerGravitationalObject : public ServerObject
{
public:
	ServerGravitationalObject()
	{
		mCreationTime = clock();
	}
	~ServerGravitationalObject(void);
	vec3f getForce(vec3f affectedLocation);
	void update();
	bool shouldBeKilled();


private:
	char mType;
	clock_t mCreationTime;
	clock_t mElapsedTime;
	clock_t mTimeToLive;
	
	
};
#endif