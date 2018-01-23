//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDResources
//	Manages the silverback for the ingame HUDs, so that ih has to be loaded just once. Singleton.
//
//*********************************************************

#ifndef CLIENT_HUD_RESOURCES
#define CLIENT_HUD_RESOURCES

#include "..\..\Gorilla.h"

#define HUD ClientHUDResources::getInstance()

class ClientHUDResources
{
public:

	static ClientHUDResources& getInstance();
	static void destroy();

	Gorilla::Silverback *getSilverback() {return mSilverback;}
	
private:
	static ClientHUDResources* instance;
	ClientHUDResources();
	ClientHUDResources(const ClientHUDResources&) {}
	~ClientHUDResources(void);

	//HUD related
	Gorilla::Silverback *mSilverback;

};

#endif