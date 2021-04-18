////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////

#include "Animation.h"
#include "SharedInfo.h"
#include "FbxHandler.h"

Renderer* localReferenceToRenderer;

Animation::Animation()
{
	m_currentExecutionIndex = 0;
	m_translationCommandTime = 0;
	m_rotationCommandTime = 0;
	m_scalingCommandTime = 0;

	m_moveRotateCommandTime = 0;
	m_moveScaleCommandTime = 0;
	m_rotateScaleCommandTime = 0;  

	m_velocity = Vector3::Zero;
	m_rotationVelocity = Vector3::Zero;
	m_LocalRotationVelocity = Vector3::Zero;
	m_scalingVelocity = Vector3::Zero;
	m_UserDefined = false;
	mThisGO = new SharedInfo();
	mCurrentAnimation = NULL;
	mChannelMap = NULL;
}

Animation::~Animation()
{
	if(mThisGO)
		delete mThisGO;
	if(mCurrentAnimation)
		delete mCurrentAnimation;
	if(mChannelMap)
		delete mChannelMap;
	if(localReferenceToRenderer)
		delete localReferenceToRenderer;
	if(m_AnimationList.size() > 0)
	{
		std::map<std::string,SkeletalAnimation*>::iterator animIt;
		for(animIt = m_AnimationList.begin();animIt != m_AnimationList.end();animIt++)
		{
		}
	}
}

void Animation::Initialize()
{
	if(mThisGO->renderer != NULL)
		localReferenceToRenderer = mThisGO->renderer;
	mDrawBindPose = false;
	mFrameInterpolate = false;
	mAnimRate = 1; 
}

void Animation::Update()
{
	AnimationUpdate();
}

ComponentType Animation::GetType()
{
	return m_type;
}

void Animation::Destroy()
{

}

void Animation::MoveTo(Vector3 source,Vector3 destination, float time)
{
	this->destination = destination;
	m_velocity.x = (destination.x - source.x)/time;
	m_velocity.y = (destination.y - source.y)/time;
	m_velocity.z = (destination.z - source.z)/time;

	this->m_objectAnimationState = Animation::MOVING;
}

void Animation::RotateTo(Vector3 source,Vector3 target,float time)
{
	this->targetRotation = target;

	m_rotationVelocity.x = (target.x - source.x)/time;
	m_rotationVelocity.y = (target.y - source.y)/time;
	m_rotationVelocity.z = (target.z - source.z)/time;

	this->m_objectAnimationState = Animation::ROTATING;
}

void Animation::ScaleTo(Vector3 source,Vector3 target,float time)
{
	this->targetRotation = target;

	m_scalingVelocity.x = (target.x - source.x)/time;
	m_scalingVelocity.y = (target.y - source.y)/time;
	m_scalingVelocity.z = (target.z - source.z)/time;

	this->m_objectAnimationState = Animation::SCALING;
}

void Animation::MoveAndRotateTo(Vector3 translationTarget,Vector3 rotationTarget, float time)
{
	//this->MoveTo(this->transform.position,translationTarget,time);
	//this->RotateTo(this->transform.worldEulerRotation,rotationTarget,time);
	this->m_objectAnimationState = Animation::MOVENROTATE;
}

void Animation::MoveAndScaleTo(Vector3 translationTarget,Vector3 scaleTarget, float time)
{
	//this->MoveTo(this->transform.position,translationTarget,time);
	//this->ScaleTo(this->transform.scale,scaleTarget,time);
	this->m_objectAnimationState = Animation::MOVENSCALE;

}

void Animation::RotateAndScaleTo(Vector3 rotationTarget,Vector3 scaleTarget, float time)
{
	//this->RotateTo(this->transform.worldEulerRotation,rotationTarget,time);
	//this->ScaleTo(this->transform.scale,scaleTarget,time);
	this->m_objectAnimationState = Animation::ROTATENSCALE;
}

void Animation::ManualMove(Vector3 direction, float speed)
{
	m_velocity.x = direction.x * speed; 
	m_velocity.y = direction.y * speed; 
	m_velocity.z = direction.z * speed; 
	this->m_objectAnimationState = Animation::MOVING;
	m_UserDefined = true;
}

