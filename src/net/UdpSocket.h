//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	UdpSocket
//	Wrapper Class to use UDP Sockets
//
//*********************************************************

#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include "Address.h"
#include <assert.h>
#include <winsock.h>

namespace net
{
	class UdpSocket
	{
	public:
	
		UdpSocket();
		~UdpSocket();
	
		bool open( unsigned short port );
		void close();
		bool isOpen() const;
		unsigned short getPort();
		bool send( const Address & destination, const void * data, int size);
		int receive( Address & sender, void * data, int size );
		
	private:
		int mSocket;
		unsigned short mPort;
		
	};
}

#endif
