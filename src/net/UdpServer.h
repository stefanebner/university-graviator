//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientHUDGame
//	Manages the ingame HUD
//
//*********************************************************

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "Address.h"
#include "UdpSocket.h"
#include "UdpConnection.h"
#include "MulticastReceiver.h"
#include <map>
#include <vector>

using namespace std;

namespace net
{
    class UdpServer
    {
    public:
        UdpServer();
        ~UdpServer();
        
        bool start( unsigned short port );
        void stop();
        void update();
        
        bool sendTo( const Address &address, string data );
        bool send( string data );
        
        int getConnectionCount();
        void setMaxConnectionCount(unsigned short count);
        
        multimap<unsigned int, string> getData();
        
    private:
        UdpSocket mSocket;
        MulticastReceiver mMulticastReceiver;
        
        map<unsigned int, UdpConnection*> mConnections;
        unsigned int mIdCounter;
        
        unsigned short mMaxConnectionCount;
        bool mIsRunning;
        
        void clear();
        UdpConnection* getConnection( const Address &address );
        void processReadReadyOnSocket();
        void processReadReadyOnMulticastReceiver();
    };
}


#endif