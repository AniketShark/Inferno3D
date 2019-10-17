////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////

#include "Collider.h"
#include "SharedInfo.h"
#include "Helper.h"
#include "CollisionManager.h"

Collider::Collider()
{
	
	m_BoundingBox = NULL;
	m_BoundingSphere = NULL;
	mThisGO  = new SharedInfo;
	m_OrientedBoundingBox = NULL;
	m_CachedPosition = Vector3::Zero;
	HRESULT hr = CoCreateGuid(&mColliderID);
}

Collider::Collider(ColliderType type)
{
	if(type == SphereCollider)
	{
		m_BoundingSphere = new XNA::Sphere;
	}

	if(type == BoxCollider)
	{
		m_BoundingBox = new XNA::AxisAlignedBox;
	}

	if(type == BoxColliderOriented)
	{
		m_OrientedBoundingBox = new XNA::OrientedBox;
	}
}

Collider::~Collider()
{
	
}

void Collider::Initialize()
{
	m_OuterBigBoundingSphere = new XNA::Sphere;
	SetOuterBigBoundingSphereMultiplier(20);
	CollisionWorld::RegisterGameobject(this);
}

void Collider::Update()
{
	bool collided = false;
	if(!Vector3::IsEqual(m_CachedPosition,mThisGO->transform->position))
	{
		SetCenter(mThisGO->transform->position);
	}

	if(m_InRangeColliders.size() > 0)
	{
		std::map<GUID*,Collider*>::iterator colliderIt;
		for (colliderIt = m_InRangeColliders.begin(); colliderIt != m_InRangeColliders.end();colliderIt++)
		{
			if(m_ColliderType == BoxCollider )
			{
				if(colliderIt->second->GetColliderType() == BoxCollider)
				{
					if(XNA::IntersectAxisAlignedBoxAxisAlignedBox(this->m_BoundingBox,colliderIt->second->GetBoundingBox()))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == SphereCollider)
				{
					if(XNA::IntersectSphereAxisAlignedBox(colliderIt->second->GetBoundingSphere(),this->m_BoundingBox))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == BoxColliderOriented)
				{
					if(XNA::IntersectAxisAlignedBoxOrientedBox(this->m_BoundingBox,colliderIt->second->GetBoundingBoxOriented()))
						collided = true;
				}
			}
			if(m_ColliderType == SphereCollider )
			{
				if(colliderIt->second->GetColliderType() == BoxCollider)
				{
					if(XNA::IntersectSphereAxisAlignedBox(this->m_BoundingSphere,colliderIt->second->GetBoundingBox()))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == SphereCollider)
				{
					if(XNA::IntersectSphereSphere(colliderIt->second->GetBoundingSphere(),this->m_BoundingSphere))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == BoxColliderOriented)
				{
					if(XNA::IntersectSphereOrientedBox(this->m_BoundingSphere,colliderIt->second->GetBoundingBoxOriented()))
						collided = true;
				}
			}
			if(m_ColliderType == BoxColliderOriented )
			{
				if(colliderIt->second->GetColliderType() == BoxCollider)
				{
					if(XNA::IntersectAxisAlignedBoxOrientedBox(colliderIt->second->GetBoundingBox(),this->m_OrientedBoundingBox))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == SphereCollider)
				{
					if(XNA::IntersectSphereOrientedBox(colliderIt->second->GetBoundingSphere(),this->m_OrientedBoundingBox))
						collided = true;
				}
				if(colliderIt->second->GetColliderType() == BoxColliderOriented)
				{
					if(XNA::IntersectSphereOrientedBox(this->m_BoundingSphere,colliderIt->second->GetBoundingBoxOriented()))
						collided = true;
				}
			}

			if(collided)
			{
				Collision collision;
				collision.mCollidedTo =  colliderIt->second->mGameObjectPointer;
				mGameObjectPointer->OnCollisionEnter(collision);

				collision.mCollidedTo = mGameObjectPointer;
				colliderIt->second->mGameObjectPointer->OnCollisionEnter(collision);
			}
		}
	}
}

void Collider::Destroy()
{

}

ComponentType Collider::GetType()
{
	return m_type;
}

void Collider::AssignSphereCollider(Vector3 center,float radius)
{
	this->m_ColliderType = SphereCollider; 
	m_BoundingSphere = new XNA::Sphere;
	m_BoundingSphere->Center = Helper::ConvertVector3ToXMFLOAT3(center);
	m_BoundingSphere->Radius = radius;
}

void Collider::AssignAxisAlignedBoxCollider(Vector3 center,Vector3 extents)
{
	this->m_ColliderType = BoxCollider; 
	m_BoundingBox = new XNA::AxisAlignedBox;
	m_BoundingBox->Center =  Helper::ConvertVector3ToXMFLOAT3(center);
	m_BoundingBox->Extents =  Helper::ConvertVector3ToXMFLOAT3(extents);

}

