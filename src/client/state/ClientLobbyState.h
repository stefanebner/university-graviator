//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientLobbyState
//	Manages the lobby ob the game.
//
//*********************************************************

#ifndef CLIENT_LOBBY_STATE
#define CLIENT_LOBBY_STATE

#include "ClientState.h"
#include "..\hud\ClientHUDLobby.h"

class ClientLobbyState: public ClientState
{
public:
	ClientLobbyState(void);
	~ClientLobbyState(void);

	virtual void setup(ClientStateSharedElements *sharedElements);

	virtual StateName getName(){return LOBBY_STATE;}
	virtual StateName update();

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool processUnbufferedInput(const Ogre::FrameEvent &evt);

private:
	Ogre::Root *mRoot;
	Ogre::RenderWindow *mWindow;
	Ogre::SceneManager *mSceneManager;
	Ogre::Camera *mCamera;
	OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;

	// HUD
	ClientHUDLobby *mHud;
	bool mStartGame;
};

#endif