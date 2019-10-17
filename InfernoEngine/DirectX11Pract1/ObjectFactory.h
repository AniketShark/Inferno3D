#ifndef ObjectFactory_H_
#define ObjectFactory_H_

#include "GameObject.h"

GameObject* Instantiate(GameObject* gameObject,Vector3 position,Vector3 rotation);

void Destroy(GameObject);

void Destroy(GameObject,float time);

#endif