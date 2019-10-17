#include "Sphere.h"

MySphere::MySphere()
{
	mCenter = Vector3::Zero;
	mRadius = 0.0f;
	this->mNormal = Vector3::Zero;
}
MySphere::~MySphere()
{

}
MySphere::MySphere(const MySphere& other)
{
	this->mCenter = other.mCenter;
	this->mNormal = other.mNormal;
	this->mRadius = other.mRadius;
}

bool MySphere::CheckCollision(const MySphere& other)
{
	 float distance =  Vector3::Distance(this->mCenter,other.mCenter);
	 float threshold = this->mRadius + other.mRadius;
	 if (distance < threshold)
	 {
		 return true;
	 }
	 return false;

}

bool CheckCollision(MySphere& first,MySphere& second)
{
	float distance =  Vector3::Distance(first.mCenter,second.mCenter);
	float threshold = first.mRadius + second.mRadius;
	if (distance < threshold)
	{
		return true;
	}
	return false;

}