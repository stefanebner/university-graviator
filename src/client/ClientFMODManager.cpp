#include "ClientFMODManager.h"
#include "fmod_errors.h"

ClientFMODManager* ClientFMODManager::instance = 0;


ClientFMODManager::ClientFMODManager()
{
	mChannel = 0;
}

ClientFMODManager::~ClientFMODManager()
{
	mChannel = 0;
}

ClientFMODManager& ClientFMODManager::getInstance()
{
	if(!instance)
		instance = new ClientFMODManager();
	return *instance;
}

void ClientFMODManager::destroy()
{
	if(!instance)
		delete instance;
	instance = 0;
}


void ClientFMODManager::update()
{
	mSystem->update();
}

// Starts FMOD
void ClientFMODManager::InitFMOD( void )
{
	mResult = System_Create( &mSystem ); // Create the main system object.
	errCheck(mResult);

	mResult = mSystem->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD system.
	errCheck(mResult);

	mResult = mSystem->createStream("./media/sounds/game_music.wav", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &mGameState);
	errCheck(mResult);

	mResult = mSystem->createSound("./media/sounds/steering_own.wav", FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &mSteeringOwn);
	errCheck(mResult);

	mResult = mSystem->createSound("./media/sounds/steering_onMap.wav", FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &mSteeringOnMap);
	errCheck(mResult);

    mResult = mSystem->createSound("./media/sounds/shot.wav", FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &mShot);
    errCheck(mResult);

    mResult = mSystem->createSound("./media/sounds/join.wav", FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &mJoinSound);
    errCheck(mResult);

    mResult = mSystem->createSound("./media/sounds/death.wav", FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &mDeathSound);
    errCheck(mResult);

	
}

// Stops FMOD
void ClientFMODManager::ExitFMOD( void )
{
	//sets all sounds to zero volume
	stopSound();
	
	mGameState = mShot = mSteeringOwn = mSteeringOnMap = NULL;

	mResult = mGameState->release();
	errCheck(mResult);

	mResult = mShot->release();
	errCheck(mResult);

    mResult = mSteeringOwn->release();
    errCheck(mResult);

	mResult = mSteeringOnMap->release();
    errCheck(mResult);

    mResult = mJoinSound->release();
    errCheck(mResult);

    mResult = mDeathSound->release();
    errCheck(mResult);

	mResult = mSystem->close();
	errCheck(mResult);

	mResult = mSystem->release();
	errCheck(mResult);
}

void ClientFMODManager::errCheck( FMOD_RESULT result )
{
	if (result != FMOD_OK)
	{
		printf("FMOD ERORR! (%d) %s \n", result, FMOD_ErrorString(result));
	}
}

void ClientFMODManager::playShotSound()
{
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mShot, false, &mChannel);

	errCheck(mResult);
}

void ClientFMODManager::playSteeringOwnSound()
{
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSteeringOwn, false, &mChannel);

	errCheck(mResult);
}

void ClientFMODManager::playSteeringOnMapSound()
{
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSteeringOnMap, false, &mChannel);

	errCheck(mResult);
}

void ClientFMODManager::playGameSound()
{
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mGameState, false, &mChannel);

	errCheck(mResult);
}

void ClientFMODManager::playJoinSound()
{
    mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mJoinSound, false, &mChannel);

    errCheck(mResult);
}

void ClientFMODManager::playDeathSound()
{
    mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mDeathSound, false, &mChannel);

    errCheck(mResult);
}

void ClientFMODManager::stopSound()
{
	mChannel->setVolume(0);
}
