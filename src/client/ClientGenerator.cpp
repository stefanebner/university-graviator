#include "ClientGenerator.h"
#include "..\Utility.h"

ClientGenerator::ClientGenerator(Ogre::SceneManager *sceneManager)
{
	mSceneManager = sceneManager;
	mRotationAxis = Ogre::Vector3(13.0,7.0,5.0);
	mRotationAxis.normalise();
	Ogre::Entity *generatorEntity = mSceneManager->createEntity("generator", "generator.mesh");
	mGeneratorNode = mSceneManager->getRootSceneNode()->createChildSceneNode("generatorNode");
	mGeneratorNode->attachObject(generatorEntity);
}

ClientGenerator::~ClientGenerator(void)
{
	mSceneManager->destroyEntity("generator");
	mSceneManager->getRootSceneNode()->removeAndDestroyChild("generatorNode");
}

void ClientGenerator::update()
{
	Ogre::Quaternion rotationQuaternion((Ogre::Radian)0.1, mRotationAxis);
	mGeneratorNode->rotate(rotationQuaternion);
}