void Collider::AssignOrientedBoxColliderCollider(Vector3 center,Vector3 extents,Vector4 orientation)
{
	this->m_ColliderType = BoxColliderOriented; 
	m_OrientedBoundingBox = new XNA::OrientedBox;
	m_OrientedBoundingBox->Center =  Helper::ConvertVector3ToXMFLOAT3(center);
	m_OrientedBoundingBox->Extents =  Helper::ConvertVector3ToXMFLOAT3(extents);
	m_OrientedBoundingBox->Orientation =  Helper::ConvertVector4ToXMFLOAT4(orientation);

}

ColliderType Collider::GetColliderType()
{
	return m_ColliderType;
}

XNA::Sphere* Collider::GetBoundingSphere()
{
	return m_BoundingSphere;
}

XNA::Sphere* Collider::GetOuterBoundingSphere()
{
	return m_OuterBigBoundingSphere;
}

XNA::AxisAlignedBox* Collider::GetBoundingBox()
{
	return m_BoundingBox;
}

XNA::OrientedBox* Collider::GetBoundingBoxOriented()
{
	return m_OrientedBoundingBox;
}

GUID Collider::GetColliderID()
{
	return mColliderID;
}

Vector3 Collider::GetExtents()
{
	if(m_ColliderType == BoxCollider)
	{
		return Helper::ConvertXMFLOAT3TOVector3(m_BoundingBox->Extents);
	}
	if(m_ColliderType == BoxColliderOriented)
	{
		return Helper::ConvertXMFLOAT3TOVector3(m_OrientedBoundingBox->Extents);
	}

	return Vector3::Zero;
}

float Collider::GetRadius()
{
	if(m_ColliderType == SphereCollider)
		return m_BoundingSphere->Radius;
	else
		return 0;
}

Vector3 Collider::GetCenter()
{
	if(m_ColliderType == SphereCollider)
		return  Helper::ConvertXMFLOAT3TOVector3(m_BoundingSphere->Center);
	if(m_ColliderType == BoxCollider)
		return  Helper::ConvertXMFLOAT3TOVector3(m_BoundingBox->Center);
	if(m_ColliderType == BoxColliderOriented)
		return  Helper::ConvertXMFLOAT3TOVector3(m_OrientedBoundingBox->Center);
	else
		return Vector3::Zero;
}

Vector4 Collider::GetOrientation()
{
	if(m_ColliderType == BoxColliderOriented)
	{
		Vector4 temp;
		temp.x = m_OrientedBoundingBox->Orientation.x;
		temp.y = m_OrientedBoundingBox->Orientation.y; 
		temp.z = m_OrientedBoundingBox->Orientation.z; 
		temp.w = m_OrientedBoundingBox->Orientation.w; 
		return temp;
	}
	else
		return Vector4::Zero;
}

void Collider::SetBounds(Bounds bounds)
{
	mBounds = bounds;
}

void Collider::SetRadius(float radius)
{
	if(m_ColliderType == SphereCollider)
	{
		m_BoundingSphere->Radius = radius;
	}
}

void Collider::SetCenter(Vector3 center)
{
	if(m_ColliderType == SphereCollider)
		m_BoundingSphere->Center =  Helper::ConvertVector3ToXMFLOAT3(center);
	if(m_ColliderType == BoxCollider)
		m_BoundingBox->Center =  Helper::ConvertVector3ToXMFLOAT3(center);
	if(m_ColliderType == BoxColliderOriented)
		m_OrientedBoundingBox->Center =  Helper::ConvertVector3ToXMFLOAT3(center);
}

void Collider::SetExtents(Vector3 extents)
{
	if(m_ColliderType == BoxCollider)
		m_BoundingBox->Extents =  Helper::ConvertVector3ToXMFLOAT3(extents);
	if(m_ColliderType == BoxColliderOriented)
		m_OrientedBoundingBox->Extents =  Helper::ConvertVector3ToXMFLOAT3(extents);
}

void Collider::SetOuterBigBoundingSphereMultiplier(float multiplier)
{
	if(m_ColliderType == SphereCollider)
	{
		m_OuterBigBoundingSphere->Radius = m_BoundingSphere->Radius * multiplier;
	}
	if(m_ColliderType == BoxCollider)
	{
		m_OuterBigBoundingSphere->Radius =  Helper::ConvertXMFLOAT3TOVector3(m_BoundingBox->Extents).Magnitude() * multiplier;
	}
	if(m_ColliderType == BoxColliderOriented)
	{
		m_OuterBigBoundingSphere->Radius =  Helper::ConvertXMFLOAT3TOVector3(m_OrientedBoundingBox->Extents).Magnitude() * multiplier;
	}
}

void Collider::AddInRangeCollider(Collider* collider)
{
	m_InRangeColliders[&collider->GetColliderID()] = collider;
}

IBehaviour* Collider::Clone()
{
	Collider* instance = new Collider(m_ColliderType);
	
	return instance;
}
