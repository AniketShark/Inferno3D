#ifndef CollisionManager_H_
#define CollisionManager_H_

#include "Vector.h"
#include "Collider.h"
#include "WorldCell.h"
#include <vector>
#include <map>




class CollisionWorld
{
public:
	 CollisionWorld();
	 ~CollisionWorld();

	 static std::vector<Collider*> m_SceneGameObjectList;
	 static std::map<int,WorldCell*> m_WorldCellList;

	static void RegisterGameobject(Collider*);
	static void RemoveGameObject(int id);
	static void UpdateCollisionWorld();
	
};
#endif