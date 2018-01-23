#include "Utility.h"
#include <ctime>
#include <cstdlib>

#define PI acos(-1.0)

Utility::Utility(void)
{
    srand( (unsigned) time(NULL) ) ;
}

Utility::~Utility(void)
{
}

vec3f setYtoArenaHeight(vec3f vector)
{
	float scaledX = vector.x / (float)ARENA_RADIUS;
	float scaledZ = vector.z / (float)ARENA_RADIUS;
	vector.y = (1 - sqrt(-scaledX*scaledX - scaledZ*scaledZ + 1)) * (float)ARENA_RADIUS;
	return vector;
}

vec3f transformToCalculationCoordinates( vec3f position )
{
    vec3f arenaSphereCenter(0.0, (float)ARENA_RADIUS, 0.0);
    vec3f centerToPosition = position - arenaSphereCenter;
    vec3f centerToZero = arenaSphereCenter *(-1);
    
    float angle1 = centerToZero.angle(centerToPosition);
    // scale from PI/2 to defined arena radius
    angle1 = angle1 * 2 * (float)ARENA_RADIUS / (float)PI;
    float angle2 = atan2(position.z, position.x);
    
    float newX = angle1 * cos(angle2);
    float newZ = angle1 * sin(angle2);
    
    return vec3f(newX, 0, newZ);
}


vec3f transformToRenderingCoordinates( vec3f position )
{
    vec3f arenaSphereCenter(0.0, (float)ARENA_RADIUS, 0.0);
    vec3f centerToZero = arenaSphereCenter *(-1);
    
    float angle1 = position.getLength();
    // scale from defined arena radius to PI/2
    angle1 = angle1 * (float)PI / (2 * (float)ARENA_RADIUS);
    float angle2 = atan2(position.z, position.x);
    
    centerToZero.x = sin(angle1) * (-centerToZero.y);
    centerToZero.y = cos(angle1) * (centerToZero.y);
    centerToZero.x = cos(angle2) * (centerToZero.x);
    centerToZero.z = sin(angle2) * (centerToZero.x);
    
    return (centerToZero + arenaSphereCenter);
}

vec3f getArenaNormalAtPosition(vec3f position)
{
	vec3f arenaSphereCenter(0.0, ARENA_RADIUS, 0.0);
	return arenaSphereCenter - position;
}

double getPi()
{
    return PI;
}
