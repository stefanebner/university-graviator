
#include "..\GraviatorSimpleTypes.h"

ClientHUD* ClientHUD::instance = 0;

ClientHUD::ClientHUD()
{
	mSilverback = ClientHUDResources::getInstance().getSilverback();
}

ClientHUD::~ClientHUD(void)
{
}

ClientHUD& ClientHUD::getInstance()
{
   if ( !instance )
      instance = new ClientHUD();
   return *instance;
}
 
void ClientHUD::destroy()
{
   if (instance)
      delete instance;
   instance = 0;
}

void ClientHUD::createGameHUD(Ogre::Viewport* viewport)
{
	mViewport = viewport;
	mHUD = mSilverback->createScreen(mViewport, "dejavu");
	mHUDLayer = mHUD->createLayer();

	// Create our drawing layer
	mCrosshairLayer = mHUD->createLayer();
	mCrosshair = mCrosshairLayer->createRectangle((mViewport->getActualWidth() * 0.5f) - 11, 
		(mViewport->getActualHeight() * 0.5f) - 11, 22, 22);
	mCrosshair->background_image("crosshair");

	mEnergyValueBackground = mHUDLayer->createRectangle(mViewport->getActualWidth()/2-50, mViewport->getActualHeight()/2+40, 100, 10);
	mEnergyValueBackground->background_colour(Gorilla::rgb(255, 255, 255, 100));

	mEnergyValue = mHUDLayer->createRectangle(mViewport->getActualWidth()/2-50, mViewport->getActualHeight()/2+40, 100, 10);
	mEnergyValue->background_colour(Gorilla::rgb(0, 0, 255, 150));
	
	// points for both players
	mPlayerPoints = mHUDLayer->createCaption(24, 0, 0, "0");
	mPlayerPoints->width(mViewport->getActualWidth()-36);
	mPlayerPoints->height(40);
	mPlayerPoints->align(Gorilla::TextAlign_Right);
	mPlayerPoints->colour(Gorilla::rgb(0, 255, 0, 100));
	mPlayerPoints->vertical_align(Gorilla::VerticalAlign_Bottom);

	mEnemyPoints = mHUDLayer->createCaption(24, 0, 0, "0");
	mEnemyPoints->width(mViewport->getActualWidth()-36);
	mEnemyPoints->height(80);
	mEnemyPoints->align(Gorilla::TextAlign_Right);
	mEnemyPoints->vertical_align(Gorilla::VerticalAlign_Bottom);
	mEnemyPoints->colour(Gorilla::rgb(255, 0, 0, 100));

}

void ClientHUD::removeHUD()
{
	mSilverback->destroyScreen(mHUD);
}


void ClientHUD::updateEnergy(Ogre::Real value)
{
	value = Ogre::Math::Clamp<Ogre::Real>(value, 0, 100);
	mEnergyValue->width(value);
}

void ClientHUD::setPlayerPoints( Ogre::Real points )
{
	mPlayerPoints->text(toString((unsigned int)points));
}

void ClientHUD::setEnemyPoints( Ogre::Real points )
{
	
	mEnemyPoints->text(toString((unsigned int)points));
}
