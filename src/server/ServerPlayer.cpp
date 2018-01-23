#include "ServerPlayer.h"
#include "..\Utility.h"

typedef ServerObjectFactory<ServerPlayer> PlayerFactory;

#define PF PlayerFactory::getInstance()

namespace //so this will be created before entering main()
{
	ServerPlayer* createPlayer()
	{
		return new ServerPlayer();
	}

	bool registerPlayer = PF.Register("createPlayer", createPlayer);
}

ServerPlayer::ServerPlayer()
{
	setType('p');
	mScore = 0;
	mPosition.set(0,0,0);
	mResawnTimer = new PrecisionTimer(10.0);
	mResawnTimer->start();

    mTimerShot = new PrecisionTimer(0.8);
    mTimerShot->start();
    mTimerSteerLeft = new PrecisionTimer(0.2);
    mTimerSteerLeft->start();
    mTimerSteerRight = new PrecisionTimer(0.2);
    mTimerSteerRight->start();
}

ServerPlayer::~ServerPlayer(void)
{
    delete mResawnTimer;
}

void ServerPlayer::update()
{
    mResawnTimer->setUpdateRate(LUA.getFloat("respawnTime"));
	mServerEnergyManager.update(mPosition.getLength());
}

vec3f ServerPlayer::getForce( vec3f affectedLocation )
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

vec3f ServerPlayer::getEnergy()
{
	return mServerEnergyManager.getStatus();
}

bool ServerPlayer::canSteerLeft()
{
    mTimerSteerLeft->setUpdateRate(LUA.getFloat("energySteerTime"));
    return
        mTimerSteerLeft->readyForNextUpdate() &&
        mServerEnergyManager.check("steeringPoint");
}

bool ServerPlayer::canSteerRight()
{
    mTimerSteerRight->setUpdateRate(LUA.getFloat("energySteerTime"));
    return
        mTimerSteerRight->readyForNextUpdate() &&
        mServerEnergyManager.check("steeringPoint");
}

bool ServerPlayer::canShoot()
{
    mTimerShot->setUpdateRate(LUA.getFloat("energyShotTime"));
    return
        mTimerShot->readyForNextUpdate() &&
        mServerEnergyManager.check("shotPoint");
}

void ServerPlayer::steerLeft()
{
    mTimerSteerLeft->update();
    mServerEnergyManager.add("steeringPoint");
}

void ServerPlayer::steerRight()
{
    mTimerSteerRight->update();
    mServerEnergyManager.add("steeringPoint");
}


void ServerPlayer::shoot()
{
    mTimerShot->update();
    mServerEnergyManager.add("shotPoint");
}

void ServerPlayer::resetEnergy()
{
    mServerEnergyManager.reset();
}


void ServerPlayer::increaseScore()
{
    mScore++;
}

unsigned int ServerPlayer::getScore()
{
    return mScore;
}

void ServerPlayer::resetScore()
{
    mScore = 0;
}

void ServerPlayer::die()
{
    mPosition.set(0, 0, 0);
    mResawnTimer->start();
}

bool ServerPlayer::isDead()
{
    return mPosition.isZero();
}

bool ServerPlayer::shouldRespawn()
{
    if (isDead())
        return mResawnTimer->readyForNextUpdate();
    else
        return false;
}

float ServerPlayer::getRespawnTime()
{
    return mResawnTimer->getTimeTillUpdate();
}