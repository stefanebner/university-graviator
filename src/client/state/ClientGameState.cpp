#include "ClientGameState.h"

#include "..\ClientNetworkHandler.h"
#include "..\..\GraviatorSimpleTypes.h"
#include "..\..\Utility.h"

#include <OgreEntity.h>
#include <pthread.h>

ClientGameState::ClientGameState(void):
	mCameraPosition(0.0,300.0,10.0),
	mCameraUpVector(0.0,1.0,0.0),
	mMaxMouseSensitivity(10),
	mMouseSensitivity(3),
	mDeathSoundPlayed(true)
{
	mCameraOrientationAngle[0] = mCameraOrientationAngle[1] = 0;
}

ClientGameState::~ClientGameState(void)
{
	deleteObjects();

	delete mObjectManager;
	delete mInputManager;
	delete mHud;
	delete mGenerator;

	mWindow->removeViewport(0);
	mSceneManager->destroyAllCameras();
	mSceneManager->destroyAllMovableObjects();
	mSceneManager->destroyAllLights();
	mRoot->destroySceneManager(mSceneManager);

	SoundSystem.ExitFMOD();

}

void ClientGameState::setup(ClientStateSharedElements *sharedElements)
{
	
	mRoot = sharedElements->root;
	mWindow = sharedElements->window;
	mMouse = sharedElements->mouse;
	mKeyboard = sharedElements->keyboard;

	mSceneManager = mRoot->createSceneManager("DefaultSceneManager");

	// Create and set up the camera
	mCamera = mSceneManager->createCamera("GameCamera");
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(1500);
	mCamera->setPosition(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z);
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Create the arena
	Ogre::Entity *arenaEntity = mSceneManager->createEntity("arena", "arena.mesh");
	Ogre::SceneNode *arenaNode = mSceneManager->getRootSceneNode()->createChildSceneNode("arenaNode");
	arenaNode->attachObject(arenaEntity);

	mGenerator = new ClientGenerator(mSceneManager);

	
	// Set ambient light
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	 
	// Create a light
	Ogre::Light* l = mSceneManager->createLight("MainLight");
	l->setPosition(20,80,50);
	

 	mObjectManager = new ClientObjectManager;
	mInputManager = new ClientInputManager;

	// create the network-thread
	ClientNetworkHandler *networkHandler = new ClientNetworkHandler(mObjectManager, mInputManager);
	pthread_t networkTid;
	pthread_create(&networkTid, NULL, &runNetworkClient, (void *)networkHandler);

	// set up the ingame hud
	mHud = new ClientHUDGame(vp, mObjectManager);

	// start sound
	SoundSystem.InitFMOD();
	SoundSystem.playGameSound();
}



StateName ClientGameState::update()
{
	if (mObjectManager->getQuitWithConnectionFailure())
		return LOBBY_STATE;
		
	deleteObjects();
	updateObjects();
	mObjectManager->makeObjectStatesOlder();
	mObjectManager->cleanUpObjectStates();

	handlePlayerDeath();
	mGenerator->update();
	updateCameraPosition();
	updateHud();

    SoundSystem.update();

	return getName();
}


bool ClientGameState::mouseMoved(const OIS::MouseEvent &arg)
{
	
	applyMouseMovementToCamera((float)arg.state.X.rel, (float)arg.state.Y.rel);
	Ogre::Quaternion qu = mCamera->getOrientation();
	mInputManager->setCameraVectorInfo(vec3f(qu.w, qu.x, qu.z));
	updateCameraPosition();
	
	return true;
}

bool ClientGameState::processUnbufferedInput(const Ogre::FrameEvent &evt)
{

	if (mMouse->getMouseState().buttonDown(mInputManager->getFireKeyCode()))
		mInputManager->setFireControlInfo(true);

	if (mKeyboard->isKeyDown(mInputManager->getLeftKeyCode())) 
	{
		mInputManager->setLeftControlInfo(true);
		mHud->setLeftArrowAlpha(1.0);
//		SoundSystem.playSteeringOwnSound();
	}
	if (mKeyboard->isKeyDown(mInputManager->getRightKeyCode())) 
	{
		mInputManager->setRightControlInfo(true);
		mHud->setRightArrowAlpha(1.0);
//		SoundSystem.playSteeringOwnSound();
	}
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) 
		return false;

	return true;
}






