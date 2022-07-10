#include "Rigidbody.h"


Rigidbody::Rigidbody()
{
	this->mMass = 0;
	this->mAcceleraion = Vector3::Zero;
	this->mVelocity = Vector3::Zero;
	this->mDrag = Vector3::Zero;
	this->mLastSimulationUpdateTime = 0;
	/*this->mRigidbodyState.mPosition = Vector3::Zero;
	this->mRigidbodyState.mVelocity = Vector3::Zero;

	this->mRigidbodyEulerState.mPosition = Vector3::Zero;
	this->mRigidbodyEulerState.mVelocity = Vector3::Zero;
	this->mRigidbodyEulerState.mForce = Vector3::Zero;
	this->mRigidbodyEulerState.mDrag = Vector3::Zero;
	this->mRigidbodyEulerState.mMass = 1;*/

}
Rigidbody::Rigidbody(const Rigidbody& otherRigidbody)
{
	this->mMass = otherRigidbody.mMass;
	this->mAcceleraion = otherRigidbody.mAcceleraion;
	this->mVelocity = otherRigidbody.mVelocity;
	this->mDrag = otherRigidbody.mDrag;
}

void Rigidbody::operator= (const Rigidbody& otherRigidbody)
{
	this->mMass = otherRigidbody.mMass;
	this->mAcceleraion = otherRigidbody.mAcceleraion;
	this->mVelocity = otherRigidbody.mVelocity;
	this->mDrag = otherRigidbody.mDrag;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::SetInitialPosition(Vector3 position)
{
	//this->mRigidbodyState.mPosition = position;
}

Vector3 Rigidbody::GetPosition()
{
	return Vector3::Zero;//  return this->mRigidbodyEulerState.mPosition;
	//return this->mRigidbodyState.mPosition;
}

void Rigidbody::SetVelocity(Vector3 velocity)
{
	//this->mRigidbodyEulerState.mVelocity = velocity;
	//this->mRigidbodyState.mVelocity = velocity;
}
Vector3 Rigidbody::GetVelocity()
{
	return Vector3::Zero; //this->mRigidbodyEulerState.mVelocity;
	//return mRigidbodyState.mVelocity;
}
void Rigidbody::SetAcceleration(Vector3 accleration)
{
	
}
void Rigidbody::SetForce(Vector3 force)
{
	//this->mRigidbodyEulerState.mForce = force;
}
Vector3 Rigidbody::GetForce()
{
	return Vector3::Zero; //this->mRigidbodyEulerState.mForce;
}
Vector3 Rigidbody::GetAcceleration()
{
	return Vector3::Zero;
}


void Rigidbody::AddForce(Vector3 direction, float force)
{
	//mRigidbodyEulerState.mForce = direction.GetNormalized() * force;
}

void Rigidbody::Update()
{
	if((mTimeSinceStart - mLastSimulationUpdateTime) > mFixedTimeStep)
	{
		//EulerIntegrate(this->mRigidbodyEulerState,mTimeSinceStart,mFixedTimeStep);
		
		mLastSimulationUpdateTime = mTimeSinceStart;
	}
}