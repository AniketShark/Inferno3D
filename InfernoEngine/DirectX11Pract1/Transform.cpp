#include "Transform.h"
#include "Helper.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}

Transform::Transform(const Transform& otherTransform)
{
	this->position = otherTransform.position;
	this->worldEulerRotation = otherTransform.worldEulerRotation;
	this->scale = otherTransform.scale;
}

void Transform::operator= (Transform go)
{
	this->position = go.position;
	this->worldEulerRotation = go.worldEulerRotation;
	this->scale = go.scale;
}

void Transform::Translate(Vector3 direction,float amount)
{
	direction = direction.GetNormalized();
	this->position = this->position + direction * amount; 
}

void Transform::Translate(float x, float y, float z)
{
	this->position = this->position + Vector3(x,y,z);
}

void Transform::Rotate(Vector3 axis,float angle,float deltaTime)
{	

}

void Transform::Rotate(float x,float y,float z)
{
	this->worldEulerRotation = this->worldEulerRotation + Vector3(x,y,z);
}

void Transform::RotateAroundLocal(Vector3 axis,float angle,float deltaTime)
{
	axis = ((axis.GetNormalized() * angle) + this->localEulerRotation);
	this->localEulerRotation = Vector3(axis.x,axis.y,axis.z);
}

void Transform::RotateAroundLocal(float x,float y,float z)
{
	this->localEulerRotation = this->localEulerRotation + Vector3(x,y,z);
}

DirectX::XMVECTOR Transform:: GetLocalRotationQuaternion()
{
	return DirectX::XMVectorSet(localQuaternionRotation.x,localQuaternionRotation.y,localQuaternionRotation.z,localQuaternionRotation.w);
}

DirectX::XMVECTOR Transform::GetWorldRotationQuaternion()
{
	return DirectX::XMVectorSet(worldQuaternionRotation.x,worldQuaternionRotation.y,worldQuaternionRotation.z,worldQuaternionRotation.w);
}

void Transform::SetLocalRotationQuaternion(Vector3 eulerRotation)
{
	float x = Helper::DegToRad(eulerRotation.x);
	float y = Helper::DegToRad(eulerRotation.y);
	float z = Helper::DegToRad(eulerRotation.z);
	DirectX::XMFLOAT4 tempQuat = DirectX::XMFLOAT4(0,0,0,0);
	DirectX::XMStoreFloat4(&tempQuat, DirectX::XMQuaternionRotationRollPitchYaw(x,y,z));
	localQuaternionRotation = Vector4(tempQuat.x,tempQuat.y,tempQuat.z,tempQuat.w);
}

void Transform::SetWorldRotationQuaternion(Vector3 eulerRotation)
{
	float x = Helper::DegToRad(eulerRotation.x);
	float y = Helper::DegToRad(eulerRotation.y);
	float z = Helper::DegToRad(eulerRotation.z);
	DirectX::XMFLOAT4 tempQuat = DirectX::XMFLOAT4(0,0,0,0);
	DirectX::XMStoreFloat4(&tempQuat, DirectX::XMQuaternionRotationRollPitchYaw(x,y,z));
	worldQuaternionRotation = Vector4(tempQuat.x,tempQuat.y,tempQuat.z,tempQuat.w);
}

void Transform::SetEulerRotationFromQuaternion(Vector3& rotationVector, DirectX::XMVECTOR theQuaternionRotation)
{
	DirectX::XMVECTOR axis;
	float angle;
	DirectX::XMFLOAT4 floatAxis;
	DirectX::XMQuaternionToAxisAngle(&axis,&angle,theQuaternionRotation);
	DirectX::XMStoreFloat4(&floatAxis,axis);
	rotationVector = Helper::ConvertXMFLOAT4TOVector3(floatAxis) * angle;
}



//////////////////////////////////////////////////////////////////////////
//Move these methods to animation/Animator object
//////////////////////////////////////////////////////////////////////////

