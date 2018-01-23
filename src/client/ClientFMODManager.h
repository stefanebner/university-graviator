//*********************************************************
//	Graviator - QPT2a - FH-Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	FMODManager.h
//	Singleton that handles all the different sounds
//	like Effects and Music
//	uses FMOD
//
//*********************************************************


#ifndef CLIENT_FMOD_MANAGER
#define CLIENT_FMOD_MANAGER

#include "fmod.hpp"
#include "windows.h"
#include <iostream>

#define SoundSystem ClientFMODManager::getInstance()

using namespace FMOD;

class ClientFMODManager
{
public:
	
	~ClientFMODManager();

	static ClientFMODManager& getInstance();
	void destroy();

	// needs to be called each frame
	void update();

	void InitFMOD();
	void ExitFMOD();

	//not yet with 3d sound
	void playShotSound();
	void playSteeringOwnSound();
	void playSteeringOnMapSound();
	void playGameSound();
	void playMenuSound();
	void playJoinSound();
	void playDeathSound();
	void stopSound();



private:
	ClientFMODManager();

	System*				mSystem;
	Sound*				mGameState;
	Sound*				mMenuState;
	Sound*				mShot;
	Sound*				mSteeringOwn;
	Sound*				mSteeringOnMap;
	Sound*				mSound;
	Sound*				mJoinSound;
	Sound*				mDeathSound;
	SoundGroup*			mSoundGroup;
	Channel*			mChannel;
	ChannelGroup*		mChannelGroup;
	FMOD_RESULT			mResult;

	static ClientFMODManager*	instance;
	
	void errCheck(FMOD_RESULT result);
	
	

};



#endif