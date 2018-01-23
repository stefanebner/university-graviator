#include "ClientObjectManager.h"



ClientObjectManager::ClientObjectManager(void):
	mMaxObjectAge(100),
	mQuitWithConnectionFailure(false),
	mOwnPlayerEnergy(100),
	mOwnPlayerPoints(0),
	mEnemyPlayerPoints(0),
	mOwnPlayerId(0),
	mOwnPlayerPosition(vec3f()),
	mOwnPlayerAlignment(vec3f(1.0,0.0,0.0))
{
	pthread_mutex_init(&mObjectStateMutex, NULL);
	pthread_mutex_init(&mObjectDeletionMutex, NULL);
}

ClientObjectManager::~ClientObjectManager(void)
{
}


void ClientObjectManager::handleInput(int id, char type, vec3f position, vec3f alignment)
{
	switch (type)	
	{
	case 'd':					// delete the object
		if (mObjectStateMap.count(id) != 0)
		{
			Synchronized threadStateBlocker(&mObjectStateMutex);
			deleteObject(id);
		}
		break;
	case 'e':					// set player energy information
		if (id == mOwnPlayerId)
		{
			mOwnPlayerEnergy = position.x;
			
            mOwnPlayerPoints = alignment.x;
            mEnemyPlayerPoints = alignment.y;
			
		}
		break;
	default:					// update the object
				
		if (id == mOwnPlayerId && mOwnPlayerId != 0)
		{
			mOwnPlayerPosition = position;
			mOwnPlayerAlignment = alignment;
		}
		else
		{
			Synchronized threadStateBlocker(&mObjectStateMutex);

			ClientObject *actualObject;

			if (mObjectStateMap.count(id) == 0)
			{
				actualObject = new ClientObject(type, position, alignment);
				
				mObjectStateMap.insert(pair<int, ClientObject *>(id,actualObject));
			}
			else 
			{
				actualObject = mObjectStateMap[id];

				if(actualObject->isSameTypeAs(type))
				{
					actualObject->update(position, alignment);
				}
				else
				{
					deleteObject(id);
					ClientObject *newObject = new ClientObject(type, position, alignment);
					mObjectStateMap.insert(pair<int, ClientObject *>(id,newObject));
				}
			}
		}
		break;
	}	// end switch
}

void ClientObjectManager::setPlayerId(unsigned int id)
{
	mOwnPlayerId = id;
}

void ClientObjectManager::makeObjectStatesOlder()
{
	Synchronized threadStateBlocker(&mObjectStateMutex);
	map<int,ClientObject *>::iterator it;
	for (it=mObjectStateMap.begin(); it != mObjectStateMap.end(); it++)
	{
		it->second->incrementAge();
	}
}

void ClientObjectManager::cleanUpObjectStates()
{
	Synchronized threadStateBlocker(&mObjectStateMutex);
	vector<int> objectIdsToDelete;
	map<int,ClientObject *>::iterator it;
	for (it=mObjectStateMap.begin(); it != mObjectStateMap.end(); it++)
	{
		if (it->second->getAge() >= mMaxObjectAge)
		{
			 objectIdsToDelete.push_back(it->first);
		}
	}
	for (unsigned int i = 0; i < objectIdsToDelete.size(); i++)
	{
		deleteObject(objectIdsToDelete[i]);
	}
}

bool ClientObjectManager::getQuitWithConnectionFailure()
{
	return mQuitWithConnectionFailure;
}

void ClientObjectManager::setQuitWithConnectionFailure(bool value)
{
	mQuitWithConnectionFailure = value;
	if (mQuitWithConnectionFailure)
	{
		// declare everything for deletion
		mMaxObjectAge = 0;
		cleanUpObjectStates();
	}
}

vec3f ClientObjectManager::getOwnPlayerPosition()
{
	return mOwnPlayerPosition;
}

vec3f ClientObjectManager::getOwnPlayerAlignment()
{
	return mOwnPlayerAlignment;
}

float ClientObjectManager::getOwnPlayerEnergy()
{
	return mOwnPlayerEnergy;
}

float ClientObjectManager::getOwnPlayerPoints()
{
    return mOwnPlayerPoints;
}

float ClientObjectManager::getEnemyPlayerPoints()
{
    return mEnemyPlayerPoints;
}

RenderObjectInformation ClientObjectManager::pullNextUnpulledState()
{
	RenderObjectInformation unpulledObjectInformation;
	int nextUnupdatedObjectId = getNextUnupdatedObjectId();
	if (nextUnupdatedObjectId != NO_OBJECTS_AVAILABLE)
	{
		ClientObject *unpulledObject = mObjectStateMap[nextUnupdatedObjectId];

		unpulledObjectInformation.id = nextUnupdatedObjectId;
		unpulledObjectInformation.type = unpulledObject->getType();
		unpulledObjectInformation.position = unpulledObject->getPosition();
		unpulledObjectInformation.alignment = unpulledObject->getAlignment();
	}
	else
	{
		unpulledObjectInformation.id = NO_OBJECTS_AVAILABLE;
	}

	return unpulledObjectInformation;
}


int ClientObjectManager::pullNextUnpulledDeletion()
{
	int id = NO_OBJECTS_AVAILABLE;
	Synchronized threadDeletionBlocker(&mObjectDeletionMutex);
	if (!mObjectDeletionList.empty())
	{
		id = mObjectDeletionList.back();
		mObjectDeletionList.pop_back();
	}
	return id;
}


int ClientObjectManager::getNextUnupdatedObjectId()
{
	int objectId = NO_OBJECTS_AVAILABLE;
	map<int,ClientObject *>::iterator it;

	Synchronized threadStateBlocker(&mObjectStateMutex);

	for (it=mObjectStateMap.begin(); it!=mObjectStateMap.end(); it++)
	{
		if (it->second->getUpdeted() == true)
		{
			objectId = it->first;
			it->second->markAsRead();
			break;
		}
	}
	return objectId;
}


void ClientObjectManager::deleteObject(int id)
{
	ClientObject *actualObject;
	actualObject = mObjectStateMap[id];
	mObjectStateMap.erase(id);

	delete actualObject;

	Synchronized threadDeletionBlocker(&mObjectDeletionMutex);


	bool isAlreadyInList = false;
	vector<int>::iterator it;
	for (it=mObjectDeletionList.begin() ; it < mObjectDeletionList.end(); it++ )
		if (*it == id)
			isAlreadyInList = true;

	if (!isAlreadyInList)
		mObjectDeletionList.push_back(id);
}