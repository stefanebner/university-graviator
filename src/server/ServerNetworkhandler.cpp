#include "ServerNetworkHandler.h"

#include "..\GraviatorSimpleTypes.h"
#include "..\Utility.h"

#include <iostream>
using namespace std;

#define BUFFER_SIZE 256

ServerNetworkHandler::ServerNetworkHandler(ServerCalculations *calculations)
:   mCalculations(calculations)
{
    mIsRunning = false;
    mMaxConnectionCount = 2;
    initializeSockets();
}

ServerNetworkHandler::~ServerNetworkHandler(void)
{
    stop();
    shutdownSockets();
}

bool ServerNetworkHandler::start(unsigned short port)
{
    if (mIsRunning)
        return false;
    
    if ( !mSocket.open(port) )
        return false;
    
    if ( !mMulticastReceiver.join( Address(225,1,1,1,1337) ))
    {
        mSocket.close();
        return false;
    }
    
    mIsRunning = true;
    return true;
}

void ServerNetworkHandler::stop()
{
    if (mIsRunning)
    {
        mSocket.close();
        mMulticastReceiver.close();
        mIsRunning = false;
        clear();
    }
}

void ServerNetworkHandler::setMaxConnectionCount(unsigned short count)
{
    mMaxConnectionCount = count;
}

void ServerNetworkHandler::handleConnection()
{ 
	while(mIsRunning)
	{
        //Sleep(25); MOVED SLEEP TO updateConnections()
        processReadReadyOnSocket();
        processReadReadyOnMulticastReceiver();
        updateConnections();
        
        // if server has no connections no data is available
        if (getConnectionCount() == 0)
                continue;
        
        handleObjectDeleting();
        handleNewData();
        mCalculations->runCalculations();
        sendCalculatedData();
	}

	
}

void ServerNetworkHandler::processReadReadyOnSocket()
{
    // live value from lua
    mMaxConnectionCount = (unsigned int)LUA.getInt("maxConnectionCount");
    
    int receivedBytes = 0;
    
    Address sender;
    char buffer[BUFFER_SIZE];
    
    string dataString = buffer;
    int size = BUFFER_SIZE;
    
    memset(buffer, 0, BUFFER_SIZE);
    receivedBytes = mSocket.receive(sender, buffer, size);
    
    while (receivedBytes > 0)
    {
        dataString = buffer;
        
        UdpConnection *currentConnection = getConnection(sender);
        if (currentConnection == 0)
        {
            if (mConnections.size() < mMaxConnectionCount)
            {
                UdpConnection *newConnection = new UdpConnection(&mSocket, sender);

                unsigned int id = mCalculations->createPlayer();

                while(!mConnections.insert(
                    pair<unsigned int, UdpConnection*>(id, newConnection)).second)
                {
                    unsigned int invalidId = id;
                    id = mCalculations->createPlayer();
                    mCalculations->deletePlayer(invalidId);
                }

                newConnection->setId(id);
                newConnection->handleIncomming(dataString);

                cout << "new connection. id: " << id << endl;
            }
        }
        else
        {
            currentConnection->handleIncomming(dataString);
        }
        
        memset(buffer, 0, BUFFER_SIZE);
        receivedBytes = mSocket.receive(sender, buffer, size);
    }
}

void ServerNetworkHandler::processReadReadyOnMulticastReceiver()
{
    int receivedBytes = 0;
    
    Address sender;
    char buffer[BUFFER_SIZE];
    
    string dataString = buffer;
    int size = BUFFER_SIZE;
    
    memset(buffer, 0, BUFFER_SIZE);
    receivedBytes = mMulticastReceiver.receive(sender, buffer, size);
    
    while (receivedBytes > 0)
    {
        dataString = buffer;
        Address responseAddress(sender.getAddress(), atoi(dataString.c_str()));
        string responseString = "I_AM_A_SERVER";
        mSocket.send(responseAddress, responseString.c_str(), responseString.size());
        
        memset(buffer, 0, BUFFER_SIZE);
        receivedBytes = mMulticastReceiver.receive(sender, buffer, size);
    }
}

void ServerNetworkHandler::updateConnections()
{
    vector<unsigned int> keys;
    
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
        keys.push_back((*it).first);
    
    if (keys.size() == 0)
    {
        Sleep(25);
        return;
    }
    
    for (unsigned int i = 0; i < keys.size(); i++)
    {
        // avoid sending new data at same time
        // to reduce package lost
        Sleep(25/keys.size());
        
        mConnections[keys[i]]->update();
        if (mConnections[keys[i]]->isDisconnected() || mConnections[keys[i]]->hasConnectFailed())
        {
            assert(mConnections[keys[i]]->getId() == keys[i]);
            
            mCalculations->deletePlayer(keys[i]);
            
            UdpConnection *tempConnection = mConnections[keys[i]];
            mConnections.erase(keys[i]);
            delete tempConnection;
            cout << "lost connection. id: " << keys[i] << endl;
        }
    }
}

