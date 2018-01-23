//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientObjectManager
//	Handles everything that comes in at the ClientNetworkHandler. 
//	Manages the objects (create, move, destroy...).
//
//*********************************************************

#ifndef CLIENT_OBJECT_MANAGER
#define CLIENT_OBJECT_MANAGER

#include "ClientObject.h"
#include "..\GraviatorSimpleTypes.h"
#include <map>
#include <vector>

#define NO_OBJECTS_AVAILABLE -1

struct RenderObjectInformation
{
	int id;
	char type;
	vec3f position, alignment;
};

using namespace std;

class ClientObjectManager
{
public:
	ClientObjectManager(void);
	~ClientObjectManager(void);
	void handleInput(int id, char type, vec3f position, vec3f alignment);
	void setPlayerId(unsigned int id);
	void makeObjectStatesOlder();
	void cleanUpObjectStates();
	
	bool getQuitWithConnectionFailure();
	void setQuitWithConnectionFailure(bool value);

	vec3f getOwnPlayerPosition();
	vec3f getOwnPlayerAlignment();

	float getOwnPlayerEnergy();
    float getOwnPlayerPoints();
    float getEnemyPlayerPoints();

	RenderObjectInformation pullNextUnpulledState();
	int pullNextUnpulledDeletion();
	
private:
	
	int getNextUnupdatedObjectId();
	void deleteObject(int id);

	pthread_mutex_t mObjectStateMutex;
	pthread_mutex_t mObjectDeletionMutex;

	int mMaxObjectAge;

	unsigned int mOwnPlayerId;
	vec3f mOwnPlayerPosition;
	vec3f mOwnPlayerAlignment;
	float mOwnPlayerEnergy;
    float mOwnPlayerPoints;
    float mEnemyPlayerPoints;

	map<int,ClientObject *> mObjectStateMap;
	vector<int> mObjectDeletionList;
	bool mQuitWithConnectionFailure;
	

};

#endif