void ClientGameState::deleteObjects()
{
	int actualObjectId = mObjectManager->pullNextUnpulledDeletion();
	while(actualObjectId != NO_OBJECTS_AVAILABLE)
	{
		Ogre::SceneNode *objectSceneNode;
		try
		{
			objectSceneNode = mSceneManager->getSceneNode(idToNodeName(actualObjectId));
			
			mSceneManager->destroyEntity(idToNodeName(actualObjectId));
			mSceneManager->getRootSceneNode()->removeAndDestroyChild(idToNodeName(actualObjectId));
			actualObjectId = mObjectManager->pullNextUnpulledDeletion();
		}
		catch(Ogre::ItemIdentityException)
		{
			actualObjectId = mObjectManager->pullNextUnpulledDeletion();
		}
	}
}


void ClientGameState::updateObjects()
{
	RenderObjectInformation actualObjectInformation = mObjectManager->pullNextUnpulledState();
	while(actualObjectInformation.id != NO_OBJECTS_AVAILABLE)
	{
		Ogre::SceneNode *objectSceneNode;
		try
		{
			objectSceneNode = mSceneManager->getSceneNode(idToNodeName(actualObjectInformation.id));
		}
		catch(Ogre::ItemIdentityException)
		{
			objectSceneNode = createSceneNode(actualObjectInformation);
		}
		updateSceneNode(objectSceneNode, actualObjectInformation);

		actualObjectInformation = mObjectManager->pullNextUnpulledState();
	}
}

void ClientGameState::updateHud()
{
	if(checkPlayerIsDead())
		mHud->showRestartWith(mObjectManager->getOwnPlayerAlignment().x);
	else
		mHud->showRestartWith(0);
	mHud->update();
}

void ClientGameState::updateCameraPosition()
{
	mCameraPosition = mObjectManager->getOwnPlayerPosition(); 
	if(checkPlayerIsDead())
	{
		mCameraPosition.x = 10;
		mCameraPosition.y = 100;
		mCameraPosition.z = 0;
	}
	mCamera->setPosition(mCameraPosition.x, mCameraPosition.y, mCameraPosition.z);

	vec3f cameraUpVector = getArenaNormalAtPosition(mCameraPosition);
	cameraUpVector.normalize();
	mCameraUpVector = Ogre::Vector3(cameraUpVector.x, cameraUpVector.y, cameraUpVector.z);

	orientateCamera();
}


void ClientGameState::applyMouseMovementToCamera(float xMovement, float yMovement)
{
	Ogre::Radian rotationAmountHorizontal = (Ogre::Radian) -xMovement/180.0 * Ogre::Math::PI;
	Ogre::Radian rotationAmountVertical = (Ogre::Radian) -yMovement/180.0 * Ogre::Math::PI;
	// apply mouse sensitivity
	rotationAmountHorizontal *= mMouseSensitivity / mMaxMouseSensitivity;
	rotationAmountVertical *= mMouseSensitivity / mMaxMouseSensitivity;

	mCameraOrientationAngle[0] += rotationAmountHorizontal;
	mCameraOrientationAngle[1] += rotationAmountVertical;

	orientateCamera();	
}


