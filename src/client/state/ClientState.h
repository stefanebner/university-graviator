//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientState
//	State-super-class. Necessary for the state-pattern.
//
//*********************************************************

#ifndef CLIENT_STATE
#define CLIENT_STATE

#include <OgreRoot.h>

#include <OgreRenderWindow.h>
#include <OISEvents.h>
#include <OISMouse.h>
#include <OISKeyboard.h>


struct ClientStateSharedElements
{
	Ogre::Root *root;
	Ogre::RenderWindow *window;
	OIS::Mouse *mouse;
	OIS::Keyboard *keyboard; 
};


enum StateName {QUIT_STATE = 0, LOBBY_STATE = 1, GAME_STATE = 2};

class ClientState
{
public:
	virtual ~ClientState(void) {}
	virtual void setup(ClientStateSharedElements *sharedElements) = 0;

	virtual StateName getName() = 0;
	virtual StateName update() = 0;

	virtual bool mouseMoved(const OIS::MouseEvent &arg) = 0;
	virtual bool processUnbufferedInput(const Ogre::FrameEvent &evt) = 0;

};

#endif