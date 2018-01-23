//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	graviator_client
//	The main class of the ogre-app.
//
//*********************************************************

//#ifndef __graviator_client_h_
//#define __graviator_client_h_
//
//#include "BaseApplication.h"
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#include "../res/resource.h"
//#endif

#include "client\state\ClientStateManager.h"
#include "client\hud\ClientHUDConsole.h"

#include <OgreRoot.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>


class graviator_client: public Ogre::FrameListener, public OIS::MouseListener, public Ogre::WindowEventListener, OgreBites::SdkTrayListener //: public BaseApplication
{
public:
    graviator_client(void);
    virtual ~graviator_client(void);
	bool go(void);

private:

	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){return true;}
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){return true;}

	bool processUnbufferedInput(const Ogre::FrameEvent &evt);
		

	Ogre::Root *mRoot;
	Ogre::String mPluginsCfg, mResourcesCfg;
	Ogre::RenderWindow *mWindow;

	

	// OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
	bool mShutDown;

	// OIS Input devices
    OIS::InputManager* mOISInputManager;
    OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;

	ClientStateManager *mStateManager;

};


//#endif // #ifndef __graviator_client_h_
