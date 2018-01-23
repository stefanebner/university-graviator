#include "ClientObject.h"

ClientObject::ClientObject(char type, vec3f position, vec3f alignment):
	mType(type), 
	mPosition(position), 
	mAlignment(alignment),
	mUpdated(true),
	mAge(0)
{
}

ClientObject::~ClientObject(void)
{
}

bool ClientObject::update(vec3f position, vec3f alignment)
{
	mPosition = position;
	mAlignment = alignment;
	
	mUpdated = true;
	mAge = 0;
	return mUpdated;
}

bool ClientObject::isSameTypeAs(char type)
{
	bool typeIsEqual = true;
	if (mType != type)
		typeIsEqual = false;
	return typeIsEqual;
}

void ClientObject::markAsRead()
{
	mUpdated = false;
}

bool ClientObject::getUpdeted()
{
	return mUpdated;
}

char ClientObject::getType()
{
	return mType;
}

vec3f ClientObject::getPosition()
{
	return mPosition;
}

vec3f ClientObject::getAlignment()
{
	return mAlignment;
}

int ClientObject::getAge()
{
	return mAge;
}

void ClientObject::incrementAge()
{
	mAge++;
}