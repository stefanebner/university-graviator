

#include "ClientHUDGame.h"

#include "..\..\GraviatorSimpleTypes.h"


ClientHUDGame::ClientHUDGame(Ogre::Viewport* viewport, ClientObjectManager *objectManager)
{
	this->init(viewport);
	mObjectManager = objectManager;

	mHUD = mSilverback->createScreen(mViewport, "dejavu");
	mHUDLayer = mHUD->createLayer();

	// Create our drawing layer
	mCrosshairLayer = mHUD->createLayer();
	mCrosshair = mCrosshairLayer->createRectangle((mViewport->getActualWidth() * 0.5f) - 11, 
		(mViewport->getActualHeight() * 0.5f) - 11, 22, 22);
	mCrosshair->background_image("crosshair");

	//Energy Bar
	mEnergyLayer = mHUD->createLayer(13);
	mEnergyValueBackground = mEnergyLayer->createRectangle((Ogre::Real)mViewport->getActualWidth()/2-50, (Ogre::Real)mViewport->getActualHeight()/2+40, 100, 10);
	mEnergyValueBackground->background_colour(Gorilla::rgb(255, 255, 255, 100));

	mEnergyValue = mEnergyLayer->createRectangle((Ogre::Real)mViewport->getActualWidth()/2-50, (Ogre::Real)mViewport->getActualHeight()/2+40, 100, 10);
	mEnergyValue->background_colour(Gorilla::rgb(0, 0, 255, 150));
	
	// points for both players
	mPlayerPoints = mHUDLayer->createCaption(24, 0, 0, "0");
	mPlayerPoints->width((Ogre::Real)mViewport->getActualWidth()-36);
	mPlayerPoints->height(40);
	mPlayerPoints->align(Gorilla::TextAlign_Right);
	mPlayerPoints->colour(Gorilla::rgb(0, 255, 0, 100));
	mPlayerPoints->vertical_align(Gorilla::VerticalAlign_Bottom);

	mEnemyPoints = mHUDLayer->createCaption(24, 0, 0, "0");
 	mEnemyPoints->width((Ogre::Real)mViewport->getActualWidth()-36);
 	mEnemyPoints->height(80);
	mEnemyPoints->align(Gorilla::TextAlign_Right);
	mEnemyPoints->vertical_align(Gorilla::VerticalAlign_Bottom);
	mEnemyPoints->colour(Gorilla::rgb(255, 0, 0, 100));

	//restartLayer
	mRestartLayer = mHUD->createLayer(14);
	mRestart = mRestartLayer->createCaption(24, (Ogre::Real)mViewport->getActualWidth()/2+220, (Ogre::Real)mViewport->getActualHeight()/2+20, "Respawning in 3 seconds...");
	mRestart->align(Gorilla::TextAlign_Right);
	mRestart->vertical_align(Gorilla::VerticalAlign_Bottom);
	mRestart->colour(Gorilla::rgb(255, 220, 220, 255));
	mRestartLayer->hide();
	restartLayerBackground = mRestartLayer->createRectangle(0, 0, (Ogre::Real)mViewport->getActualWidth(), (Ogre::Real)mViewport->getActualHeight());
	restartLayerBackground->background_colour(Gorilla::rgb(0, 0, 0, 200));

	

	//steering left&right visual cues
	mSteeringLeftLayer = mHUD->createLayer(13);
	mSteeringLeft = mSteeringLeftLayer->createRectangle(20, (Ogre::Real)mViewport->getActualHeight()/2-40, 80, 80);
	mSteeringLeft->background_image("arrowleft");
	mSteeringLeft->background_colour(Gorilla::rgb(0, 0, 0, 0));
	setLeftArrowAlpha(0.0);

	mSteeringRightLayer = mHUD->createLayer(13);
	mSteeringRight = mSteeringRightLayer->createRectangle((Ogre::Real)mViewport->getActualWidth()-100, (Ogre::Real)mViewport->getActualHeight()/2-40, 80, 80);
	mSteeringRight->background_image("arrowright");
	mSteeringRight->background_colour(Gorilla::rgb(0, 0, 0, 0));
	setRightArrowAlpha(0.0);

}

ClientHUDGame::~ClientHUDGame(void)
{
	mSilverback->destroyScreen(mHUD);
}

void ClientHUDGame::update()
{
	updateAlphaLayers();
	updateEnergy(mObjectManager->getOwnPlayerEnergy());
	setPlayerPoints(mObjectManager->getOwnPlayerPoints());
	setEnemyPoints(mObjectManager->getEnemyPlayerPoints());
}

void ClientHUDGame::showRestartWith(float currentTimerCount)
{
	if(currentTimerCount > 0)
	{
		mCrosshairLayer->hide();
		mEnergyLayer->hide();
		mRestartLayer->show();

		Ogre::String remainingTime = "spawning in ";
		remainingTime+=toString((int)(currentTimerCount+1));
		remainingTime+=" seconds...";
		mRestart->text(remainingTime);
		setRestartLayerAlpha(currentTimerCount - ((int)(currentTimerCount)));
	}
	else
	{
		mEnergyLayer->show();
		mCrosshairLayer->show();
		mRestartLayer->hide();
	}
	

}

void ClientHUDGame::setLeftArrowAlpha(Ogre::Real alpha)
{
	mSteeringLeftAlpha = alpha;
	int iAlpha = (int)(mSteeringLeftAlpha*255);
	clampAlpha(&iAlpha);
	mSteeringLeft->background_colour(Gorilla::rgb(0, 0, 0, iAlpha));
}

void ClientHUDGame::setRightArrowAlpha(Ogre::Real alpha)
{
	mSteeringRightAlpha = alpha;
	int iAlpha = (int)(mSteeringRightAlpha*255);
	clampAlpha(&iAlpha);
	mSteeringRight->background_colour(Gorilla::rgb(0, 0, 0, iAlpha));
}

void ClientHUDGame::setRestartLayerAlpha(Ogre::Real alpha)
{
	mRestartLayerAlpha = alpha;
	int iAlpha = (int)(mRestartLayerAlpha*255);
	clampAlpha(&iAlpha);
	mRestart->colour(Gorilla::rgb(255, 0, 0, iAlpha));
}

Ogre::Real ClientHUDGame::getLeftArrowAlpha()
{
	return mSteeringLeftAlpha;
}

Ogre::Real ClientHUDGame::getRightArrowAlpha()
{
	return mSteeringRightAlpha;
}

Ogre::Real ClientHUDGame::getRestartLayerAlpha()
{
	return mRestartLayerAlpha;
}

void ClientHUDGame::updateEnergy(Ogre::Real value)
{
	value = Ogre::Math::Clamp<Ogre::Real>(value, 0, 100);
	mEnergyValue->width(value);
}

void ClientHUDGame::setPlayerPoints( Ogre::Real points )
{
	mPlayerPoints->text(toString((unsigned int)points));
}

void ClientHUDGame::setEnemyPoints( Ogre::Real points )
{
	mEnemyPoints->text(toString((unsigned int)points));
}

void ClientHUDGame::updateAlphaLayers()
{
	Ogre::Real alphaModifier = 0.1;
	setLeftArrowAlpha(getLeftArrowAlpha()-alphaModifier);
	setRightArrowAlpha(getRightArrowAlpha()-alphaModifier);
	setRestartLayerAlpha(getRestartLayerAlpha()-alphaModifier);
}

void ClientHUDGame::clampAlpha( int* alpha )
{
	if (*alpha <0)
	{
		*alpha = 0;
	}
	else if(*alpha > 230)
	{
		*alpha = 230;
	}
}

