//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientGameState
//	Manages the game.
//
//*********************************************************

#ifndef CLIENT_GAME_STATE
#define CLIENT_GAME_STATE

#include "ClientState.h"

#include "..\ClientGenerator.h"
#include "..\ClientObjectManager.h"
#include "..\ClientInputManager.h"
#include "..\hud\ClientHUDGame.h"
#include "..\..\vec3f.h"
#include "..\ClientFMODManager.h"


class ClientGameState: public ClientState
{
public:
	ClientGameState(void);
	~ClientGameState(void);
	virtual void setup(ClientStateSharedElements *sharedElements);

	virtual StateName getName(){return GAME_STATE;}
	virtual StateName update();

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool processUnbufferedInput(const Ogre::FrameEvent &evt);

private:

	void deleteObjects();
	void updateObjects();

	void updateHud();

	void updateCameraPosition();
	void applyMouseMovementToCamera(float xMovement, float yMovement);
	void orientateCamera();
	Ogre::SceneNode *createSceneNode(RenderObjectInformation objectInformation);
	void updateSceneNode(Ogre::SceneNode *objectSceneNode, RenderObjectInformation objectInformation);
	bool checkPlayerIsDead();
	void handlePlayerDeath();
	Ogre::String idToNodeName(int id);

	Ogre::Root *mRoot;
	Ogre::RenderWindow *mWindow;
	Ogre::SceneManager *mSceneManager;
	Ogre::Camera *mCamera;
	OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;

	// direct user control influences
	float mMouseSensitivity, mMaxMouseSensitivity;

	vec3f mCameraPosition;
	Ogre::Vector3 mCameraUpVector;
	Ogre::Radian mCameraOrientationAngle[2];
	ClientObjectManager *mObjectManager;
	ClientInputManager *mInputManager;

	bool mDeathSoundPlayed;

	// HUD
	ClientHUDGame *mHud;

	ClientGenerator *mGenerator;

};

void *runNetworkClient(void *arg);


#endif