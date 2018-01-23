#include "ClientHUDConsole.h"

#include "../../net/Address.h"

#pragma warning ( disable : 4244 )

#define WRITE OgreConsole::getSingleton().print


ClientHUDConsole::ClientHUDConsole(Ogre::Viewport* viewport, OIS::Keyboard* keyboard)
{
	this->init(viewport);

	mKeyboard = keyboard;
	mKeyboard->setEventCallback(mKeyListener);
	
	mScreen=mSilverback->createScreen(mViewport, "dejavu");
	mFPSLayer = mScreen->createLayer(14);
	mFPS = mFPSLayer->createCaption(14, 10, 10, Ogre::StringUtil::BLANK);
	mConsole = new OgreConsole();
	mConsole->init(mScreen);
	mConsole->addCommand("connect", &connectToServer);

	mVisibility = false;
	mConsole->setVisible(mVisibility);
}

ClientHUDConsole::~ClientHUDConsole(void)
{
	mSilverback->destroyScreen(mScreen);
}

void ClientHUDConsole::update()
{
	mConsole->setVisible(mVisibility);

	std::stringstream s;
	s << "FPS: " << "notyetdone\n";
	mFPS->top(mViewport->getActualHeight()-25);
	mFPS->text(s.str());
}

bool ClientHUDConsole::processUnbufferedInput( const Ogre::FrameEvent &evt )
{
	return true;
}

void ClientHUDConsole::toggleVisibility()
{
	if(mVisibility)
		mVisibility = false;
	else mVisibility = true;
}

bool ClientHUDConsole::getVisibility()
{
	return mVisibility;
}


void connectToServer(Ogre::StringVector& vec)
{
	OgreConsole::getSingleton().print("Connecting");

	if(vec.size()==1)
	{
		WRITE("you forgot to add an IP and Port eg: 127.000.00.1:5000");
	}
	else
	{
		Ogre::String ip = vec[1];
		net::Address* address = new net::Address(ip);
		WRITE(address->getAddressString());
	}
}