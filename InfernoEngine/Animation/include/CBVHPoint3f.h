#ifndef _CBVHPoint3f_HG_
#define _CBVHPoint3f_HG_

class CBVHPoint3f
{
public:
	CBVHPoint3f();
	CBVHPoint3f( float x, float y, float z );
	CBVHPoint3f(const CBVHPoint3f& p);   // copy constructor
	float x; float y; float z;
};

#endif