#include "UdpServer.h"

using namespace net;

// DEBUG
#include <iostream>


UdpServer::UdpServer()
{
    mIsRunning = false;
    mMaxConnectionCount = 2;
    mIdCounter = 1;
}

UdpServer::~UdpServer()
{
    stop();
}


bool UdpServer::start(unsigned short port)
{
    if (mIsRunning)
        return false;
    
    if ( !mSocket.open(port) )
        return false;
    
    
    if ( !mMulticastReceiver.join( Address(225,1,1,1,1337) ))
        return false;
    
    
    mIsRunning = true;
    return true;
}

void UdpServer::stop()
{
    if (mIsRunning)
    {
        mSocket.close();
        mIsRunning = false;
        clear();
    }
}

void UdpServer::update()
{
    processReadReadyOnSocket();
    processReadReadyOnMulticastReceiver();
    
    vector<unsigned int> keys;
    
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
        keys.push_back((*it).first);
    
    for (unsigned int i = 0; i < keys.size(); i++)
    {
        mConnections[keys[i]]->update();
        if (mConnections[keys[i]]->isDisconnected() || mConnections[keys[i]]->hasConnectFailed())
        {
            UdpConnection *tempConnection = mConnections[keys[i]];
            mConnections.erase(keys[i]);
            delete tempConnection;
        }
    }
}

bool UdpServer::sendTo(const Address &address, string data)
{
    if (!mIsRunning)
        return false;
    
    return mSocket.send(address, data.c_str(), data.size());
}

bool UdpServer::send(string data)
{
    if (!mIsRunning)
        return false;
    
    if (mConnections.size() == 0)
        return false;
    
    bool returnValue = true;
    
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        if (!(*it).second->isReady())
            continue;
        
        if (!(*it).second->sendData(data))
            returnValue = false;
    }
    
    return returnValue;
}

void UdpServer::processReadReadyOnSocket()
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
        
        UdpConnection *currentConnection = getConnection(sender);
        if (currentConnection == 0 && mConnections.size() < mMaxConnectionCount)
        {
            UdpConnection *newConnection = new UdpConnection(&mSocket, sender);
            
            while(!mConnections.insert(
                pair<unsigned int, UdpConnection*>(mIdCounter, newConnection)).second)
            {
                mIdCounter++;
            }
            
            newConnection->setId(mIdCounter);
            mIdCounter++;
            
            newConnection->handleIncomming(dataString);
        }
        else
        {
            currentConnection->handleIncomming(dataString);
        }
        
        memset(data, 0, 1024);
        receivedBytes = mSocket.receive(sender, data, size);
    }
}

void UdpServer::processReadReadyOnMulticastReceiver()
{
    int receivedBytes = 0;
    
    Address sender;
    char data[1024];
    
    string dataString = data;
    int size = 1024;
    
    memset(data, 0, 1024);
    receivedBytes = mMulticastReceiver.receive(sender, data, size);
    
    while (receivedBytes > 0)
    {
        dataString = data;
        Address responseAddress(sender.getAddress(), atoi(dataString.c_str()));
        string responseString = "I_AM_A_SERVER";
        mSocket.send(responseAddress, responseString.c_str(), responseString.size());
        
        memset(data, 0, 1024);
        receivedBytes = mMulticastReceiver.receive(sender, data, size);
    }
}



int UdpServer::getConnectionCount()
{
    int counter = 0;
    
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        if ((*it).second->isReady())
            counter++;
    }
    return counter;
}

void UdpServer::setMaxConnectionCount(unsigned short count)
{
    mMaxConnectionCount = count;
}

multimap<unsigned int, string> UdpServer::getData()
{
    
    multimap<unsigned int, string> returnMap;
    
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        string currentData;
        while (true)
        {
            currentData = (*it).second->getData();
            if (currentData.size() > 0)
                returnMap.insert(pair<unsigned int, string>((*it).second->getId(), currentData));
            else
                break;
        } 
    }
    
    return returnMap;
}

void UdpServer::clear()
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        delete (*it).second;
    }
    mConnections.clear();
    
}

UdpConnection* UdpServer::getConnection(const Address &address)
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        if ((*it).second->getAddress() == address)
            return (*it).second;
    }
    
    return 0;
}
