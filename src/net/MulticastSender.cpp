#include "MulticastSender.h"


using namespace net;

MulticastSender::MulticastSender()
{
	mSocket = 0;
}

MulticastSender::~MulticastSender()
{
	close();
}

void MulticastSender::close()
{
	if ( mSocket != 0 )
	{
		closesocket( mSocket );
		mSocket = 0;
	}
}


bool MulticastSender::open( const Address & group )
{
    mSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (mSocket < 0)
    {
        return false;
    }

    
    //Initialize the group sockaddr structure
    memset((char *) &mGroupSock, 0, sizeof(mGroupSock));
    mGroupSock.sin_family = AF_INET;
    mGroupSock.sin_addr.s_addr = htonl( group.getAddress() );
    mGroupSock.sin_port = htons( group.getPort() );
    
    
    //Disable loopback so you do not receive your own datagrams.
    char loopch=0;
    if (    setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_LOOP,
            (char *)&loopch, sizeof(loopch)) < 0)
    {
        close();
        return false;
    }
    
    // Set local interface for outbound multicast datagrams.
    // The IP address specified must be associated with a local,
    // multicast-capable interface.
    mLocalInterface.s_addr = INADDR_ANY;
    
    if (    setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_IF,
            (char *)&mLocalInterface,
            sizeof(mLocalInterface)) < 0)
    {
        close();
        return false;
    }
    
    return true;
}

bool MulticastSender::send( const void * data, int size)
{
	assert( data );
	assert( size > 0 );

	if ( mSocket == 0 )
		return false;

	int sent_bytes =
	    sendto( mSocket,
	            (const char*)data,
	            size,
	            0,
	            (sockaddr*)&mGroupSock,
	            sizeof(mGroupSock) );

	return sent_bytes == size;
}
