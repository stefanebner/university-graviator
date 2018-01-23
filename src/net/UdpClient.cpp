#include "UdpClient.h"

using namespace net;


UdpClient::UdpClient()
: mConnection(&mSocket)
{
    mIsRunning = false;
    mIsSearchingForServer = false;
    mSearchingTimeout = 10.0f;
    mSearchingInterval = 1.0f;
    mConnection.setAutoReconnect(10);
}

UdpClient::~UdpClient()
{
    stop();
}

bool UdpClient::start(unsigned short port)
{
    if (mIsRunning)
        return false;
    
    if ( !mSocket.open(port) )
        return false;
    
    mIsRunning = true;
    return true;
}

bool UdpClient::isRunning()
{
    return mIsRunning;
}

void UdpClient::connect( const Address &address)
{
    if (mIsRunning)
    {
        mConnection.connect(address);
    }
}

void UdpClient::connectToFirstResponingServer()
{
    if (!mIsRunning)
        return;
    
    mMulticastSender.open( Address(225,1,1,1,1337) );
    
    string portString = toString(mSocket.getPort());
    mMulticastSender.send(portString.c_str(), portString.size());
    mIsSearchingForServer = true;
    mSearchingTimeoutTimer.start();
    mSearchingIntervalTimer.start();
}

bool UdpClient::isConnected()
{
    return mConnection.isReady();
}

bool UdpClient::hasConnectFailed()
{
    return mConnection.hasConnectFailed();
}

bool UdpClient::isDisconnected()
{
    return mConnection.isDisconnected();
}

void UdpClient::disconnect()
{
    mConnection.disconnect();
}

void UdpClient::stop()
{
    if (mIsRunning)
    {
        mConnection.disconnect();
        mSocket.close();
        mIsRunning = false;
        mIsSearchingForServer = false;
    }
}

unsigned int UdpClient::getId()
{
    return mConnection.getId();
}

bool UdpClient::send(string data)
{
    if (!mIsRunning)
        return false;
    
    if (!mConnection.isReady())
        return false;
    
    return mConnection.sendData(data);
}

void UdpClient::update()
{
    processReadReady();
    mConnection.update();
    
    if (mIsSearchingForServer && mSearchingTimeoutTimer.getTime() > mSearchingTimeout)
    {
        mIsSearchingForServer = false;
        mSearchingIntervalTimer.reset();
        mSearchingTimeoutTimer.reset();
    }
    else if (mIsSearchingForServer && mSearchingIntervalTimer.getTime() > mSearchingInterval)
    {
        mSearchingIntervalTimer.restart();
        string portString = toString(mSocket.getPort());
        mMulticastSender.send(portString.c_str(), portString.size());
    }
}

string UdpClient::getData()
{
    string tempString;
    if (mConnection.isReady())
        tempString = mConnection.getData();
        
    return tempString;
}

queue<string> UdpClient::getDataQueue()
{
    queue<string> tempQueue;
    
    if (mConnection.isReady())
        tempQueue = mConnection.getDataQueue();
    
    return tempQueue;
}

void UdpClient::processReadReady()
{
    int receivedBytes = 0;
    
    Address sender;
    char data[1024];
    
    string dataString = data;
    int size = 1024;
    
    memset(data, 0, 1024);
    receivedBytes = mSocket.receive(sender, data, size);
    
    while (receivedBytes > 0)
    {
        dataString = data;
        
        if (mIsSearchingForServer)
        {
            // TODO: handle more information from server
            if (dataString == "I_AM_A_SERVER")
            {
                mIsSearchingForServer = false;
                connect(sender);
                return;
            }
        }
        else if (sender == mConnection.getAddress())
        {
            mConnection.handleIncomming(dataString);
        }
        
        memset(data, 0, 1024);
        receivedBytes = mSocket.receive(sender, data, size);
    }
}

