#include "ServerObject.h"
#include "LuaStateManager.h"

#define LUA LuaStateManager::getInstance()

ServerObject::ServerObject()
{
}

char ServerObject::getType()
{
	return mType;
}

void ServerObject::setType( char type )
{
	mType = type;
	switch(mType)
	{
	case 's': 
		mSecondsToLive = LUA.getInt("massShotTimeToLive");
		mMass = LUA.getFloat("massShot");
		mMortal=true;
		break;
	case 'g':
		mSecondsToLive = LUA.getInt("massProjectionTimeToLive");
		mMass = LUA.getFloat("massProjection");
		mMortal=true;
		break;
	case 'c':
		mMortal=false;
		mMass = LUA.getFloat("massCenter");
		break;
	case 'p':
		mMortal = false;
		mMass = LUA.getFloat("massPlayer");
		break;
	case 'd':
	
		break;
	}
}

vec3f ServerObject::getPosition()
{
	return mPosition;
}

void ServerObject::setPosition( const vec3f newPosition )
{
	mPosition = newPosition;
}

float ServerObject::getMass()
{
	return mMass;
}

void ServerObject::setMass( float newMass )
{
	mMass = newMass;
}

unsigned int ServerObject::getID()
{
	return mID;
}

void ServerObject::setID( unsigned int id )
{
	mID = id;
}

vec3f ServerObject::getVelocity()
{
	return mVelocity;
}

void ServerObject::setVelocity( vec3f velocity )
{
	mVelocity = velocity;
}


vec3f ServerObject::getOrientation()
{
	return mOrientation;
}

void ServerObject::setOrientation( vec3f orientation )
{
	mOrientation = orientation;
}
