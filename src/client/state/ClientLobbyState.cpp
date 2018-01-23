#include "ClientLobbyState.h"

ClientLobbyState::ClientLobbyState(void)
{
	mStartGame = false;
}


ClientLobbyState::~ClientLobbyState(void)
{
	delete mHud;
	mWindow->removeViewport(0);
	mSceneManager->destroyAllCameras();
}


void ClientLobbyState::setup(ClientStateSharedElements *sharedElements)
{
	mRoot = sharedElements->root;
	mWindow = sharedElements->window;
	mMouse = sharedElements->mouse;
	mKeyboard = sharedElements->keyboard;

	mSceneManager = mRoot->createSceneManager("DefaultSceneManager");

	mCamera = mSceneManager->createCamera("LobbyCamera");
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(1500);
	mCamera->setPosition(0.0, 1.0, 0.0);
	mCamera->lookAt(Ogre::Vector3(0,0,0));

	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	mHud = new ClientHUDLobby(vp);
}


StateName ClientLobbyState::update()
{
	if (mStartGame)
	{
		return GAME_STATE;
	}
	return getName();
}


bool ClientLobbyState::mouseMoved(const OIS::MouseEvent &arg)
{
	return true;
}


bool ClientLobbyState::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
	if (mMouse->getMouseState().buttonDown(OIS::MB_Left))
		mStartGame = true;
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) 
		return false;
	return true;
}