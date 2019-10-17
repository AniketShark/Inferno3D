#ifndef Component_H_
#define Component_H_

#include "Object.h"
#include "Transform.h"

enum ComponentType
{
	AnimationComp,
	Rigidbody,
	CameraComp,
	ColliderComp,
	RenderComp,
	LightComp,
	ClothComp,
	Script
};

class Component 
{
public:
	Transform* transform;
	virtual ~Component();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;
	virtual ComponentType GetType() = 0;
protected:
	ComponentType m_type;
	
};


#endif