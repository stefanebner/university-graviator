#include "ClientNetworkHandler.h"


#include "..\vec3f.h"

#include <string>
#include <cstdlib>

ClientNetworkHandler::ClientNetworkHandler(
    ClientObjectManager *objectManager,
    ClientInputManager *inputManager)
:
	mObjectManager(objectManager),
	mInputMangager(inputManager),
	mConnection(&mSocket)
{
    mIsRunning = false;
    mIsSearchingForServer = false;
    mSearchingTimeout = 10.0f;
    mSearchingInterval = 1.0f;
    initializeSockets();
}

ClientNetworkHandler::~ClientNetworkHandler(void)
{
    stop();
    shutdownSockets();
}

bool ClientNetworkHandler::start(unsigned int port)
{
    if (mIsRunning)
        return false;

    if ( !mSocket.open(port) )
        return false;

    mIsRunning = true;
    return true;
}

void ClientNetworkHandler::connectToIp( const Address &address )
{
    if (mIsRunning)
    {
        mConnection.connect(address);
    }
}

void ClientNetworkHandler::connectToFirstResponingServer()
{
    if (!mIsRunning)
        return;

    bool result = mMulticastSender.open( Address(225,1,1,1,1337) );

    string portString = toString(mSocket.getPort());
    mMulticastSender.send(portString.c_str(), portString.size());
    mIsSearchingForServer = true;
    mSearchingTimeoutTimer.start();
    mSearchingIntervalTimer.start();
}

void ClientNetworkHandler::stop()
{
    if (mIsRunning)
    {
        mSocket.close();
        mMulticastSender.close();
        mIsRunning = false;
    }
}



void ClientNetworkHandler::handleConnection()
{
    while(!mConnection.isDisconnected() &&  !mConnection.hasConnectFailed() || mIsSearchingForServer)
    {
        Sleep(20);
        processReadReady();
        mConnection.update();
        unsigned int id = mConnection.getId();
        
        if (id != 0)
            mObjectManager->setPlayerId(id);
        
        if (mIsSearchingForServer &&
            mSearchingTimeoutTimer.getTime() > mSearchingTimeout)
        {
            mIsSearchingForServer = false;
            mConnection.disconnect();
            mSearchingIntervalTimer.reset();
            mSearchingIntervalTimer.reset();
        }
        else if (mIsSearchingForServer &&
                 mSearchingIntervalTimer.getTime() > mSearchingInterval)
        {
            mSearchingIntervalTimer.restart();
            string portString = toString(mSocket.getPort());
            mMulticastSender.send(portString.c_str(), portString.size());
        }
        else
        {
            string dataString = parseInfoToDataString(mInputMangager->getControlInfo());
            mConnection.sendData(dataString);

            parseData();
        }
    }
    
  mObjectManager->setQuitWithConnectionFailure(true);
}

void ClientNetworkHandler::processReadReady()
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
                connectToIp(sender);
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




void ClientNetworkHandler::parseData()
{
    
    string dataString = mConnection.getData();
    
    while(dataString.size() > 0)
    {
        string debugString = dataString;
    
        int id = atoi( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        char type = dataString.substr(0, dataString.find(',')).c_str()[0];
        dataString = dataString.substr(dataString.find(',')+1);
        
        if ((type != 'p') && (type != 'e') && (type != 'g') && (type != 's') && (type != 'r') && (type != 'c') && (type != 'd'))
        {
            dataString = mConnection.getData();
            continue;
        }
            
        
        vec3f position;
        
        position.x = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        position.y = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        position.z = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        vec3f alignment;
        
        alignment.x = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        alignment.y = (float)atof( dataString.substr(0, dataString.find(',')).c_str() );
        dataString = dataString.substr(dataString.find(',')+1);
        
        alignment.z = (float)atof( dataString.substr(0, dataString.find(';')).c_str() );
        dataString = dataString.substr(dataString.find(';')+1);
        
        mObjectManager->handleInput(id, type, position, alignment);

        dataString = mConnection.getData();

    }
}

string ClientNetworkHandler::parseInfoToDataString(ClientControlInfo &info)
{
    string returnString;
    
    returnString += toString(info.left) + ",";
    returnString += toString(info.right) + ",";
    returnString += toString(info.fire) + ",";
    returnString += toString(info.cameraVector.x) + ",";
    returnString += toString(info.cameraVector.y) + ",";
    returnString += toString(info.cameraVector.z);
    
    return returnString;
}