void ServerNetworkHandler::handleObjectDeleting()
{
    vector<unsigned int> keys;
    map<unsigned int, ServerGravitationalObject*>::iterator it;

    for (   it =  mCalculations->mCurrentObjects.begin();
        it != mCalculations->mCurrentObjects.end();
        it++)
    {
        keys.push_back((*it).first);
    }

    for (unsigned int i = 0; i < keys.size(); i++)
    {
        ServerGravitationalObject *current = mCalculations->mCurrentObjects[keys[i]];
        string stringToSend;
        if (current->shouldBeKilled())
        {
            stringToSend = toString(keys[i]) + ',';
            stringToSend += toString('d') + ',';
            // position not relevant for deleting -> can be set to 1,1,1
            stringToSend += "1,1,1";
            // alignment not relevant -> can be set to 1,1,1
            stringToSend += "1,1,1";
            mCalculations->deleteObject(keys[i]);
            send(stringToSend);
        }
    }
}

void ServerNetworkHandler::handleNewData()
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        string dataString;
        while (true)
        {
            dataString = (*it).second->getData();
            if (dataString.size() > 0)
            {
                bool left = (dataString.substr(0, dataString.find(',')) == "1" ? true : false);
                dataString = dataString.substr(dataString.find(',')+1);
                
                bool right = (dataString.substr(0, dataString.find(',')) == "1" ? true : false);
                dataString = dataString.substr(dataString.find(',')+1);
                
                bool fire = (dataString.substr(0, dataString.find(',')) == "1" ? true : false);
                dataString = dataString.substr(dataString.find(',')+1);
                
                vec3f camera;
                camera.x = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
                dataString = dataString.substr(dataString.find(',')+1);
                
                camera.y = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
                dataString = dataString.substr(dataString.find(',')+1);
                
                camera.z = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
                dataString = dataString.substr(dataString.find(',')+1);
                
                mCalculations->handleInput(camera, fire, left, right, (*it).first);
            }
            else
            {
                break;
            }
        } 
    }
}

void ServerNetworkHandler::sendCalculatedData()
{
    string stringToSend;
    
	map<unsigned int, ServerPlayer*>::iterator itPlayers = mCalculations->mCurrentPlayers.begin();

	for(itPlayers; itPlayers != mCalculations->mCurrentPlayers.end(); itPlayers++)
    {
        stringToSend = toString(itPlayers->second->getID()) + ',';
        stringToSend += toString(itPlayers->second->getType()) + ',';
        vec3f position = setYtoArenaHeight(itPlayers->second->getPosition());
        
        stringToSend += toString(position.x) + ',';
        stringToSend += toString(position.y) + ',';
        stringToSend += toString(position.z) + ',';
        
        if (position.isZero())
        {
            stringToSend += toString(itPlayers->second->getRespawnTime()) + ',';
            // last 2 floats not needed
            stringToSend += "1,1";
        }
        else
        {
            vec3f orientation = itPlayers->second->getOrientation();
            stringToSend += toString(orientation.x) + ',';
            stringToSend += toString(orientation.y) + ',';
            stringToSend += toString(orientation.z);
        }

        send(stringToSend);
        stringToSend.clear();  
        
        // send energy of player
        stringToSend = toString(itPlayers->second->getID()) + ',';
        stringToSend += toString('e') + ',';
        
        vec3f energy = itPlayers->second->getEnergy();
        stringToSend += toString(energy.x) + ',';
        stringToSend += toString(energy.y) + ',';
        stringToSend += toString(energy.z) + ',';
        
        // last 3 values used to send score
        stringToSend += toString(itPlayers->second->getScore()) + ',';
        stringToSend += toString(mCalculations->getNextHighestScore(itPlayers->first)) + ',';
        stringToSend += toString(mCalculations->getScoreLimit());
        
        send(stringToSend);
        stringToSend.clear();  
        
    }
    
      
    
    vector<unsigned int> keys;
    map<unsigned int, ServerGravitationalObject*>::iterator it;
    
    for (   it =  mCalculations->mCurrentObjects.begin();
            it != mCalculations->mCurrentObjects.end();
            it++)
    {
        keys.push_back((*it).first);
    }
    
    for (unsigned int i = 0; i < keys.size(); i++)
    {
        ServerGravitationalObject *current = mCalculations->mCurrentObjects[keys[i]];
        

        stringToSend = toString(keys[i]) + ',';
        stringToSend += toString(current->getType()) + ',';
        
        vec3f position = setYtoArenaHeight(current->getPosition());
        
        stringToSend += toString(position.x) + ',';
        stringToSend += toString(position.y) + ',';
        stringToSend += toString(position.z) + ',';
        // alignment not relevant -> can be set to 1,1,1
        stringToSend += "1,1,1";
        
        send(stringToSend);
    }
}

void ServerNetworkHandler::send(string data)
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        if (!(*it).second->isReady())
            continue;
        
        (*it).second->sendData(data);
    }
}

unsigned short ServerNetworkHandler::getConnectionCount()
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

UdpConnection* ServerNetworkHandler::getConnection(const net::Address &address)
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        if ((*it).second->getAddress() == address)
            return (*it).second;
    }
    
    return 0;
}


void ServerNetworkHandler::clear()
{
    map<unsigned int, UdpConnection*>::iterator it;
    for (it = mConnections.begin(); it != mConnections.end(); it++)
    {
        delete (*it).second;
    }
    mConnections.clear();
}
