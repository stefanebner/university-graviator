#include "ClientStateManager.h"

#include "ClientLobbyState.h"
#include "ClientGameState.h"

ClientStateManager::ClientStateManager(ClientStateSharedElements *sharedElements, StateName firstStateName):
	mSharedElements(sharedElements)
{
	switch (firstStateName)
	{
	case LOBBY_STATE:
		mCurrentState = new ClientLobbyState;
		break;
	case GAME_STATE:
		mCurrentState = new ClientGameState;
		break;
	default:
		mCurrentState = new ClientLobbyState;
		break;
	}
	mCurrentState->setup(mSharedElements);
}

ClientStateManager::~ClientStateManager(void)
{
}

bool ClientStateManager::update()
{
	bool continueGame = true;
	StateName nextStateName = mCurrentState->update();
	if (nextStateName != mCurrentState->getName())
	{
		delete mCurrentState;
		switch (nextStateName)
		{
		case LOBBY_STATE:
			mCurrentState = new ClientLobbyState;
			break;
		case GAME_STATE:
			mCurrentState = new ClientGameState;
			break;
		case QUIT_STATE:
			continueGame = false;
			break;
		}
		mCurrentState->setup(mSharedElements);
	}
	return continueGame;
}

bool ClientStateManager::mouseMoved(const OIS::MouseEvent &arg)
{
	return mCurrentState->mouseMoved(arg);
}

bool ClientStateManager::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
	return mCurrentState->processUnbufferedInput(evt);
}