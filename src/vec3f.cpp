#include "vec3f.h"
#include <cmath>

vec3f::vec3f( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

vec3f::vec3f(const vec3f& o)
{
	x = o.x;
	y = o.y;
	z = o.z;
}

void vec3f::normalize()
{
	float vectorLength = sqrt(dot(*this));
	*this = (*this) / vectorLength;
}


bool vec3f::isZero()
{
	return( x == 0 && y == 0 && z == 0);
}

void vec3f::set( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

bool vec3f::areEqual( const vec3f& o )
{
	float epsilon = 0.001f;	// tollerance
	bool areEqual = false;
	vec3f differenceVector;
	differenceVector.set(x-o.x, y-o.y, z-o.z);

	if (dot(differenceVector) < epsilon)
		areEqual = true;
	return areEqual;
}

float vec3f::dot( const vec3f& o )
{
	return (x*o.x + y*o.y + z*o.z);
}

float vec3f::getLength()
{
	return sqrt(dotSelf());
}

float vec3f::angle(const vec3f& o)
{
    vec3f other = o;
    return acos((dot(o) / (getLength() * other.getLength())));
}

vec3f vec3f::projectTo(const vec3f &o)
{
    vec3f other = o;
    return (other * other.dot(vec3f(this->x, this->y, this->z)) / other.dot(other));
}


float vec3f::dotSelf()
{
	return (x*x + y*y + z*z);
}

vec3f vec3f::cross(const vec3f& o)
{
	return vec3f(y*o.z - o.y*z, z*o.x - o.z*x, x*o.y - o.x*y);
}