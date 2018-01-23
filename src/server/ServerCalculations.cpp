#include "ServerCalculations.h"
#include "..\Utility.h"

#define OBJECT ServerGravitationalObject
#define PLAYER ServerPlayer
#define OF ObjectFactory::getInstance()
#define PF PlayerFactory::getInstance()
#define LUA LuaStateManager::getInstance()

#define FORCE_MODIFIER 0.025

ServerCalculations::ServerCalculations(void)
{
	LUA.init("settings.lua");
	mIdCounter = 0;
	mKillRange = 7.0;
	mSteeringPointDistance = 10.0;
	mFriction = 0.01f;

	vec3f centerPosition(0, 0, 0);

	//this object wont change at all so we can create it on creation of the calculations class
	createObject("centerPoint", centerPosition);
	
	mTimer.start();
	mScoreLimit = 0;
	
}

ServerCalculations::~ServerCalculations(void)
{
}

void ServerCalculations::runCalculations()
{
    float timeDifference = mTimer.getTime();
    mTimer.reset();

    // live balancing via lua
    mKillRange = LUA.getFloat("killRange");
    mSteeringPointDistance = LUA.getFloat("steeringPointDistance");
    mFriction = LUA.getFloat("friction");

    map<unsigned int, OBJECT*>::iterator itObjects;
    map<unsigned int, PLAYER*>::iterator itPlayers;

    for (itPlayers = mCurrentPlayers.begin(); itPlayers != mCurrentPlayers.end(); itPlayers++)
    {
        itPlayers->second->update();
        
        if (itPlayers->second->isDead())
        {
            if (itPlayers->second->shouldRespawn())
                spawnPlayer(itPlayers->second->getID());
            else
                continue;
        }

        vec3f calculationForce, calculationPosition;

        if(USE_2D_CALCULATION)
        {
            calculationPosition = itPlayers->second->getPosition();
        }
        else
        {
            calculationPosition =
                transformToCalculationCoordinates(itPlayers->second->getPosition());
        }


        // check if player is to close to center
        if (calculationPosition.getLength() < mKillRange)
        {
            map<unsigned int, PLAYER*>::iterator itP;
            for (itP = mCurrentPlayers.begin(); itP != mCurrentPlayers.end(); itP++)
            {
                if (itP != itPlayers)
                {
                    itP->second->increaseScore();
                }
            }

            itPlayers->second->die();
            continue;
        }


        // calc force of gravitational objects
        // players are pulled towards them
        for (itObjects = mCurrentObjects.begin(); itObjects!=mCurrentObjects.end(); itObjects++)
        {
            calculationForce += itObjects->second->getForce(calculationPosition);

            // if currentObject is the center == 'c'
            // update value from lua file
            if(itObjects->second->getType() == 'c')
            {
                itObjects->second->setMass(LUA.getFloat("massCenter"));
            }
        }


        // calc force of other players
        // players are pushed away from them
        map<unsigned int, PLAYER*>::iterator itP;
        for (itP = mCurrentPlayers.begin(); itP != mCurrentPlayers.end(); itP++)
        {
            if (itP != itPlayers)
            {
                calculationForce -= itP->second->getForce(calculationPosition);
            }
        }

        vec3f calculationVelocity = itPlayers->second->getVelocity();
        calculationVelocity *= (1- mFriction);

        // scale the velocity the player is moving
        calculationVelocity += calculationForce * (float)FORCE_MODIFIER; 

        if(doesPlayerCollideWithArenaBounds(calculationPosition))
        {
            recalcVelocityAndPositionAtArenaBounds(calculationVelocity, calculationPosition);
        }
        else
        {
            calculationPosition += calculationVelocity;
        }

        if (USE_2D_CALCULATION)
            itPlayers->second->setPosition(calculationPosition);
        else
            itPlayers->second->setPosition(transformToRenderingCoordinates(calculationPosition));

        itPlayers->second->setVelocity(calculationVelocity);

        //live balancing via lua file
        itPlayers->second->setMass(LUA.getFloat("massPlayer"));
    }

}