void Animation::ManualMove(Vector3 direction, float speed,float time)
{
	m_velocity.x = direction.x * speed; 
	m_velocity.y = direction.y * speed; 
	m_velocity.z = direction.z * speed; 
	this->m_objectAnimationState = Animation::MOVING;
	//m_UserDefined = true;
}

void Animation::ManualRotate(Vector3 rotationAxis, float speed)
{
	m_rotationVelocity.x = rotationAxis.x * speed; 
	m_rotationVelocity.y = rotationAxis.y * speed; 
	m_rotationVelocity.z = rotationAxis.z * speed; 

	this->m_objectAnimationState = Animation::ROTATING;

	m_UserDefined = true;
}

void Animation::ManualRotateLocal(Vector3 rotationAxis, float speed)
{
	m_LocalRotationVelocity.x = rotationAxis.x * speed; 
	m_LocalRotationVelocity.y = rotationAxis.y * speed; 
	m_LocalRotationVelocity.z = rotationAxis.z * speed; 



	this->m_objectAnimationState = Animation::ROTATING;

	m_UserDefined = true;
}

void Animation::ManualRotate(Vector3 rotationAxis,float speed ,float time)
{
	m_rotationVelocity.x = rotationAxis.x * speed; 
	m_rotationVelocity.y = rotationAxis.y * speed; 
	m_rotationVelocity.z = rotationAxis.z * speed; 

	this->m_objectAnimationState = Animation::ROTATING;

	//m_UserDefined = true;
}

void Animation::ManualScale(Vector3 axis, float speed)
{
	m_scalingVelocity.x = axis.x * speed; 
	m_scalingVelocity.y = axis.y * speed; 
	m_scalingVelocity.z = axis.z * speed; 
	this->m_objectAnimationState = Animation::SCALING;
	m_UserDefined = true;
}

void Animation::ManualScale(Vector3 axis, float speed,float time)
{
	m_scalingVelocity.x = axis.x * speed; 
	m_scalingVelocity.y = axis.y * speed; 
	m_scalingVelocity.z = axis.z * speed; 
	this->m_objectAnimationState = Animation::SCALING;
	m_UserDefined = true;
}

void Animation::ManualMoveAndRotateTo(Vector3 direction,Vector3 rotationAxis, float moveSpeed,float rotationSpeed)
{
	m_velocity.x = direction.x * moveSpeed; 
	m_velocity.y = direction.y * moveSpeed; 
	m_velocity.z = direction.z * moveSpeed; 

	m_rotationVelocity.x = rotationAxis.x * rotationSpeed; 
	m_rotationVelocity.y = rotationAxis.y * rotationSpeed; 
	m_rotationVelocity.z = rotationAxis.z * rotationSpeed; 


	this->m_objectAnimationState = Animation::MOVENROTATE;
	m_UserDefined = true;
}

void Animation::ManualMoveAndScaleTo(Vector3 direction,Vector3 axis, float moveSpeed,float scaleSpeed)
{
	m_velocity.x = direction.x * moveSpeed; 
	m_velocity.y = direction.y * moveSpeed; 
	m_velocity.z = direction.z * moveSpeed; 

	m_scalingVelocity.x = axis.x * scaleSpeed; 
	m_scalingVelocity.y = axis.y * scaleSpeed; 
	m_scalingVelocity.z = axis.z * scaleSpeed; 

	this->m_objectAnimationState = Animation::MOVENSCALE;
	m_UserDefined = true;

}

void Animation::ManualRotateAndScaleTo(Vector3 rotationAxis,Vector3 axis, float rotationSpeed,float scaleSpeed)
{

	m_rotationVelocity.x = rotationAxis.x * rotationSpeed; 
	m_rotationVelocity.y = rotationAxis.y * rotationSpeed; 
	m_rotationVelocity.z = rotationAxis.z * rotationSpeed; 

	m_scalingVelocity.x = axis.x * scaleSpeed; 
	m_scalingVelocity.y = axis.y * scaleSpeed; 
	m_scalingVelocity.z = axis.z * scaleSpeed; 

	this->m_objectAnimationState = Animation::ROTATENSCALE;
	m_UserDefined = true;
}