void ClientGameState::orientateCamera()
{
	// look at (0,0,0)
	mCamera->setDirection(Ogre::Vector3(-mCameraPosition.x, -mCameraPosition.y, -mCameraPosition.z));

	Ogre::Quaternion rotationQuaternionHorizontal(mCameraOrientationAngle[0], mCameraUpVector);
	mCamera->rotate(rotationQuaternionHorizontal);

	Ogre::Vector3 cameraRightVector = mCamera->getDerivedRight();
	
	Ogre::Quaternion rotationQuaternionVertival(mCameraOrientationAngle[1], cameraRightVector);
	mCamera->rotate(rotationQuaternionVertival);

	Ogre::Vector3 cameraDirection = mCamera->getDerivedDirection();
	mInputManager->setCameraVectorInfo(vec3f(cameraDirection.x, cameraDirection.y, cameraDirection.z));

}


Ogre::SceneNode *ClientGameState::createSceneNode(RenderObjectInformation objectInformation)
{
	Ogre::String objectNodeName = idToNodeName(objectInformation.id);
	Ogre::Entity *objectEntity;
	switch (objectInformation.type)
	{
	case 'p':
		objectEntity = mSceneManager->createEntity(objectNodeName, "ship.mesh");
		SoundSystem.playJoinSound();
        break;
	case 'c':
		objectEntity = mSceneManager->createEntity(objectNodeName, "center.mesh");
        break;
    case 's':
        objectEntity = mSceneManager->createEntity(objectNodeName, "gravPoint.mesh");
        SoundSystem.playShotSound();
        break;
    case 'g':
        objectEntity = mSceneManager->createEntity(objectNodeName, "gravPoint.mesh");
        SoundSystem.playSteeringOnMapSound();
        break;
	default:
		objectEntity = mSceneManager->createEntity(objectNodeName, "weapon.mesh");
		break;
	}
	Ogre::SceneNode *objectNode = mSceneManager->getRootSceneNode()->createChildSceneNode(objectNodeName);
	objectNode->attachObject(objectEntity);

	return objectNode;
}


void ClientGameState::updateSceneNode(Ogre::SceneNode *objectSceneNode, RenderObjectInformation objectInformation)
{
	// update the position
	objectSceneNode->setPosition(	objectInformation.position.x,
									objectInformation.position.y,
									objectInformation.position.z);
	// update the orientation
	vec3f upVector = getArenaNormalAtPosition(vec3f(objectInformation.position.x,
													objectInformation.position.y,
													objectInformation.position.z));
	Ogre::Vector3 orientationDirection(objectInformation.alignment.x, objectInformation.alignment.y, objectInformation.alignment.z);
	Ogre::Vector3 orientationUp(upVector.x,upVector.y,upVector.z);
	Ogre::Vector3 orientationRight = orientationUp.crossProduct(orientationDirection);
	orientationDirection = orientationRight.crossProduct(orientationUp);

	Ogre::Quaternion objectOrientation(	orientationRight.normalisedCopy(), 
										orientationUp.normalisedCopy(), 
										orientationDirection.normalisedCopy());
	objectSceneNode->setOrientation(objectOrientation);
}

bool ClientGameState::checkPlayerIsDead()
{
	bool playerIsDead = false;
	if ((mObjectManager->getOwnPlayerPosition()).isZero())
		playerIsDead = true;
	return playerIsDead;
}

void ClientGameState::handlePlayerDeath()
{
	if (checkPlayerIsDead())
	{
		if (!mDeathSoundPlayed)
		{
			SoundSystem.playDeathSound();
			mDeathSoundPlayed = true;
		}
	}
	else
		mDeathSoundPlayed = false;
}

Ogre::String ClientGameState::idToNodeName(int id)
{
	Ogre::String nodeName(toString(id));
	nodeName.append("Node");
	return nodeName;
}


void *runNetworkClient(void *arg)
{
	pthread_detach(pthread_self());
	ClientNetworkHandler *networkHandler = (ClientNetworkHandler*)arg;

    for (unsigned int i = 4000; i < 4010; i++)
        networkHandler->start(i);

	// use this line to connect to a server running in LAN allowing multicast
	//networkHandler->connectToFirstResponingServer();

	// use this line to connect to a specific ip
    networkHandler->connectToIp(Address(192,168,123,1,5000));

	networkHandler->handleConnection();

	delete networkHandler;
	return NULL;
}