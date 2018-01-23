#include "ClientHUDLobby.h"

#include "..\..\GraviatorSimpleTypes.h"


ClientHUDLobby::ClientHUDLobby(Ogre::Viewport* viewport)
{
	this->init(viewport);
	
	mHUD = mSilverback->createScreen(mViewport, "dejavu");
	mHUDLayer = mHUD->createLayer();
	mStartGame = mHUDLayer->createRectangle((Ogre::Real)mViewport->getActualWidth()/2-72, (Ogre::Real)mViewport->getActualHeight()/2, 145, 20);
	mStartGame->background_colour(Gorilla::rgb(255, 255, 255, 100));
	mTextToStart = mHUDLayer->createCaption(9, (Ogre::Real)mViewport->getActualWidth()/2-68, (Ogre::Real)mViewport->getActualHeight()/2+2, "klick to start the game");
}

ClientHUDLobby::~ClientHUDLobby(void)
{
	mSilverback->destroyScreen(mHUD);
}

void ClientHUDLobby::update()
{

}


// private functions -------------------------------------------------------
