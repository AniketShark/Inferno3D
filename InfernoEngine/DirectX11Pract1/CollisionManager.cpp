#include "CollisionManager.h"



std::vector<Collider*> CollisionWorld::m_SceneGameObjectList;
std::map<int,WorldCell*> CollisionWorld::m_WorldCellList;

CollisionWorld::CollisionWorld()
{

}
CollisionWorld::~CollisionWorld()
{

}
void CollisionWorld::RegisterGameobject(Collider* collider)
{
	m_SceneGameObjectList.push_back(collider);
}
void CollisionWorld::RemoveGameObject(int id)
{
	//m_SceneGameObjectList.push_back(gameObject);
}
void CollisionWorld::UpdateCollisionWorld()
{
	for (int i = 0; i != m_SceneGameObjectList.size();i++)
	{
		for (int j = 0; j != m_SceneGameObjectList.size();j++)
		{
			if(m_SceneGameObjectList[j]->GetColliderType() == SphereCollider)
			{
				if(XNA::IntersectSphereSphere(m_SceneGameObjectList[i]->GetOuterBoundingSphere(),m_SceneGameObjectList[j]->GetBoundingSphere()))
					m_SceneGameObjectList[i]->AddInRangeCollider(m_SceneGameObjectList[j]);
			}

			if(m_SceneGameObjectList[j]->GetColliderType() == BoxCollider)
			{
				if(XNA::IntersectSphereAxisAlignedBox(m_SceneGameObjectList[i]->GetOuterBoundingSphere(),m_SceneGameObjectList[j]->GetBoundingBox()))
					m_SceneGameObjectList[i]->AddInRangeCollider(m_SceneGameObjectList[j]);
			}

			if(m_SceneGameObjectList[j]->GetColliderType() == BoxColliderOriented)
			{
				if(XNA::IntersectSphereOrientedBox(m_SceneGameObjectList[i]->GetOuterBoundingSphere(),m_SceneGameObjectList[j]->GetBoundingBoxOriented()))
					m_SceneGameObjectList[i]->AddInRangeCollider(m_SceneGameObjectList[j]);
			}
		}
	}
}