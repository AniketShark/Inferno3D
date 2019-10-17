
////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////

#ifndef Animation_H_
#define Animation_H_

#include <vector>
#include <map>
#include <string>
#include "Vector.h"
#include "CHRTimer.h"
#include "IBehavior.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"


class Animation : public IBehaviour
{
public:
	Animation();
	virtual ~Animation();

	int m_currentExecutionIndex;

	float m_translationCommandTime;
	float m_rotationCommandTime;
	float m_scalingCommandTime; 

	float m_moveRotateCommandTime;
	float m_moveScaleCommandTime;
	float m_rotateScaleCommandTime;

	std::map<std::string,SkeletalAnimation*> m_AnimationList;

	SkeletalAnimation* mCurrentAnimation;
	

	enum AnimationState
	{
		MOVING,
		ROTATING,
		SCALING,
		MOVENROTATE,
		MOVENSCALE,
		ROTATENSCALE,
		IDLE
	};

	Vector3 destination;
	Vector3 targetRotation;
	Vector3 targetScale;
	Vector3 m_velocity;
	Vector3 m_rotationVelocity;
	Vector3 m_LocalRotationVelocity;
	Vector3 m_scalingVelocity;
	Vector3 m_acceleration; 
	bool m_UserDefined;

	AnimationState m_objectAnimationState;
	AnimationState m_previousObjectAnimationState;

	CHRTimer m_timer;

	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();
	virtual IBehaviour* Clone();
	// Updates animation at each deltaTime 
	void Update(float deltaTime);

	//////////////////////////////////////////////////////////////////////////
	//  Animation Scripting commands
	//////////////////////////////////////////////////////////////////////////

	//move to the destination from source to destination in given timespan
	void MoveTo(Vector3 source,Vector3 destination, float time);
	//scale to the destination from source to destination in given timespan
	void ScaleTo(Vector3 source,Vector3 target,float time);
	//rotate from source to destination rotation in given timespan
	void RotateTo(Vector3 source,Vector3 target,float time);
	//moves and rotates to the target translation and rotation in given timespan
	void MoveAndRotateTo(Vector3 translationTarget,Vector3 rotationTarget,float time);
	//Moves and Scales to the target translation and scale in given timespan
	void MoveAndScaleTo(Vector3 translationTarget,Vector3 scaleTarget,float time);
	//Rotate and Scales to the target translation and scale in given timespan
	void RotateAndScaleTo(Vector3 rotationTarget,Vector3 scaleTarget,float time);

	//////////////////////////////////////////////////////////////////////////
	//  Manual runtime animation commands
	//////////////////////////////////////////////////////////////////////////

	//Moves Object in given direction with the given speed
	void ManualMove(Vector3 direction, float speed);
	//Moves Object in given direction with the given speed for given timespan
	void ManualMove(Vector3 direction, float speed, float time);

	//Rotates Object around given globalaxis with the given speed
	void ManualRotate(Vector3 rotationAxis, float speed);
	//Rotates Object around given localaxis with the given speed
	void ManualRotateLocal(Vector3 rotationAxis, float speed);
	//Rotates Object around given globalaxis with the given speed for given time
	void ManualRotate(Vector3 rotationAxis,float speed ,float time);

	//Scales Object around given localaxis with the given speed
	void ManualScale(Vector3 axis, float speed);
	//Scales Object around given localaxis with the given speed for given time
	void ManualScale(Vector3 axis, float speed, float time);

	//Move and Rotate Object around given axis with the given movespeed and given rotationspeed
	void ManualMoveAndRotateTo(Vector3 direction,Vector3 rotationAxis, float moveSpeed,float rotationSpeed);
	//Move and Scale Object in given direction with the given movespeed and given scaleSpeed
	void ManualMoveAndScaleTo(Vector3 direction,Vector3 axis, float moveSpeed,float scaleSpeed);
	//Rotate and Scale Object in given direction with the given rotationspeed and given scaleSpeed
	void ManualRotateAndScaleTo(Vector3 rotationAxis,Vector3 axis, float rotationSpeed,float scaleSpeed);

	//Creates Acceleration curve based on the final velocity of the object after the full execution time 
	// of command(i.e duration of animation) and no of samples we want to take
	// you can specify the curve type as well whether you want ease in 
	// or ease out animation
	void CreateAccelerationCurve(Vector3 finalVelocity,float commandExecutionTime,int noOfSteps,std::string curveType);

	void CalculateVelocity(Vector3 &velocity,float time);
	// Executes commands in the sequential order
	void ExecuteCommands(int);
	//play current animation command
	void Play();
	// stop current animation
	void Stop();


	//////////////////////////////////////////////////////////////////////////
	// All FBX Animation Methods
	//////////////////////////////////////////////////////////////////////////
	
	// plays specified animation from fbx file
	void PlayAnimation(std::string name);
	// sets the animation to be played
	void SetAnimation(SkeletalAnimation* anim);
	//Loads skeletal aimation from fbx file
	void LoadSkeletalAnimationFromFile(std::string fileName);
	// updates fbx animation at g_globalDeltaTime
	void AnimationUpdate();

	float                   mCurrentTime;
	float                   mAnimRate;
	float                   mCurrentFrame;

	bool                    mDrawBindPose;
	bool                    mSingleBoneSkinning;
	bool                    mFrameInterpolate;
	float                   mDebugRotation;

	unsigned char*          mChannelMap;
};



#endif