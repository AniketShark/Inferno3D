#include "CBVHPoint3f.h"

CBVHPoint3f::CBVHPoint3f()
{
	this->x = this->y = this->z = 0.0f;
}

CBVHPoint3f::CBVHPoint3f( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

CBVHPoint3f::CBVHPoint3f(const CBVHPoint3f& p)   // copy constructor
{
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
}