void Animation::Update(float deltaTime)
{
	//float deltaX = 0;
	//float deltaY = 0;
	//float deltaZ = 0;

	//float deltaRotX = 0;
	//float deltaRotY = 0;
	//float deltaRotZ = 0;

	//float deltaScaX = 0;
	//float deltaScaY = 0;
	//float deltaScaZ = 0;


	//if(this->m_objectAnimationState == Animation::IDLE)  
	//{
	//}

	//if(this->m_objectAnimationState == Animation::MOVING || 
	//	this->m_objectAnimationState == Animation::MOVENROTATE || 
	//	this->m_objectAnimationState == Animation::MOVENSCALE)  
	//{

	//	//1. Move the object based on velocity (and acceleration?)
	//	deltaX = this->m_velocity.x * deltaTime;
	//	deltaY = this->m_velocity.y * deltaTime;
	//	deltaZ = this->m_velocity.z * deltaTime;

	//	this->transform.position.x += deltaX;
	//	this->transform.position.y += deltaY;
	//	this->transform.position.z += deltaZ;
	//}

	//if(this->m_objectAnimationState == Animation::ROTATING || 
	//	this->m_objectAnimationState == Animation::MOVENROTATE 
	//	|| this->m_objectAnimationState == Animation::ROTATENSCALE)
	//{
	//	// 1. Move the object based on velocity (and acceleration?)
	//	deltaRotX = this->m_rotationVelocity.x * deltaTime;
	//	deltaRotY = this->m_rotationVelocity.y * deltaTime;
	//	deltaRotZ = this->m_rotationVelocity.z * deltaTime;

	//	float localDeltaRotX = this->m_LocalRotationVelocity.x * deltaTime;
	//	float localDeltaRotY = this->m_LocalRotationVelocity.y * deltaTime;
	//	float localDeltaRotZ = this->m_LocalRotationVelocity.z * deltaTime;
	//	
	//	this->transform.worldEulerRotation.x += deltaRotX;
	//	this->transform.worldEulerRotation.y += deltaRotY;
	//	this->transform.worldEulerRotation.z += deltaRotZ;

	//	this->transform.localEulerRotation.x += localDeltaRotX;
	//	this->transform.localEulerRotation.y += localDeltaRotY;
	//	this->transform.localEulerRotation.z += localDeltaRotZ;
	//}
	//if(this->m_objectAnimationState == Animation::SCALING 
	//	|| this->m_objectAnimationState == Animation::ROTATENSCALE
	//	|| this->m_objectAnimationState == Animation::MOVENSCALE)  
	//{
	//	// 1. Move the object based on velocity (and acceleration?)
	//	deltaScaX = this->m_scalingVelocity.x * deltaTime;
	//	deltaScaY = this->m_scalingVelocity.y * deltaTime;
	//	deltaScaZ = this->m_scalingVelocity.z * deltaTime;

	//	this->transform.scale.x += deltaScaX;
	//	this->transform.scale.y += deltaScaY;
	//	this->transform.scale.z += deltaScaZ;
	//}

	//if(this->m_currentExecutionIndex != m_CommandList.size())
	//{
	//	AnimationCommand currentCommand = m_CommandList[m_currentExecutionIndex];
	//	if(currentCommand.m_command == "MoveTo")
	//	{
	//		if(Vector3::IsEqual(this->transform.position,currentCommand.m_destination))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		}
	//		m_translationCommandTime += deltaTime;
	//		if(currentCommand.m_accelerationType != "")
	//			CalculateVelocity(this->m_velocity,m_translationCommandTime);
	//	}
	//	else if(currentCommand.m_command == "RotateTo")
	//	{	
	//		if(Vector3::IsEqual(this->transform.worldEulerRotation,currentCommand.m_targetRotation))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		} 
	//		m_rotationCommandTime += deltaTime;
	//		if(currentCommand.m_accelerationType != "")
	//			CalculateVelocity(this->m_rotationVelocity,m_rotationCommandTime);
	//	}
	//	else if(currentCommand.m_command == "ScaleTo")
	//	{
	//		if(Vector3::IsEqual(this->transform.scale,currentCommand.m_targetScale))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		}
	//		m_scalingCommandTime += deltaTime;
	//		if(currentCommand.m_accelerationType != "")
	//			CalculateVelocity(this->m_scalingVelocity,m_scalingCommandTime);
	//	}
	//	else if(currentCommand.m_command == "MoveAndRotateTo")
	//	{
	//		if(Vector3::IsEqual(this->transform.worldEulerRotation,currentCommand.m_targetRotation))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		}
	//		m_moveRotateCommandTime += deltaTime;
	//	}
	//	else if(currentCommand.m_command == "MoveAndScaleTo")
	//	{
	//		if(Vector3::IsEqual(this->transform.scale,currentCommand.m_targetScale))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		}
	//		m_moveScaleCommandTime += deltaTime;
	//	}
	//	else if(currentCommand.m_command == "RotateAndScaleTo")
	//	{
	//		if(Vector3::IsEqual(this->transform.scale,currentCommand.m_targetScale))
	//		{
	//			m_currentExecutionIndex += 1;
	//			this->ExecuteCommands(m_currentExecutionIndex);
	//		}
	//		m_rotateScaleCommandTime += deltaTime;
	//	}

	//	this->m_previousObjectAnimationState = this->m_objectAnimationState;
	//}
	//else
	//{
	//	if(!m_UserDefined)
	//		this->m_objectAnimationState = IDLE;
	//}

}

