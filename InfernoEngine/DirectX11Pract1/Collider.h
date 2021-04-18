

#ifndef Collider_H_
#define Collider_H_

#include<DirectXCollision.h>
#include "Vector.h"
#include <vector>
#include "IBehavior.h"
#include <map>

enum ColliderType
{
	SphereCollider,
	BoxCollider,
	BoxColliderOriented
};

struct Collision
{
	Vector3 mCollisionPoint;
	GameObject* mCollidedTo;
};

struct Bounds
{
	Vector3 m_Center;
	float m_Radius;
	Vector3 m_Extents;
	Vector4 m_Orientation;
};

class Collider : public IBehaviour
{
public:
	Collider();
	Collider(ColliderType type);
	~Collider();
	
	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();
	virtual IBehaviour* Clone();

	Bounds mBounds;
	void AssignSphereCollider(Vector3 center,float radius);
	void AssignAxisAlignedBoxCollider(Vector3 center,Vector3 extents);
	void AssignOrientedBoxColliderCollider(Vector3 center,Vector3 extents,Vector4 orientation);
	ColliderType GetColliderType();
	Vector3 GetExtents();
	float GetRadius();
	Vector3 GetCenter();
	Vector4 GetOrientation();
	GUID GetColliderID();
	 
	DirectX::BoundingSphere* GetOuterBoundingSphere();
	DirectX::BoundingSphere* GetBoundingSphere();
	DirectX::BoundingBox* GetBoundingBox();
	DirectX::BoundingOrientedBox* GetBoundingBoxOriented();

	void SetBounds(Bounds bounds);
	void SetRadius(float radius);
	void SetCenter(Vector3 center);
	void SetExtents(Vector3 extents);
	// multiplies original extents by multiplier to encapsulate an object
	void SetOuterBigBoundingSphereMultiplier(float multiplier);
	void AddInRangeCollider(Collider*);

private:
	ColliderType m_ColliderType;
	DirectX::BoundingSphere* m_BoundingSphere;
	DirectX::BoundingBox* m_BoundingBox;
	DirectX::BoundingOrientedBox* m_OrientedBoundingBox;
	DirectX::BoundingSphere* m_OuterBigBoundingSphere;
	GUID mColliderID;
	Vector3 m_CachedPosition;
	std::map<GUID*,Collider*> m_InRangeColliders;

};

#endif