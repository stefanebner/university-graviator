#include "UdpConnection.h"
#include "..\GraviatorSimpleTypes.h"


using namespace net;

UdpConnection::UdpConnection(net::UdpSocket *socket)
{
    init();
    mSocket = socket;
}

UdpConnection::UdpConnection(net::UdpSocket *socket, const net::Address &address)
{
    init();
    mSocket = socket;
    connect(address);
}

void UdpConnection::setDisconnectTimeout(float disconnectTimeout)
{
    if (disconnectTimeout > MAXIMAL_DISCONNECT_TIMEOUT)
        mDisconnectTimeout = MAXIMAL_DISCONNECT_TIMEOUT;
    else if (disconnectTimeout < MINIMAL_DISCONNECT_TIMEOUT)
        mDisconnectTimeout = MINIMAL_DISCONNECT_TIMEOUT;
    else
        mDisconnectTimeout = disconnectTimeout;
        
}

void UdpConnection::setConnectTimeout(float connectTimeout)
{
    if (connectTimeout > MAXIMAL_CONNECT_TIMEOUT)
        mConnectTimeout = MAXIMAL_CONNECT_TIMEOUT;
    else if (connectTimeout < MINIMAL_CONNECT_TIMEOUT)
        mConnectTimeout = MINIMAL_CONNECT_TIMEOUT;
    else
        mConnectTimeout = connectTimeout;
}    

void UdpConnection::setLifeproofInterval(float lifeproofInterval)
{
    if (lifeproofInterval > MAXIMAL_LIFEPROOF_INTERVAL)
        mLifeproofInterval = MAXIMAL_LIFEPROOF_INTERVAL;
    else if (lifeproofInterval < MINIMAL_LIFEPROOF_INTERVAL)
        mLifeproofInterval = MINIMAL_LIFEPROOF_INTERVAL;
    else
        mLifeproofInterval = lifeproofInterval;
}

void UdpConnection::setAutoReconnect(unsigned short tries)
{
    if (tries > MAXIMAL_RECONNECT_TRIES)
        mMaxReconnectTries = MAXIMAL_RECONNECT_TRIES;
    else if (tries < MINIMAL_RECONNECT_TRIES)
        mMaxReconnectTries = MINIMAL_RECONNECT_TRIES;
    else
        mMaxReconnectTries = tries;
}


void UdpConnection::connect(const net::Address &address)
{
    mConnectedAddress = address;
    mState = WaitingForGreeting;
    
    mConnectTimer.restart();
    mLifeproofTimer.restart();
    mDisconnectTimer.reset();
    
    if (!sendGreeting())
    {
        clear();
        mState = ConnectFailed;
    }
}

void UdpConnection::disconnect()
{
    clear();
    mState = Disconnected;
}

void UdpConnection::setId(unsigned int id)
{
    mSetId = id;
}

unsigned int UdpConnection::getId()
{
    if (mSetId > 0)
        return mSetId;
    else
        return mReceivedId;
}

void UdpConnection::update()
{

    switch (mState)
    {
        case WaitingForGreeting:
        {
            if (mConnectTimer.getTime() > mConnectTimeout)
            {
                if (mReconnectTries < mMaxReconnectTries)
                {
                    mReconnectTries++;
                    connect(mConnectedAddress);
                }
                else
                {
                    clear();
                    mState = ConnectFailed;
                }
            }
            else if (mLifeproofTimer.getTime() > mLifeproofInterval)
            {
                sendGreeting();
                mLifeproofTimer.restart();
            }
            break;
        }
        case ReadyForUse:
        {
            if (mSendBuffer.size() > 0)
            {
                if(mSocket->send(mConnectedAddress, mSendBuffer.c_str(), mSendBuffer.size()))
                    mSendBuffer.clear();
            }
            
            if (mDisconnectTimer.getTime() > mDisconnectTimeout)
            {
                if (mReconnectTries < mMaxReconnectTries)
                {
                    mReconnectTries++;
                    connect(mConnectedAddress);
                }
                else
                {
                    disconnect();
                }
            }
            else if (mLifeproofTimer.getTime() > mLifeproofInterval)
            {
                sendPing();
                mLifeproofTimer.restart();
            }
        }
    }
}

