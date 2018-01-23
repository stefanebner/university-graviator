#include "UdpSocket.h"


using namespace net;
	
UdpSocket::UdpSocket()
{
	mSocket = 0;
	mPort = 0;
}

UdpSocket::~UdpSocket()
{
	close();
}

bool UdpSocket::open( unsigned short port )
{
	assert( !isOpen() );

	// create socket
	mSocket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if ( mSocket <= 0 )
	{
	    printf( "failed to create socket\n" );
		mSocket = 0;
		return false;
	}

	// bind to port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( (unsigned short) port );
    
	if ( bind( mSocket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
	{
		close();
		return false;
	}

	// set non-blocking io
	DWORD nonBlocking = 1;
	if ( ioctlsocket( mSocket, FIONBIO, &nonBlocking ) != 0 )
	{
		close();
		return false;
	}
	
    mPort = port;
    
	return true;
}

void UdpSocket::close()
{
	if ( mSocket != 0 )
	{
		closesocket( mSocket );
		mSocket = 0;
	}
}

bool UdpSocket::isOpen() const
{
	return mSocket != 0;
}

unsigned short UdpSocket::getPort()
{
    return mPort;
}

bool UdpSocket::send( const Address & destination, const void * data, int size)
{
	assert( data );
	assert( size > 0 );

	if ( mSocket == 0 )
		return false;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( destination.getAddress() );
	address.sin_port = htons( (unsigned short) destination.getPort() );

	int sent_bytes = sendto(    mSocket,
	                            (const char*)data,
	                            size,
	                            0,
	                            (sockaddr*)&address,
	                            sizeof(sockaddr_in) );

	return sent_bytes == size;
}

int UdpSocket::receive( Address & sender, void * data, int size )
{
	assert( data );
	assert( size > 0 );

	if ( mSocket == 0 )
		return false;
	
	typedef int socklen_t;
	
	sockaddr_in from;
	socklen_t fromLength = sizeof( from );

	int received_bytes = recvfrom( mSocket, (char*)data, size, 0, (sockaddr*)&from, &fromLength );

	if ( received_bytes <= 0 )
		return 0;
    
	unsigned int address = ntohl( from.sin_addr.s_addr );
	unsigned int port = ntohs( from.sin_port );

	sender = Address( address, port );

	return received_bytes;
}
