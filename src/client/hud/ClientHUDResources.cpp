#include "ClientHUDResources.h"

ClientHUDResources* ClientHUDResources::instance = 0;

ClientHUDResources::ClientHUDResources()
{
	mSilverback = new Gorilla::Silverback();
	mSilverback->loadAtlas("dejavu");
}

ClientHUDResources::~ClientHUDResources(void)
{
	delete mSilverback;
}

ClientHUDResources& ClientHUDResources::getInstance()
{
   if ( !instance )
      instance = new ClientHUDResources();
   return *instance;
}
 
void ClientHUDResources::destroy()
{
   if (instance)
      delete instance;
   instance = 0;
}
