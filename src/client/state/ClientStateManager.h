//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientStateManager
//	Manages states of the game.
//
//*********************************************************

#ifndef CLIENT_STATE_MANAGER
#define CLIENT_STATE_MANAGER


#include "ClientState.h"


class ClientStateManager
{
public:
	ClientStateManager(ClientStateSharedElements *sharedElements, StateName firstStateName);
	~ClientStateManager(void);

	bool update();
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);

private:
	ClientStateSharedElements *mSharedElements;

	ClientState *mCurrentState;


};

#endif