unsigned int ServerCalculations::createPlayer()
{
    unsigned int currentId = getNextUsableId();
    mCurrentPlayers.insert( pair<unsigned int, PLAYER*>( currentId, PF.create("createPlayer") ) );

    map<unsigned int, PLAYER*>::iterator it;

    if (mCurrentPlayers.count(currentId)>0)
    {
        it = mCurrentPlayers.find(currentId);
        it->second->setID(currentId);
        //spawnPlayer(currentId);
    }

    return currentId;	
}




void ServerCalculations::spawnPlayer(unsigned int id)
{
    map<unsigned int, PLAYER*>::iterator it;
    
    if (mCurrentPlayers.count(id)>0)
    {
        it = mCurrentPlayers.find(id);
        it->second->setPosition(getNextUsablePosition());
        it->second->setVelocity(getDefaultVelocity(it->second->getPosition()));
        it->second->resetEnergy();
    }
}


void ServerCalculations::deletePlayer( unsigned int id )
{
    map<unsigned int, PLAYER*>::iterator it = mCurrentPlayers.find(id);
    
    if(it != mCurrentPlayers.end())
    {
        delete (PLAYER*)it->second;
        mCurrentPlayers.erase(it);
        mNextIdToUse.push_back(id);
    }
    else
    {
        printf("cant delete player with id: %i", id);
    }

}


void ServerCalculations::handleInput( vec3f playerOrientation, bool shooting, bool left, bool right, unsigned int playerId )
{
    vec3f playerPosition;

    map<unsigned int, PLAYER*>::iterator itPlayers = mCurrentPlayers.find(playerId);

    if (itPlayers != mCurrentPlayers.end())
    {
        if (itPlayers->second->isDead())
            return;
        
        itPlayers->second->setOrientation(playerOrientation);
        playerPosition = itPlayers->second->getPosition();

        
        if (left && itPlayers->second->canSteerLeft())
        {
            vec3f calculationPlayerPosition;

            if (USE_2D_CALCULATION)
                calculationPlayerPosition = playerPosition;
            else
                calculationPlayerPosition = transformToCalculationCoordinates(playerPosition);

            vec3f calculationLeftSteering(playerOrientation.z, 0, -playerOrientation.x);
            calculationLeftSteering *= mSteeringPointDistance;
            calculationLeftSteering += calculationPlayerPosition;

            if (USE_2D_CALCULATION)
            {
                createObject("steeringPoint", calculationLeftSteering);
            }
            else
            {
                createObject("steeringPoint",
                    transformToRenderingCoordinates(calculationLeftSteering));

            }

            itPlayers->second->steerLeft();
        }

        if (right && itPlayers->second->canSteerRight())
        {
            vec3f calculationPlayerPosition;

            if (USE_2D_CALCULATION)
                calculationPlayerPosition = playerPosition;
            else
                calculationPlayerPosition = transformToCalculationCoordinates(playerPosition);


            vec3f calculationRightSteering(-playerOrientation.z, 0, playerOrientation.x);
            calculationRightSteering *= mSteeringPointDistance;
            calculationRightSteering += calculationPlayerPosition;

            if (USE_2D_CALCULATION)
            {
                createObject("steeringPoint", calculationRightSteering);
            }
            else
            {
                createObject("steeringPoint",
                    transformToRenderingCoordinates(calculationRightSteering));
            }
            itPlayers->second->steerRight();
        }

        if (shooting && itPlayers->second->canShoot())
        {
            vec3f pointOfImpact;
            if (USE_2D_CALCULATION)
                pointOfImpact = getPointOfImpact(setYtoArenaHeight(playerPosition), playerOrientation);
            else
                pointOfImpact = getPointOfImpact(playerPosition, playerOrientation);

            // do not create shot outside the arena
            if (pointOfImpact.y < ARENA_RADIUS)
            {
                itPlayers->second->shoot();
                // if we use 2D calculations
                // set Y to 0 because it is 0 in all other points
                if (USE_2D_CALCULATION)
                    pointOfImpact.y = 0;

                createObject("shotPoint", pointOfImpact);
            }
        }
    }
    else
    {
        printf("cant find the player under this id: %i\n", playerId);
    }

}


