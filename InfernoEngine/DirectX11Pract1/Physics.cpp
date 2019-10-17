// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="Physics.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "Physics.h"
#include <math.h>


/// <summary>
/// Initializes a new instance of the <see cref="Physics" /> class.
/// </summary>
Physics::Physics()
{

}
/// <summary>
/// Finalizes an instance of the <see cref="Physics" /> class.
/// </summary>
Physics::~Physics()
{

}

/// <summary>
/// Initializes the physics.
/// </summary>
void Physics::InitPhysics()
{
	CreateFoundationObject();
	CreatePhysicsObject();
	InitCookingLibrary();
}

/// <summary>
/// Creates the foundation object.
/// </summary>
void Physics::CreateFoundationObject()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if(!Physics::mFoundation)
		MessageBox(NULL,L"PxCreateFoundation failed!",L"PhysX Error",MB_OK);
}

/// <summary>
/// Creates the physics object.
/// </summary>
void Physics::CreatePhysicsObject()
{
	bool recordMemoryAllocations = true;
	mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
	if(!mProfileZoneManager)
		MessageBox(NULL,L"PxProfileZoneManager::createProfileZoneManager failed!",L"PhysX Error",MB_OK);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );
	if(!mPhysics)
		MessageBox(NULL,L"PxCreatePhysics failed!",L"PhysX Error",MB_OK);
}

/// <summary>
/// Initializes the cooking library.
/// </summary>
void Physics::InitCookingLibrary()
{
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams());
	if (!mCooking)
		MessageBox(NULL,L"PxCreateCooking failed!",L"PhysX Error",MB_OK);
}

void Physics::CreatePhysicsScene()
{

}

/// <summary>
/// Releases this instance.
/// </summary>
void Physics::Release()
{
	mPhysics->release();
	mFoundation->release();
}

