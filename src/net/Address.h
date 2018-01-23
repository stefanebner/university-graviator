//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Address
//	simplifies the useage of IP addresses
//
//*********************************************************

#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

using namespace std;

namespace net
{
    class Address
    {
    public:

	    Address();
	    Address(    unsigned char a,
	                unsigned char b,
	                unsigned char c,
	                unsigned char d,
	                unsigned short port );
	    Address( unsigned int address, unsigned short port );
	    Address( string address );

	    unsigned int getAddress() const;
	    string getAddressString();
	    

	    unsigned char getA() const;
	    unsigned char getB() const;
	    unsigned char getC() const;
	    unsigned char getD() const;
	    unsigned short getPort() const;
	    bool operator == ( const Address & other ) const;
	    bool operator != ( const Address & other ) const;

    private:
	    unsigned int mAddress;
	    unsigned short mPort;
	    
    };
}

#endif
