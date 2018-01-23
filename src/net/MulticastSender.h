//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	MulticastSender
//	used to send data to multicast groups
//  client uses this class to send request
//  for available servers
//
//*********************************************************

#ifndef MULTICAST_SENDER_H
#define MULTICAST_SENDER_H

#include "Address.h"
#include <assert.h>
#include <winsock.h>


namespace net
{
	class MulticastSender
	{
	public:
	
		MulticastSender();
		~MulticastSender();

		void close();
		bool open( const Address & group );
		bool send( const void * data, int size );
		
	private:
	    struct in_addr        mLocalInterface;
        struct sockaddr_in    mGroupSock;
        int                   mSocket;
	};
}

#endif