//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Object
//	Abstract base class for the players and gravitational
//	objects on the server
//
//*********************************************************

#ifndef SERVER_OBJECT_H
#define SERVER_OBJECT_H

#include "../src/vec3f.h"
#include <ctime>

class ServerObject
{
public:
	ServerObject(void);

	char getType(); 
	void setType(char type);

	vec3f getPosition();
	void setPosition(const vec3f newPosition);

	float getMass();
	void setMass(float newMass);

	unsigned int getID();
	void setID(unsigned int id);

	vec3f getVelocity();
	void setVelocity(vec3f velocity);
	
	vec3f getOrientation();
	void setOrientation(vec3f orientation);

	virtual vec3f getForce(vec3f affectedLocation) = 0;
	virtual void update() = 0;
	
protected:
	virtual ~ServerObject(void) {}
	
	char mType;
	float mMass;
	unsigned int mID;
	bool mMortal;
	int mSecondsToLive;
	vec3f mPosition, mHeading, mVelocity, mOrientation;

};




#endif