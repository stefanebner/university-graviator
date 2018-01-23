//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDGame
//	Manages the ingame HUD
//
//*********************************************************

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H


#include "UdpSocket.h"
#include "UdpConnection.h"
#include "MulticastSender.h"
#include "..\GraviatorSimpleTypes.h"
#include "..\Timer.h"
#include <queue>

using namespace std;

namespace net
{
    class UdpClient
    {
    public:
        UdpClient();
        ~UdpClient();
        
        bool start( unsigned short port );
        bool isRunning();
        void connect( const Address &address );
        void connectToFirstResponingServer();
        bool isConnected();
        bool hasConnectFailed();
        bool isDisconnected();
        void disconnect();
        void stop();
        
        unsigned int getId();
        
        bool send( string data );
        void update();
        
        string getData();
        queue<string> getDataQueue();
        
    private:
        UdpSocket mSocket;
        MulticastSender mMulticastSender;
        UdpConnection mConnection;
        
        bool mIsSearchingForServer;
        
        Timer mSearchingIntervalTimer;
        Timer mSearchingTimeoutTimer;
        float mSearchingInterval;
        float mSearchingTimeout;
        
        bool mIsRunning;
        void processReadReady();
    };
}

#endif