void Animation::ExecuteCommands(int currentIndex)
{

	//if( m_currentExecutionIndex != m_CommandList.size())
	//{
	//	AnimationCommand command =  m_CommandList[currentIndex];
	//	if(command.m_command == "MoveTo")
	//	{
	//		std::string name = Name;
	//		m_translationCommandTime = 0;
	//		if(this->m_velocity.Magnitude() > 0)
	//				this->m_velocity = Vector3::Zero;
	//		this->MoveTo(this->transform.position,command.m_destination,command.time);
	//		if(command.m_accelerationType != "")
	//			this->CreateAccelerationCurve(this->m_velocity,command.time,command.m_accelerationSteps,command.m_accelerationType);
	//	}
	//	else if(command.m_command == "RotateTo")
	//	{
	//		std::string name = Name;
	//		m_rotationCommandTime = 0;
	//		this->RotateTo(this->transform.worldEulerRotation,command.m_targetRotation,command.time);
	//		if(command.m_accelerationType != "")
	//			this->CreateAccelerationCurve(this->m_rotationVelocity,command.time,command.m_accelerationSteps,command.m_accelerationType);
	//	}
	//	else if(command.m_command == "ScaleTo")
	//	{
	//		std::string name = Name;
	//		m_scalingCommandTime = 0;
	//		this->ScaleTo(this->transform.scale,command.m_targetScale,command.time);
	//		if(command.m_accelerationType != "")
	//			this->CreateAccelerationCurve(this->m_scalingVelocity,command.time,command.m_accelerationSteps,command.m_accelerationType);
	//	}
	//	else if(command.m_command == "MoveAndRotateTo")
	//	{
	//		std::string name = Name;
	//		m_moveRotateCommandTime = 0;
	//		this->MoveAndRotateTo(command.m_destination,command.m_targetRotation,command.time);
	//	}
	//	else if(command.m_command == "MoveAndScaleTo")
	//	{
	//		std::string name = Name;
	//		m_moveScaleCommandTime = 0;
	//		this->MoveAndScaleTo(command.m_destination,command.m_targetScale,command.time);
	//	}
	//	else if(command.m_command == "RotateAndScaleTo")
	//	{
	//		std::string name = Name;
	//		m_rotateScaleCommandTime = 0;
	//		this->RotateAndScaleTo(command.m_targetRotation,command.m_targetScale,command.time);
	//	}
	//}
	//else
	//{

	//}
}

void Animation::CreateAccelerationCurve(Vector3 finalVelocity,float commandExecutionTime,int noOfSteps, std::string curveType)
{
	float oneStepTime = commandExecutionTime / (float)noOfSteps;

	float xVelStep = finalVelocity.x/noOfSteps;
	float yVelStep = finalVelocity.y/noOfSteps;
	float zVelStep = finalVelocity.z/noOfSteps;

	Vector3 tempVelocity = Vector3::Zero;
	float time = 0;
	//velocityTable.clear();
	//timeTable.clear();
	if(curveType == "easeIn")
		tempVelocity = finalVelocity;


	for (int index = 0; index != noOfSteps;index++)
	{
		//this->velocityTable.push_back(tempVelocity);
		//this->timeTable.push_back(time);

		if(curveType == "easeOut")
		{
			tempVelocity.x += xVelStep;
			tempVelocity.y += yVelStep;
			tempVelocity.z += zVelStep;
		}

		if(curveType == "easeIn")
		{
			tempVelocity.x -= xVelStep;
			tempVelocity.y -= yVelStep;
			tempVelocity.z -= zVelStep;
		}


		time += oneStepTime;
	}


}

