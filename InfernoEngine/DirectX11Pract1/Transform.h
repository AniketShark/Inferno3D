////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////
#ifndef Transform_H_
#define Transform_H_

#include <windows.h>
#include "Vector.h"
#include "Helper.h"
#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	~Transform();
	Transform(const Transform&);

	Vector3 position;
	Vector3 scale;
	Vector3 localPosition;
	Vector3 worldEulerRotation;
	Vector3	localEulerRotation;
	Vector4 localQuaternionRotation;
	Vector4 worldQuaternionRotation;

	void operator= (Transform transform);
	// translate int the given direction by amount
	void Translate(Vector3 direction,float amount);
	// translate int the given direction by amount
	void Translate(float x, float y, float z);

	//Rotate object around given axis with given angle
	void Rotate(Vector3 axis,float angle,float deltaTime);
	void Rotate(float x,float y,float z);
	//Rotate object around given axis
	void RotateAroundLocal(Vector3 axis,float angle,float deltaTime);
	void RotateAroundLocal(float x,float y,float z);

	DirectX::XMVECTOR GetLocalRotationQuaternion();
	DirectX::XMVECTOR GetWorldRotationQuaternion();
	//Set local rotation quaternion from euler rotation
	void SetLocalRotationQuaternion(Vector3 eulerRotation);
	//Set world rotation quaternion from euler rotation
	void SetWorldRotationQuaternion(Vector3 eulerRotation);

	//Calculate euler angles from quaternion rotation
	void SetEulerRotationFromQuaternion(Vector3&, DirectX::XMVECTOR theQuaternionRotation);

	//Todo : move this to animationObject class
	//void SetLocalQuaternionRotationSpeed(Vector3 eulerRotationSpeed);
	//void SetWorldQuaternionRotationSpeed(Vector3 eulerRotationSpeed);

private:
	int m_ChildCount;

};

#endif