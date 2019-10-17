

#ifndef Collider_H_
#define Collider_H_

#include "xnacollision.h"
#include "Vector.h"
#include <vector>
#include "IBehavior.h"
#include <map>
using namespace XNA;

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
	XNA::Sphere* GetOuterBoundingSphere();
	XNA::Sphere* GetBoundingSphere();
	XNA::AxisAlignedBox* GetBoundingBox();
	XNA::OrientedBox* GetBoundingBoxOriented();

	void SetBounds(Bounds bounds);
	void SetRadius(float radius);
	void SetCenter(Vector3 center);
	void SetExtents(Vector3 extents);
	// multiplies original extents by multiplier to encapsulate an object
	void SetOuterBigBoundingSphereMultiplier(float multiplier);
	void AddInRangeCollider(Collider*);

private:
	ColliderType m_ColliderType;
	XNA::Sphere* m_BoundingSphere;
	XNA::AxisAlignedBox* m_BoundingBox;
	XNA::OrientedBox* m_OrientedBoundingBox;
	
	XNA::Sphere* m_OuterBigBoundingSphere;
	GUID mColliderID;
	Vector3 m_CachedPosition;
	std::map<GUID*,Collider*> m_InRangeColliders;

};

#endif