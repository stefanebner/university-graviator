#include "ServerGravitationalObject.h"
#include "../Utility.h"

typedef ServerObjectFactory<ServerGravitationalObject> ObjectFactory;

#define OF ObjectFactory::getInstance()


namespace //so this will be created before entering main()
{
	ServerGravitationalObject* shotPoint()
	{
		return new ServerGravitationalObject();
	}

	bool registerShot = OF.Register("shotPoint", shotPoint);
}
namespace
{
	ServerGravitationalObject* steeringPoint()
	{
		return new ServerGravitationalObject();
	}

	bool registerSteering = OF.Register("steeringPoint", steeringPoint);
}

namespace
{
	ServerGravitationalObject* centerPoint()
	{
		return new ServerGravitationalObject();
	}

	bool registerCenter = OF.Register("centerPoint", centerPoint);
}

ServerGravitationalObject::~ServerGravitationalObject(void)
{
}

void ServerGravitationalObject::update()
{
}

bool ServerGravitationalObject::shouldBeKilled()
{
	if (mMortal)
	{
		if (clock() > mCreationTime + mSecondsToLive * CLOCKS_PER_SEC)
		{
			return true;
		}
		else return false;
	}
	else return false;
}

vec3f ServerGravitationalObject::getForce( vec3f affectedLocation )
{
	vec3f returnVector(0, 0, 0);
	float gravity = LUA.getFloat("gravity");

    vec3f calculationPosition;
    
    if (USE_2D_CALCULATION)
        calculationPosition = mPosition;
    else
        calculationPosition = transformToCalculationCoordinates(mPosition);
    
    
    vec3f radius(
        affectedLocation.x - calculationPosition.x,
        0,
        affectedLocation.z - calculationPosition.z);
        
	returnVector = (-radius/radius.getLength() * (gravity * mMass) / radius.dotSelf());
	
	if (returnVector.getLength() > mMass / 10)
	{
	    returnVector.normalize();
	    returnVector = returnVector * mMass / 10;
	}
	
	return returnVector;
}
