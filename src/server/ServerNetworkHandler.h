//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ServerNetworkHandler
//	Handles the connections to the clients
//
//*********************************************************

#ifndef SERVER_NETWORK_HANDLER_H
#define SERVER_NETWORK_HANDLER_H

#include "..\net\Net.h"
#include "ServerCalculations.h"

using namespace std;
using namespace net;

class ServerNetworkHandler
{
public:
	ServerNetworkHandler(ServerCalculations *calculations);
	~ServerNetworkHandler(void);
	
    bool start( unsigned short port );
    void stop();
    void setMaxConnectionCount(unsigned short count);
    
	void handleConnection();

private:
    void processReadReadyOnSocket();
    void processReadReadyOnMulticastReceiver();
    void updateConnections();
    void handleObjectDeleting();
    void handleNewData();
    void sendCalculatedData();

    void send(string data);

    unsigned short getConnectionCount();

    void clear();
    UdpConnection* getConnection( const Address &address );

    ServerCalculations *mCalculations;
	
	UdpSocket mSocket;
    MulticastReceiver mMulticastReceiver;
    
    map<unsigned int, UdpConnection*> mConnections;
    
    unsigned short mMaxConnectionCount;
    
    bool mIsRunning;
    
    PrecisionTimer *updateTimer;
    
};
#endif