void ServerCalculations::deleteObject( unsigned int id )
{
	map<unsigned int, ServerGravitationalObject*>::iterator it = mCurrentObjects.find(id);
	
	if(it != mCurrentObjects.end())
	{
		delete (OBJECT*)it->second;
		mCurrentObjects.erase(it);
		mNextIdToUse.push_back(id);
	}
	else
	{
		printf("cant delete object with id: %i", id);
	}
	
}

unsigned int ServerCalculations::getScoreLimit()
{
    return mScoreLimit;
}

unsigned int ServerCalculations::getNextHighestScore(unsigned int playerId)
{
    unsigned int maxScore = 0;
    map<unsigned int, PLAYER*>::iterator it;
    for (it = mCurrentPlayers.begin(); it != mCurrentPlayers.end(); it++)
    {
        if (it->first != playerId)
        {
            maxScore = max(it->second->getScore(), maxScore);
        }
    }
    return maxScore;
}

unsigned int ServerCalculations::getNextUsableId()
{
	unsigned int currentId;

	if (mNextIdToUse.empty())
	{
		currentId = mIdCounter;
		mIdCounter++;
		return currentId;
	}
	else
	{
		currentId = mNextIdToUse.front();
		mNextIdToUse.pop_front();
		return currentId;
	}
}

void ServerCalculations::createObject(const char* type, vec3f position)
{
    //assign the short type each gravitational object has - gType for gravitationalType
    char gType;
    if (type == "shotPoint") gType = 's'; 
    else if (type == "steeringPoint") gType = 'g';
    else gType = 'c';

    unsigned int currentId;

    currentId = getNextUsableId();

    mCurrentObjects.insert( pair<unsigned int, OBJECT*>( currentId, OF.create(type) ) );

    map<unsigned int, OBJECT*>::iterator it;

    if (mCurrentObjects.count(currentId)>0)
    {
        it = mCurrentObjects.find(currentId);
        it->second->setType(gType);
        it->second->setPosition(position);
        it->second->setID(currentId);
    }

}


vec3f ServerCalculations::getNextUsablePosition()
{
    vec3f position;
    
    map<unsigned int, PLAYER*>::iterator itPlayers;
    for (itPlayers = mCurrentPlayers.begin(); itPlayers != mCurrentPlayers.end(); itPlayers++)
    {
        position += itPlayers->second->getPosition();
    }
    
    if (position.isZero())
        position = vec3f((float)(rand()%3-1), 0, (float)(rand()%3-1));
    
    position = position * (-1);
    position.normalize();
    position *= (float)(ARENA_RADIUS / LUA.getFloat("startHeight"));
    
    if (USE_2D_CALCULATION)
        return position;
    else
        return setYtoArenaHeight(position);
    
}

vec3f ServerCalculations::getDefaultVelocity( vec3f position )
{
    vec3f normal(position.z, 0, -position.x);
    normal.normalize();
    
    normal *= LUA.getFloat("startVelocityMultiplikator");
    
    if (rand()%2 == 0)
        normal *= (-1);

    return normal;
}

bool ServerCalculations::doesPlayerCollideWithArenaBounds(vec3f position)
{
    return (position.getLength() + PLAYER_SIZE > ARENA_RADIUS);
}

void ServerCalculations::recalcVelocityAndPositionAtArenaBounds(vec3f &velocity, vec3f &position)
{
    vec3f maxPosition = position;
    maxPosition.normalize();
    maxPosition *= (ARENA_RADIUS - PLAYER_SIZE);

    vec3f overBoundsVector = maxPosition - position;
    position += overBoundsVector * 2;

    vec3f velocityModifier = velocity.projectTo(overBoundsVector);
    velocityModifier *= -1;

    velocity += (velocityModifier * 1.2f);
    velocity *= 0.9f;
}


vec3f ServerCalculations::getPointOfImpact(vec3f position, vec3f orientation)
{
    float a = ARENA_RADIUS;
    vec3f arenaSphereCenter(0.0, ARENA_RADIUS, 0.0);
    vec3f positionToCenter = (arenaSphereCenter-position);
    float c = positionToCenter.getLength();
    float alpha = orientation.angle(positionToCenter);

    float gamma = asin((sin(alpha) * c / a));

    float beta = (float)getPi() - alpha - gamma;
    float b = sin(beta) * a / sin(alpha);

    orientation.normalize();
    orientation *= b;
    return position + orientation;
}