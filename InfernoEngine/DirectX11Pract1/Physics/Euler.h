#ifndef Euler_H_
#define Euler_H_

#include "Vector.h"

class EulerState
{
public:
	float mSize;
	float mMass;
	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mAccerleration;
	Vector3 mForce;
	Vector3 mDrag;
	float mInverseMass;
};

void EulerIntegrate(EulerState& state,float t,float dt);

#endif