void Animation::CalculateVelocity(Vector3 &velocity,float time)
{
	float max = 0;
	float min = 0;
	Vector3 maxVelocity = Vector3::Zero; 
	Vector3 minVelocity = Vector3::Zero; 
	//std::vector<float>::iterator timeIt;

	//for (int timeIt = 0; timeIt != timeTable.size();timeIt++)
	//{
	//	//if(timeTable[timeIt] < time)
	//	//{

	//	//}
	//	//else
	//	//{
	//	//	/*max = timeTable[timeIt];
	//	//	min = timeTable[timeIt - 1];
	//	//	maxVelocity = velocityTable[timeIt];
	//	//	minVelocity = velocityTable[timeIt - 1];*/
	//	//	velocity.x = velocityTable[timeIt].x;
	//	//	velocity.y = velocityTable[timeIt].y;
	//	//	velocity.z = velocityTable[timeIt].z;
	//	//	break;
	//	//}
	//}

	/*float diff = max - min;
	float diff2 = max - time;
	Vector3 diffVelocity = maxVelocity - minVelocity;

	velocity.x += diff2*diffVelocity.x;
	velocity.y += diff2*diffVelocity.y;
	velocity.z += diff2*diffVelocity.z;*/



}

void Animation::Play()
{
//	this->m_objectAnimationState = this->m_previousObjectAnimationState;
}

void Animation::Stop()
{
	//this->m_objectAnimationState = AnimationState::IDLE;	
}


//////////////////////////////////////////////////////////////////////////
// All FBX Animation Methods
//////////////////////////////////////////////////////////////////////////
void Animation::LoadSkeletalAnimationFromFile(std::string fileName)
{
	//SkeletalAnimation* anim = NULL; 
	//std::string actualFileName =  Helper::SplitFilename(fileName);
	//anim = LoadAnimationFromFbx(fileName.c_str());

	//if(anim != NULL)
	//{
	//	m_AnimationList[actualFileName] = anim;
	//}
}

