#include "MulticastReceiver.h"


using namespace net;

MulticastReceiver::MulticastReceiver()
{
	mSocket = 0;
}

MulticastReceiver::~MulticastReceiver()
{
	close();
}

void MulticastReceiver::close()
{
	if ( mSocket != 0 )
	{
		closesocket( mSocket );
		mSocket = 0;
	}
}


bool MulticastReceiver::join( const Address & groupaddress)
{
    mSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSocket < 0)
    {
        return false;
    }
    int reuse=1;

    if (    setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR,
            (char *)&reuse, sizeof(reuse)) < 0)
    {
        close();
        return false;
    }
    
    memset((char *) &mLocalSock, 0, sizeof(mLocalSock));
    mLocalSock.sin_family = AF_INET;
    mLocalSock.sin_port = htons( groupaddress.getPort() );
    mLocalSock.sin_addr.s_addr  = INADDR_ANY;

    if (bind(mSocket, (struct sockaddr*)&mLocalSock, sizeof(mLocalSock)))
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

    mGroup.imr_multiaddr.s_addr = htonl( groupaddress.getAddress() );
    mGroup.imr_interface.s_addr = INADDR_ANY;
    
    if (    setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
            (char *)&mGroup, sizeof(mGroup)) < 0)
    {
        close();
        return false;
    }
    
    return true;
}

int MulticastReceiver::receive( Address & sender, void * data, int size )
{
	assert( data );
	assert( size > 0 );

	if ( mSocket == 0 )
		return false;
	
	typedef int socklen_t;
	
	sockaddr_in from;
	socklen_t fromLength = sizeof( from );

	int received_bytes = 
	    recvfrom( mSocket, (char*)data, size, 0, (sockaddr*)&from, &fromLength );

	if ( received_bytes <= 0 )
		return 0;

	unsigned int address = ntohl( from.sin_addr.s_addr );
	unsigned int port = ntohs( from.sin_port );

	sender = Address( address, port );

	return received_bytes;
}
