#ifndef Rigidbody_H_
#define Rigidbody_H_

#include "RK4.h"
#include "Euler.h"
#include "CHRTimer.h"
#include "Vector.h"



class Rigidbody
{
public:
	Rigidbody();
	Rigidbody(const Rigidbody&);
	~Rigidbody();
	float mMass;
	
	Vector3 mVelocity;
	Vector3 mAcceleraion;
	Vector3 mDrag;
	
	void operator= (const Rigidbody&);
	void SetInitialPosition(Vector3); 
	Vector3 GetPosition();
	void SetVelocity(Vector3);
	Vector3 GetVelocity();
	void SetForce(Vector3 force);
	Vector3 GetForce();
	void SetAcceleration(Vector3);
	Vector3 GetAcceleration();
	void AddForce(Vector3, float);
	void Update();


private :
	float mLastSimulationUpdateTime;
	State mRigidbodyState;
	EulerState mRigidbodyEulerState;
	Derivative mRigidbodyDerivative;
};
#endif