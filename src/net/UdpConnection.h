//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	UdpConnection
//	simulates a virtual connection over UDP
//
//*********************************************************

#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H


#include "Address.h"
#include "UdpSocket.h"
#include "..\Timer.h"
#include <string>
#include <queue>

// lifeproof_interval defines the time
// between 2 greetings or pings (in sec)
#define DEFAULT_LIFEPROOF_INTERVAL 1.0f
#define MINIMAL_LIFEPROOF_INTERVAL 0.5f
#define MAXIMAL_LIFEPROOF_INTERVAL 5.0f

// defines when connecting is aborted (in sec)
#define DEFAULT_CONNECT_TIMEOUT 5.0f
#define MINIMAL_CONNECT_TIMEOUT 2.0f
#define MAXIMAL_CONNECT_TIMEOUT 20.0f

// defines when connection is closes
// if there is no incomming data (in sec)
#define DEFAULT_DISCONNECT_TIMEOUT 10.0f
#define MINIMAL_DISCONNECT_TIMEOUT 5.0f
#define MAXIMAL_DISCONNECT_TIMEOUT 60.0f

// defines how often the connections tries to reconnect
#define DEFAULT_RECONNECT_TRIES 0
#define MINIMAL_RECONNECT_TRIES 0
#define MAXIMAL_RECONNECT_TRIES 10

using namespace std;

namespace net
{
    class UdpConnection
    {
    public:
        UdpConnection( UdpSocket *socket );
        UdpConnection( UdpSocket *socket, const Address &address );
        
        void setDisconnectTimeout(float disconnectTimeout);
        void setConnectTimeout(float connectTimeout);
        void setLifeproofInterval(float lifeproofInterval);
        void setAutoReconnect(unsigned short tries);
        
        void connect( const Address &address );
        void disconnect();
        
        void setId(unsigned int id);
        unsigned int getId();
        
        void update();
        
        bool hasConnectFailed();
        bool isDisconnected();
        bool isReady();
        
        bool sendData( const string data );
        void handleIncomming( const string data );
        
        string getData();
        queue<string> getDataQueue();
        Address getAddress();
        
    private:
        enum ConnectionState {Disconnected, WaitingForGreeting,
            ConnectFailed, ReadyForUse};
        enum DataType {Undefined, Greeting, Ping, Pong, Data};

        void init();
        void clear();
        
        void readAndRemoveHeader();

        bool sendGreeting();
        bool sendPing();
        bool sendPong();

        unsigned int mSetId;
        unsigned int mReceivedId;
        
        UdpSocket *mSocket;
        Address mConnectedAddress;
        
        string mReceiveBuffer;
        string mSendBuffer;
        string mCurrentParameter;
        
        queue<string> mDataQueue;
        
        
        ConnectionState mState;
        DataType mCurrentDataType;
        
        Timer mDisconnectTimer;
        Timer mConnectTimer;
        Timer mLifeproofTimer;
        
        float mDisconnectTimeout;
        float mConnectTimeout;
        float mLifeproofInterval;
        unsigned short mMaxReconnectTries;
        unsigned short mReconnectTries;
        
    };

}

#endif