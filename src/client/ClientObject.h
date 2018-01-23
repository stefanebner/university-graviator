//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	ClientObejct
//	Holds the data of all the renderable objects.
//
//*********************************************************

#ifndef CLIENT_OBJECT
#define CLIENT_OBJECT

#include "..\vec3f.h"

class ClientObject
{
public:
	ClientObject(char type, vec3f position, vec3f alignment);
	~ClientObject(void);
	bool update(vec3f position, vec3f alignment);
	bool isSameTypeAs(char type);
	void markAsRead();
	bool getUpdeted();
	char getType();
	vec3f getPosition();
	vec3f getAlignment();
	int getAge();
	void incrementAge();

private:
	bool mUpdated;
	char mType;
	vec3f mPosition ,mAlignment;
	int mAge;
};

#endif