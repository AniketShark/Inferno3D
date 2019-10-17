// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Shark
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="Physics.h" company="Inferno Studio">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef Physics_H_
#define Physics_H_

#include <windows.h>
#include <map>
#include "PhysX/Include/PxPhysicsAPI.h"

using namespace physx;


/// <summary>
/// Class Physics.
/// </summary>
class Physics
{
public:
	Physics();
	~Physics();
	void InitPhysics();
	void Release();

private:
	/// <summary>
	/// The g default error callback
	/// </summary>
	physx::PxDefaultErrorCallback gDefaultErrorCallback;
	/// <summary>
	/// The g default allocator callback
	/// </summary>
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	/// <summary>
	/// The m foundation
	/// </summary>
	physx::PxFoundation* mFoundation;
	/// <summary>
	/// The m profile zone manager
	/// </summary>
	physx::PxProfileZoneManager* mProfileZoneManager;
	/// <summary>
	/// The m physics
	/// </summary>
	physx::PxPhysics* mPhysics;
	/// <summary>
	/// The m cooking
	/// </summary>
	physx::PxCooking* mCooking;

	void CreateFoundationObject();
	void CreatePhysicsObject();
	void InitCookingLibrary();
	void CreatePhysicsScene();
};







#endif