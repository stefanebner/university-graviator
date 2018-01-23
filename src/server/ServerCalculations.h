//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Calculations
//  contains calculations and objects of the game logic
//
//*********************************************************

#ifndef SERVER_CALCULATIONS_H
#define SERVER_CALCULATIONS_H


#include "ServerObjectFactory.h"
#include "ServerPlayer.h"
#include "ServerGravitationalObject.h"
#include "LuaStateManager.h"
#include "../GraviatorSimpleTypes.h"
#include "../src/vec3f.h"
#include "../src/Timer.h"
#include "../PrecisionTimer.h"

#include <map>
#include <deque>
#include <set>

using namespace std;

typedef ServerObjectFactory<ServerPlayer> PlayerFactory;
typedef ServerObjectFactory<ServerGravitationalObject> ObjectFactory;

class ServerCalculations
{
public:
	ServerCalculations(void);
	~ServerCalculations(void);
	void runCalculations();
	
	unsigned int createPlayer();
	void spawnPlayer(unsigned int id);
	void deletePlayer(unsigned int id);
	void handleInput(vec3f camera, bool shooting, bool left, bool right, unsigned int playerId);
	void deleteObject(unsigned int id);
	
	unsigned int getScoreLimit();
	unsigned int getNextHighestScore(unsigned int playerId);

    // ServerNetworkHandler needs full access to this 2 maps
	map<unsigned int, ServerGravitationalObject*> mCurrentObjects;
	map<unsigned int, ServerPlayer*> mCurrentPlayers;
	
private:
	unsigned int getNextUsableId();
	void createObject(const char* type, vec3f position);
	vec3f getNextUsablePosition();
	vec3f getDefaultVelocity( vec3f position );
    bool doesPlayerCollideWithArenaBounds(vec3f position);
    void recalcVelocityAndPositionAtArenaBounds(vec3f &velocity, vec3f &position);
    vec3f getPointOfImpact(vec3f position, vec3f orientation);

	deque<unsigned int> mNextIdToUse;
	int mIdCounter;
	float mStartTimer;
	Timer mTimer;
	unsigned int mScoreLimit;
	
    float mKillRange;
    float mSteeringPointDistance;
    float mFriction;
};



#endif