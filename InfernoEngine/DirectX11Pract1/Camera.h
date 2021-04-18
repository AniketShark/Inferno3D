
////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////

#ifndef Camera_H_
#define Camera_H_

#include <windows.h>
#include <DirectXMath.h>
#include "Vector.h"
#include "IBehavior.h"

class Camera : public IBehaviour
{
public:
	Camera();
	~Camera();
	Transform* mLookAtTransform;
	float mFov;
	Vector2 mAspectRatio;
	float mNearClipPlaneDistance;
	float mFarClipPlaneDistance;

	DirectX::XMFLOAT3 Eye;
	DirectX::XMFLOAT3 View;
	DirectX::XMFLOAT3 Up;

	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();

	virtual IBehaviour* Clone();

	//Sets the Camera to given Position
	void SetCamera(float posX,float posY,float posZ);

	//Sets the look at point
	void LookAtPoint(float posX,float posY,float posZ);
	//Sets transform the camera will look at.
	void SetLookAtTransform(Transform* transform);
	
	//moves camera 
	void MoveCamera(float deltaX,float deltaY,float deltaZ);

	//moves camera in the given direction
	void Camera::MoveCamera(Vector3 direction);

	//Rotates Camera view
	void CameraPitchYawRoll(float pitch,float yaw,float roll);
	
	void MoveAroundPoint(Vector3 point,float distance,float moveSpeed);
	
};

#endif 
