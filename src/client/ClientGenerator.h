//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientGenerator
//	Manages the spinning generator at the center of the arena.
//
//*********************************************************

#ifndef CLIENT_GENERATOR
#define CLIENT_GENERATOR

#include "..\vec3f.h"
#include <OgreRoot.h>
#include <OgreEntity.h>

class ClientGenerator
{
public:
	ClientGenerator(Ogre::SceneManager *sceneManager);
	~ClientGenerator(void);
	void update();

private:
	Ogre::SceneManager *mSceneManager;
	Ogre::SceneNode *mGeneratorNode;
	Ogre::Vector3 mRotationAxis;

};

#endif