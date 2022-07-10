#ifndef RK4_H_
#define RK4_H_

#include <windows.h>
#include <DirectXMath.h>
//#include <xnamath.h>
#include <stdio.h>
#include <math.h>
#include "Vector.h"
// Make it C++ ish
#include <iostream>


class State
{
public:
	//primary values
	Vector3 mPosition;
	Vector3 mMomentum;
	Vector3 mOrientation;
	Vector3 mAngularMomentum;

	//Secondary state
	Vector3 mVelocity;
	Vector3 mSpin;
	Vector3 mAngularVelocity;
	
	/*Matrix mBodyToWorld;
	Matrix mWorldToBody;*/

	/// constant state
	float mSize;
	float mMass;
	float mInverseMass;
	float mInertiaTensor;
	float mInverseInertiaTensor;

	/// Recalculate secondary state values from primary values.

	
};

class Derivative
{
public:
	Vector3 mVelocity;
	Vector3 mForce;

	Vector3 mTorque;
	
};

float acceleration(const State &state, float t);

Derivative evaluate(const State &initial, float t);

Derivative evaluate(const State &initial, float t, float dt, const Derivative &derivative);

void integrate(State &state, float t, float dt);

#endif