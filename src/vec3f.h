//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	vec3f
//	Helper class for vector operations
//  not yet fully functional
//
//*********************************************************

#ifndef VEC3F_H
#define VEC3F_H


#include <cmath>

class vec3f
{
public:
	~vec3f() {}

	vec3f( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f );
	vec3f(const vec3f& o);

	//operator overloading:

	//negative values
	vec3f operator-() const { return vec3f(-x, -y, -z); }
	//same values
	bool operator==( const vec3f& o) { return (x == o.x) && (y == o.y) && (z == o.z); }
	//different values
	bool operator!=( const vec3f& o) { return (x != o.x) || (y != o.y) || (z != o.z); }

	// set to the same
	vec3f & operator=(const vec3f& o) { x = o.x; y = o.y; z = o.z; return *this; }

	// +=
	vec3f operator+(const vec3f& o) const { return vec3f(x+o.x, y+o.y, z+o.z); }
	vec3f & operator+=(const vec3f& o) { x += o.x; y += o.y; z += o.z; return *this; }
	

	// subtract
	vec3f operator-(const vec3f& o) const { return vec3f(x-o.x, y-o.y, z-o.z); }
	vec3f & operator-=(const vec3f& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }

	//multiply
	vec3f operator*(const float& value) const { return vec3f(x*value, y*value, z*value); }
	vec3f & operator*=(const float& value) { x *= value; y *= value; z *= value; return *this; }

	//divide
	vec3f operator/(const vec3f& o) const { return vec3f( o.x!=0 ? x/o.x : x, o.y!=0 ? y/o.y : y, o.z!=0 ? z/o.z : z ); }
	vec3f operator/(const float& value) { return vec3f( value!=0 ? x/value : x, value!=0 ? y/value : y, value!=0 ? z/value : z ); }

	void normalize();
	bool isZero();
	void set(float _x, float _y, float _z);
	float dot(const vec3f& o);
	float dotSelf();
	vec3f cross(const vec3f& o);
	bool areEqual(const vec3f& o);
	float getLength();
	float angle(const vec3f& o);
	vec3f projectTo(const vec3f& o);

	union
	{
		struct 
		{
			float x;
			float y;
			float z;
		};
		float v[3];
	};

};


#endif