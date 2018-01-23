//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientNetworkHandler
//	Handles the connection to the server.
//
//*********************************************************

#ifndef CLIENT_NETWORK_HANDLER
#define CLIENT_NETWORK_HANDLER

#include "ClientObjectManager.h"
#include "ClientInputManager.h"
#include "..\net\Net.h"
#include "..\Timer.h"

using namespace net;
using namespace std;


class ClientNetworkHandler
{
public:
	ClientNetworkHandler(ClientObjectManager *objectManager, ClientInputManager *inputManager);
	~ClientNetworkHandler(void);
	
	bool start(unsigned int port);
	void connectToIp(const Address &address);
	void connectToFirstResponingServer();
	void stop();
	
    void handleConnection();

private:
    void processReadReady();
	void parseData();
	string parseInfoToDataString(ClientControlInfo &info);

	ClientObjectManager *mObjectManager;
	ClientInputManager *mInputMangager;
	
    UdpSocket mSocket;
    MulticastSender mMulticastSender;
    UdpConnection mConnection;

    bool mIsSearchingForServer;

    Timer mSearchingIntervalTimer;
    Timer mSearchingTimeoutTimer;
    float mSearchingInterval;
    float mSearchingTimeout;

    bool mIsRunning;
    
};

#endif