/// <summary>
/// Updates Animation.
/// </summary>
void Animation::AnimationUpdate()
{
	Mesh* mMesh = localReferenceToRenderer->mMesh;

	if(mCurrentAnimation != NULL && mMesh != NULL)
	{
		float oldTime = mCurrentTime;
		mCurrentTime = oldTime;

		//find the appropriate frame
		mCurrentTime += g_globalDeltaTime * mAnimRate;

		while(mCurrentTime >= mCurrentAnimation->mDuration)
		{
			mCurrentTime -= mCurrentAnimation->mDuration;
		}
		int frame = 0;
		while(mCurrentTime > mCurrentAnimation->mKeys[frame].mTime && frame < mCurrentAnimation->mNumKeys)
		{
			++frame;
		}

		if(frame >= mCurrentAnimation->mNumKeys)
		{
			frame = 0;
		}
		if(!mFrameInterpolate)
		{
			mCurrentFrame = static_cast<float>(frame);
			// we've got the frame, so update the animation   
			for(int i = 0; i < mMesh->mNoOfBones; ++i)
			{
				if(mChannelMap[i] != -1)
				{
					BonePose* jnt = &(mCurrentAnimation->mKeys[frame].mBones[mChannelMap[i]]);
					localReferenceToRenderer->mCurrentBones[i].translation = jnt->translation;
					localReferenceToRenderer->mCurrentBones[i].rotation = jnt->rotation;
					localReferenceToRenderer->mCurrentBones[i].scale = jnt->scale;
				}
			}
		}
		else
		{
			//frame interpolation
			int prevFrame = 0;
			float t = 0;
			if(frame > 0)
			{
				prevFrame = frame - 1;
				t = (mCurrentTime - mCurrentAnimation->mKeys[prevFrame].mTime) / (mCurrentAnimation->mKeys[frame].mTime - mCurrentAnimation->mKeys[prevFrame].mTime);
			}

			mCurrentFrame = static_cast<float>(frame) + t; //this is just for show in the title-bar

			// we've got the frame, so update the animation   
			for(int i = 0; i < mMesh->mNoOfBones; ++i)
			{
				if(mChannelMap[i] != -1)
				{
					BonePose* jntA = &(mCurrentAnimation->mKeys[prevFrame].mBones[mChannelMap[i]]);
					BonePose* jntB = &(mCurrentAnimation->mKeys[frame].mBones[mChannelMap[i]]);

					XMStoreFloat3(&localReferenceToRenderer->mCurrentBones[i].translation, DirectX::XMVectorLerp(XMLoadFloat3(&jntA->translation), XMLoadFloat3(&jntB->translation), t));
					XMStoreFloat4(&localReferenceToRenderer->mCurrentBones[i].rotation, DirectX::XMQuaternionSlerp(XMLoadFloat4(&jntA->rotation), XMLoadFloat4(&jntB->rotation), t));
					XMStoreFloat3(&localReferenceToRenderer->mCurrentBones[i].scale, DirectX::XMVectorLerp(XMLoadFloat3(&jntA->scale), XMLoadFloat3(&jntB->scale), t));
				}
			}
		}
	}
	

	//update the current pose
	for(int i = 0; i < mMesh->mNoOfBones; ++i)
	{
		DirectX::XMMATRIX m = localReferenceToRenderer->mCurrentBones[i].GetTransform();
		if(mMesh->mSkeleton[i].parent >= 0)
		{
			DirectX::XMMATRIX c = XMLoadFloat4x4(&localReferenceToRenderer->mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
			m *= c;
		}
		XMStoreFloat4x4(&localReferenceToRenderer->mCurrentGlobalPose[i], m);
	}

	////update the skeleton debug draw lines
	//int b = 0;
	//for(int i = 0; i < mMesh->mNoOfBones; ++i)
	//{
	//	XMFLOAT3 tempPoint(0.0f, 0.0f, 0.0f);
	//	XMVECTOR temp = XMLoadFloat3(&tempPoint);
	//	XMMATRIX m = XMLoadFloat4x4(&localReferenceToRenderer->mCurrentGlobalPose[i]);
	//	XMStoreFloat4(&localReferenceToRenderer->mNewBoneVerts[b].m_Position, XMVector3Transform(temp, m));

	//	if(mMesh->mSkeleton[i].parent >= 0)
	//		m = XMLoadFloat4x4(&localReferenceToRenderer->mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
	//	XMStoreFloat4(&localReferenceToRenderer->mNewBoneVerts[b+1].m_Position, XMVector3Transform(temp, m));
	//	b += 2;
	//}

	////update the vertices's
	//if(mDrawBindPose)
	//{
	//	for(int i = 0; i < mMesh->mNoOfVertices; ++i)
	//	{
	//		//localReferenceToRenderer->mCurrentVerts[i].Pos = mMesh->mVerts[i].Pos;
	//		localReferenceToRenderer->mCurrentVertices[i].m_Position = mMesh->mPlyVertices[i].m_Position;

	//	}
	//}
}

void Animation::PlayAnimation(std::string name)
{
	std::map<std::string,SkeletalAnimation*>::iterator animIt;
	animIt = m_AnimationList.find(name);
	if(animIt != m_AnimationList.end())
	{
		SetAnimation(animIt->second);
	}
}

void Animation::SetAnimation(SkeletalAnimation* anim)
{
	mCurrentAnimation = anim;
	Mesh* mMesh = localReferenceToRenderer->mMesh;
	mCurrentTime = 0.0f;
	if(anim && mMesh)
	{
		//init channel-map
		if(!mChannelMap)
		{
			mChannelMap = new (std::nothrow) unsigned char[mMesh->mNoOfBones];
		}
		if(mChannelMap)
		{

			for(int i = 0; i < mMesh->mNoOfBones; ++i)
			{
				mChannelMap[i] = -1;
				for(int j = 0; j < anim->mNumBones; ++j)
				{
					if(strcmp(mMesh->mSkeleton[i].name, anim->mSkeleton[j].name) == 0)
					{
						mChannelMap[i] = j;
						break;
					}
				}
			}
		}
	}
	else
	{
		if(mChannelMap) delete [] mChannelMap;
		mChannelMap = NULL;
	}
}

IBehaviour* Animation::Clone()
{
	Animation* instance = new Animation;
	return instance;
}