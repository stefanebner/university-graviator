//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	MulticastReceiver
//	used to receive data from multicast groups
//  server uses this class for listening
//  to requests from clients
//
//*********************************************************

#ifndef MULTICAST_RECEIVER_H
#define MULTICAST_RECEIVER_H


#include "Address.h"
#include <assert.h>
#include <winsock.h>

namespace net
{
	class MulticastReceiver
	{
	public:
	
		MulticastReceiver();
		~MulticastReceiver();

		void close();
		bool join( const Address & groupaddress);
		int receive( Address & sender, void * data, int size );
		
	private:
	    struct sockaddr_in    mLocalSock;
        struct ip_mreq        mGroup;
        int                   mSocket;
	};

}


#endif