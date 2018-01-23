#include "ClientInputManager.h"

ClientInputManager::ClientInputManager(void)
{
	setControlsToStandardMapping();
	mControlInfo.left = false;
	mControlInfo.right = false;
	mControlInfo.fire = false;
	mControlInfo.cameraVector = vec3f();

}

ClientInputManager::~ClientInputManager(void)
{
}

void ClientInputManager::setControlsToStandardMapping()
{
	mLeftKey = OIS::KC_A;
	mRightKey = OIS::KC_D;
	mFireKey = OIS::MB_Left;
}

ClientControlInfo ClientInputManager::getControlInfo()
{
	ClientControlInfo returnControlInfo = mControlInfo;
	mControlInfo.left = false;
	mControlInfo.right = false;
	mControlInfo.fire = false;
	return returnControlInfo;
}