//void Transform::SetLocalQuaternionRotationSpeed(Vector3 eulerRotationSpeed)
//{
//	float x = Helper::DegToRad(eulerRotationSpeed.x);
//	float y = Helper::DegToRad(eulerRotationSpeed.y);
//	float z = Helper::DegToRad(eulerRotationSpeed.z);
//	XMStoreFloat4(&localRotationSpeedQuaternion,XMQuaternionRotationRollPitchYaw(x,y,z));
//}
//
//void Transform::SetWorldQuaternionRotationSpeed(Vector3 eulerRotationSpeed)
//{
//	float x = Helper::DegToRad(eulerRotationSpeed.x);
//	float y = Helper::DegToRad(eulerRotationSpeed.y);
//	float z = Helper::DegToRad(eulerRotationSpeed.z);
//	XMStoreFloat4(&worldRotationSpeedQuaternion,XMQuaternionRotationRollPitchYaw(x,y,z));
//}
//void Transform::IncrementRotation( float timeDelta )
//{
//	m_IncrementLocalRotation(timeDelta);
//	m_IncremenWorldRotation(timeDelta);
//}
//
//void Transform::m_IncrementLocalRotation(float timeDelta)
//{
//	// Determine how much of the rotation speed we have to go.
//	// In 1 second, we would move an angle of m_pqRotationSpeed.
//	// This is from the "origin" (or the "unit" quaternion)
//	XMVECTOR qIdentity = XMQuaternionIdentity();
//	XMVECTOR qRotationSpeed 
//		= XMVectorSet( this->localRotationSpeedQuaternion.x, this->localRotationSpeedQuaternion.y, 
//		this->localRotationSpeedQuaternion.z, this->localRotationSpeedQuaternion.w );
//	XMVECTOR qAdjust 
//		= XMQuaternionSlerp( qIdentity, qRotationSpeed, timeDelta );
//
//	// Now get the current angle... 
//	XMVECTOR qCurrentRotation 
//		= XMVectorSet( this->localQuaternionRotation.x, this->localQuaternionRotation.y, 
//		this->localQuaternionRotation.z, this->localQuaternionRotation.w );
//
//	// Do the actual transformation...
//	XMVECTOR qResult = XMQuaternionMultiply( qCurrentRotation, qAdjust );
//	qResult = XMQuaternionNormalize( qResult );
//
//	// Save it back
//	XMStoreFloat4( &(this->localQuaternionRotation), qResult );
//	SetEulerRotationFromQuaternion(this->localEulerRotation,qResult);
//}
//
//void GameObject::m_IncremenWorldRotation(float timeDelta)
//{
//	// Determine how much of the rotation speed we have to go.
//	// In 1 second, we would move an angle of m_pqRotationSpeed.
//	// This is from the "origin" (or the "unit" quaternion)
//	XMVECTOR qIdentity = XMQuaternionIdentity();
//	XMVECTOR qRotationSpeed 
//		= XMVectorSet( this->worldRotationSpeedQuaternion.x, this->worldRotationSpeedQuaternion.y, 
//		this->worldRotationSpeedQuaternion.z, this->worldRotationSpeedQuaternion.w );
//	XMVECTOR qAdjust 
//		= XMQuaternionSlerp( qIdentity, qRotationSpeed, timeDelta );
//
//	// Now get the current angle... 
//	XMVECTOR qCurrentRotation 
//		= XMVectorSet( this->worldQuaternionRotation.x, this->worldQuaternionRotation.y, 
//		this->worldQuaternionRotation.z, this->worldQuaternionRotation.w );
//
//	// Do the actual transformation...
//	XMVECTOR qResult = XMQuaternionMultiply( qCurrentRotation, qAdjust );
//	qResult = XMQuaternionNormalize( qResult );
//
//	// Save it back
//	XMStoreFloat4( &(this->worldQuaternionRotation), qResult );
//	SetEulerRotationFromQuaternion(this->worldEulerRotation,qResult);
//}