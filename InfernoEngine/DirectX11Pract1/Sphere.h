#ifndef Sphere_H_
#define Sphere_H_

#include "Vector.h"


class MySphere
{
public:
	MySphere();
	~MySphere();
	MySphere(const MySphere&);
	
	Vector3 mNormal;
	Vector3 mCenter;
	float mRadius;

	bool CheckCollision(const MySphere& other);
	
};

bool CheckCollision(MySphere& first,MySphere& second);

#endif