bool UdpConnection::hasConnectFailed()
{
    return mState == ConnectFailed;
}

bool UdpConnection::isDisconnected()
{
    return mState == Disconnected;
}

bool UdpConnection::isReady()
{
    return mState == ReadyForUse;
}

bool UdpConnection::sendData(const string data)
{
    if (mState != ReadyForUse)
        return false;
    
    if (data.size() == 0)
        return false;
    
    string stringToSend = "DATA " + data;
    
    // ensure that there is a ; at the end of string
    if (stringToSend.find_last_of(';') != stringToSend.at(stringToSend.size()-1))
        stringToSend += ";";

    mSendBuffer += stringToSend;
    return true;
}

void UdpConnection::handleIncomming( const string data )
{
    mReceiveBuffer += data;
    do
    {
        if (mReceiveBuffer.size() <= 0)
            break;
            
        mDisconnectTimer.restart();
        mReconnectTries = 0;
        readAndRemoveHeader();
        
        switch(mCurrentDataType)
        {
            case Greeting:
                if (mState == WaitingForGreeting)
                    mState = ReadyForUse;
                else if (mState == ReadyForUse)
                    sendGreeting();
                break;
            case Ping:
                sendPong();
                break;
            case Pong:
                mReceivedId = atoi(mCurrentParameter.c_str());
                break;
            case Data:
                mDataQueue.push(mCurrentParameter);
                break;
            case Undefined:
            default:
                break;
        }
    } while (mCurrentDataType != Undefined);
}

string UdpConnection::getData()
{
    string returnValue;
    if (!mDataQueue.empty())
    {
        returnValue = mDataQueue.front();
        mDataQueue.pop();
    }
    return returnValue;
}

queue<string> UdpConnection::getDataQueue()
{
    return mDataQueue;
}


Address UdpConnection::getAddress()
{
    return mConnectedAddress;
}

void UdpConnection::init()
{
    mDisconnectTimeout = DEFAULT_DISCONNECT_TIMEOUT;
    mConnectTimeout = DEFAULT_CONNECT_TIMEOUT;
    mLifeproofInterval = DEFAULT_LIFEPROOF_INTERVAL;
    mMaxReconnectTries = DEFAULT_RECONNECT_TRIES;
    mReconnectTries = 0;
    mConnectedAddress = Address();
    mState = Disconnected;
    mCurrentDataType = Undefined;
    mSetId = 0;
    mReceivedId = 0;
}

void UdpConnection::clear()
{
    while (!mDataQueue.empty())
        mDataQueue.pop();

    mConnectedAddress = Address();
    mCurrentDataType = Undefined;
    mReconnectTries = 0;

    mDisconnectTimer.reset();
    mConnectTimer.reset();
    mLifeproofTimer.reset();
}


void UdpConnection::readAndRemoveHeader()
{
    size_t found = mReceiveBuffer.find(' ');
    string header = mReceiveBuffer.substr(0, found);
    
    if (header == "GREETING")
        mCurrentDataType = Greeting;
    else if (header == "PING")
        mCurrentDataType = Ping;
    else if (header == "PONG")
        mCurrentDataType = Pong;
    else if (header == "DATA")
        mCurrentDataType = Data;
    else
        mCurrentDataType = Undefined;
    
    size_t foundEnd = mReceiveBuffer.find(';');
    mCurrentParameter = mReceiveBuffer.substr(found+1, foundEnd-found-1);
    mReceiveBuffer = mReceiveBuffer.substr(foundEnd+1);
}


bool UdpConnection::sendGreeting()
{
    string stringToSend = "GREETING ;";
    return mSocket->send(mConnectedAddress, stringToSend.c_str(), stringToSend.size());
}


bool UdpConnection::sendPing()
{
    if (mState != ReadyForUse)
        return false;
    
    string stringToSend = "PING ;";
    return mSocket->send(mConnectedAddress, stringToSend.c_str(), stringToSend.size());
}

bool UdpConnection::sendPong()
{
    if (mState != ReadyForUse)
        return false;
        
    string stringToSend = "PONG " + toString(getId()) + ";";
    return mSocket->send(mConnectedAddress, stringToSend.c_str(), stringToSend.size());
}
