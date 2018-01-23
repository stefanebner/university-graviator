#include "Address.h"

#include "..\GraviatorSimpleTypes.h"
#include <assert.h>

using namespace net;

Address::Address()
{
	mAddress = 0;
	mPort = 0;
}

Address::Address(   unsigned char a,
                    unsigned char b,
                    unsigned char c,
                    unsigned char d,
                    unsigned short port )
{
	mAddress = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
	mPort = port;
}

Address::Address( unsigned int address, unsigned short port )
{
	mAddress = address;
	mPort = port;
}

Address::Address(string address)
{
    unsigned char a = atoi(address.substr(0, address.find('.')).c_str());
    address = address.substr(address.find('.')+1);
    
    unsigned char b = atoi(address.substr(0, address.find('.')).c_str());
    address = address.substr(address.find('.')+1);
    
    unsigned char c = atoi(address.substr(0, address.find('.')).c_str());
    address = address.substr(address.find('.')+1);
    
    unsigned char d = atoi(address.substr(0, address.find('.')).c_str());
    address = address.substr(address.find(':')+1);
    
    unsigned short port = atoi(address.c_str());
    
    mAddress = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
	mPort = port;
}

unsigned int Address::getAddress() const
{
    return mAddress;
}

string Address::getAddressString()
{
    return  toString((int)getA()) + "." +
            toString((int)getB()) + "." +
            toString((int)getC()) + "." +
            toString((int)getD()) + ":" +
            toString(getPort());
}

unsigned char Address::getA() const

{
	return ( unsigned char ) ( mAddress >> 24 );
}

unsigned char Address::getB() const
{
	return ( unsigned char ) ( mAddress >> 16 );
}

unsigned char Address::getC() const
{
	return ( unsigned char ) ( mAddress >> 8 );
}

unsigned char Address::getD() const
{
	return ( unsigned char ) ( mAddress );
}

unsigned short Address::getPort() const
{ 
	return mPort;
}

bool Address::operator == ( const Address & other ) const
{
	return mAddress == other.mAddress && mPort == other.mPort;
}

bool Address::operator != ( const Address & other ) const
{
	return ! ( *this == other );
}
