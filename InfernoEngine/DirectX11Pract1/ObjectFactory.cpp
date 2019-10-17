#include "ObjectFactory.h"
#include "Scene.h"

GameObject* Instantiate(GameObject* gameobject,Vector3 position,Vector3 rotation)
{
	GameObject* instance =  gameobject->Clone();
	instance->transform.position = position;
	instance->transform.SetWorldRotationQuaternion(rotation);
	return instance;
}

void Destroy(GameObject)
{

}

void Destroy(GameObject,float time)
{

}

