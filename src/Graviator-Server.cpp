//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Server
//	starting Point for the server
//
//*********************************************************

#include "../src/server/ServerCalculations.h"
#include "../src/server/ServerNetworkHandler.h"
#include "Timer.h"

#include <iostream>

using namespace net;
using namespace std;

int main()
{
	ServerCalculations* gameLogic = new ServerCalculations();
	ServerNetworkHandler* server = new ServerNetworkHandler(gameLogic);
	
	server->setMaxConnectionCount(4);
	
	for (unsigned short i = 5000; i < 5010; i++)
	{
	    if (server->start(i))
	    {
	        cout << "server running on port " << i << endl;
	        server->handleConnection();
	    }
	    else
	    {
	        cout << "starting server on port " << i << " failed" << endl;
	    }
	}
	
	system("PAUSE");
	return 0;
}
