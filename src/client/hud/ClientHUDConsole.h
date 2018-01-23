//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDConsole
//	Manages the the console
//
//*********************************************************

#ifndef CLIENT_HUD_CONSOLE_H
#define CLIENT_HUD_CONSOLE_H

#include "ClientHUD.h"
#include "OgreConsoleForGorilla.h"

#include "Ogre.h"
#include "OIS.h"

#include <sstream>



class ClientHUDConsole : public ClientHUD
{
public:
	ClientHUDConsole(Ogre::Viewport* viewport, OIS::Keyboard* keyboard);
	~ClientHUDConsole(void);

	void update();
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);
	void toggleVisibility();
	bool getVisibility();

private:
	
	Gorilla::Screen*	mScreen;
	OgreConsole*		mConsole;
	Gorilla::Layer*		mFPSLayer;
	Gorilla::Caption*	mFPS;
	Ogre::Real			mTimer;
	OIS::Keyboard*		mKeyboard;
	OIS::KeyListener*	mKeyListener;

	bool				mVisibility;
};

void connectToServer(Ogre::StringVector& vec);

#endif