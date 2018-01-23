//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUD
//	Abstract class for the HUD.
//
//*********************************************************

#ifndef CLIENT_HUD
#define CLIENT_HUD


#include "ClientHUDResources.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>


class ClientHUD
{
public:
	virtual ~ClientHUD(void) {}
	virtual void update() = 0;
	

protected:

	void init(Ogre::Viewport* viewport) 
	{
		mSilverback =  HUD.getSilverback();
		mViewport = viewport;
	}

	Ogre::Viewport *mViewport;
	Gorilla::Silverback *mSilverback;
};

#endif