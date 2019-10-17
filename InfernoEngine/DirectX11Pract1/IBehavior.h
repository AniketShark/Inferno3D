// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-15-2014
// ***********************************************************************
// <copyright file="IBehavior.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************


#ifndef IBehaviour_H_
#define IBehaviour_H_

#include "Component.h"
class SharedInfo;
class GameObject;

/// <summary>
/// Class IBehaviour.
/// </summary>
class IBehaviour : public Component
{
public:
	IBehaviour();
	virtual ~IBehaviour();
	bool mEnabled;
	//Initializes Object
	virtual void Initialize();
	//Updates Component every frame
	virtual void Update();
	//Destroys and releases all the resources when called 
	virtual void Destroy();
	//Clones the behavior
	virtual IBehaviour* Clone();
	/// <summary>
	/// The  shared information pointer
	/// </summary>
	SharedInfo* mThisGO;
	GameObject* mGameObjectPointer;
};	

#endif