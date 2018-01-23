//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Net
//	Contains all necessary classes and methods
//  that are needed for network communication
//
//*********************************************************

#ifndef NET_H
#define NET_H

#include "Address.h"
#include "UdpSocket.h"
#include "UdpConnection.h"
#include "MulticastSender.h"
#include "MulticastReceiver.h"

namespace net
{
    inline bool initializeSockets()
    {
    	
        WSADATA WsaData;
	    return WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
    	
    }

    inline void shutdownSockets()
    {
	    WSACleanup();
    }
}

#endif
