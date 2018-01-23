//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientInputManager
//	Gets the userinput so that the network can send it to the server.
//
//*********************************************************

#ifndef CLIENT_INPUT_MANAGER
#define CLIENT_INPUT_MANAGER

#include "..\GraviatorSimpleTypes.h"
#include "..\vec3f.h"

#include <OISKeyboard.h>
#include <OISMouse.h>

struct ClientControlInfo
{
	bool left, right, fire;
	vec3f cameraVector;
};

class ClientInputManager
{
public:
	ClientInputManager(void);
	~ClientInputManager(void);
	void setControlsToStandardMapping();
	void setLeftControlInfo(bool state){mControlInfo.left = state;}
	void setRightControlInfo(bool state){mControlInfo.right = state;}
	void setFireControlInfo(bool state){mControlInfo.fire = state;}
	void setCameraVectorInfo(vec3f cameraVector){mControlInfo.cameraVector = cameraVector;}
	ClientControlInfo getControlInfo();
	OIS::KeyCode getLeftKeyCode(){return mLeftKey;}
	OIS::KeyCode getRightKeyCode(){return mRightKey;}
	OIS::MouseButtonID getFireKeyCode(){return mFireKey;}
private:
	ClientControlInfo mControlInfo;
	OIS::KeyCode mLeftKey, mRightKey;
	OIS::MouseButtonID mFireKey;
};

#endif