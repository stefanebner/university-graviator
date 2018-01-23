//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDLobby
//	Manages the lobby HUD
//
//*********************************************************

#ifndef CLIENT_HUD_LOBBY
#define CLIENT_HUD_LOBBY

#include "ClientHUD.h"
#include "..\ClientObjectManager.h"

class ClientHUDLobby: public ClientHUD
{
public:
	ClientHUDLobby(Ogre::Viewport* viewport);
	~ClientHUDLobby(void);

	void update();


private:

	//HUD related
	Gorilla::Screen*	mHUD;
	Gorilla::Layer*		mHUDLayer;
	Gorilla::Rectangle* mStartGame;
	Gorilla::Caption*	mTextToStart;
};

#endif