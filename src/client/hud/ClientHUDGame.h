//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDGame
//	Manages the ingame HUD
//
//*********************************************************

#ifndef CLIENT_HUD_GAME
#define CLIENT_HUD_GAME

#include "ClientHUD.h"
#include "..\ClientObjectManager.h"
#include "..\..\GraviatorSimpleTypes.h"

class ClientHUDGame: public ClientHUD
{
public:
	ClientHUDGame(Ogre::Viewport* viewport, ClientObjectManager *objectManager);
	~ClientHUDGame(void);

	void update();
	void showRestartWith(float currentTimerCount);
	void setLeftArrowAlpha(Ogre::Real alpha);
	void setRightArrowAlpha(Ogre::Real alpha);
	void setRestartLayerAlpha(Ogre::Real alpha);
	Ogre::Real getLeftArrowAlpha();
	Ogre::Real getRightArrowAlpha();
	Ogre::Real getRestartLayerAlpha();

private:
	void updateEnergy(Ogre::Real value);
	void setPlayerPoints(Ogre::Real points);
	void setEnemyPoints(Ogre::Real points);
	void updateAlphaLayers();
	void clampAlpha(int* alpha);
	

	ClientObjectManager*	mObjectManager;	
	//HUD related
	Gorilla::Screen*        mHUD;
	Gorilla::Layer*         mHUDLayer;
	Gorilla::Layer*         mRestartLayer;
	Gorilla::Layer*         mEnergyLayer;
	Gorilla::Layer*			mCrosshairLayer;
	Gorilla::Layer*			mSteeringLeftLayer;
	Gorilla::Layer*			mSteeringRightLayer;
	Gorilla::Rectangle*		mCrosshair;
	Gorilla::Rectangle*		mEnergyValue;
	Gorilla::Rectangle*		mEnergyValueBackground;
	Gorilla::Rectangle*		mSteeringLeft;
	Gorilla::Rectangle*		mSteeringRight;
	Gorilla::Rectangle*		restartLayerBackground;
	Ogre::Real*				mBasePower;
	Gorilla::Caption*		mEnemyPoints;
	Gorilla::Caption*		mPlayerPoints;
	Gorilla::Caption*		mRestart;

	Ogre::Real				mSteeringLeftAlpha;
	Ogre::Real				mSteeringRightAlpha;
	Ogre::Real				mRestartLayerAlpha;
	
	
};

#endif