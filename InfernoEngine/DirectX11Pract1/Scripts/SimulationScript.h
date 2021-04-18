#ifndef SimulationScript_H_
#define SimulationScript_H_


#include "IBehavior.h"
#include "SharedInfo.h"
#include "global.h"
#include "CHRTimer.h"
#include "GameObject.h"
#include<DirectXCollision.h>

class SimulationScript: public IBehaviour
{
public:

	float rotationX,rotationY,rotationZ;
	bool madePrefab;
	GameObject* m_SpheresList;
	GameObject* mCollidingSphere;
	int noOfParticles;
	int clothStiffness;
	
	SimulationScript()
	{
		mThisGO = new SharedInfo();
		rotationX = 0;
		rotationY = 0;
		rotationZ = 0;
		m_type = Script;
	}
	~SimulationScript()
	{
		if(mThisGO)
			delete mThisGO;
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();
	IBehaviour* Clone();
};

void SimulationScript::Initialize()
{
	
}

void SimulationScript::Update()
{
	g_input.Frame();

	/*if(noOfParticles > 0)
	{
		for (int index = 0; index < noOfParticles;index++)
		{
			Vector3 position = m_SpheresList[index].transform.position;
			m_SpheresList[index].transform.position = mThisGO->cloth->GetParticleAt(index);

		}
	}
	if(g_input.GetKey(DIK_1))
	{
		clothStiffness += 5;
		if(clothStiffness > 100)
			clothStiffness = 100;
		mThisGO->cloth->StiffnessFactor(clothStiffness);
	}
	if(g_input.GetKey(DIK_2))
	{
		clothStiffness -= 5;
		if(clothStiffness < 0)
			clothStiffness = 2;
		mThisGO->cloth->StiffnessFactor(clothStiffness);
	}
	
	if(g_input.GetKey(DIK_4))
	{
		mThisGO->cloth->ApplyAccelerationInDirection(Vector3::Forward,8);
	}
	else
	{
		mThisGO->cloth->ApplyAccelerationInDirection(Vector3::Down,12);
	}*/
}

void SimulationScript::Destroy()
{
	if(mThisGO)
		delete mThisGO;
}

ComponentType SimulationScript::GetType()
{
	return m_type;
}

IBehaviour* SimulationScript::Clone()
{
	SimulationScript* instance = new SimulationScript;
	return instance;
}


//for(int i = 0; i < 500; i++)
//{
//	g_vecObjects[i+2].PosX = g_ParticleSystem.getParticle(i).x;
//	g_vecObjects[i+2].PosY = g_ParticleSystem.getParticle(i).y;
//	g_vecObjects[i+2].PosZ = g_ParticleSystem.getParticle(i).z;
//	vecColliders[i].Center.x = g_ParticleSystem.getParticle(i).x;
//	vecColliders[i].Center.y = g_ParticleSystem.getParticle(i).y;
//	vecColliders[i].Center.z = g_ParticleSystem.getParticle(i).z;
//
//	if (XNA::IntersectSphereAxisAlignedBox(&vecColliders[i], &cubeCollider))
//	{
//
//		g_ParticleSystem.getParticle( i ).y = g_vecObjects[1].PosY + 10; //0.0f;
//		g_ParticleSystem.getParticleOld( i ).y = g_vecObjects[1].PosY + 10; //0.0f;
//		//g_vecObjects[1].texture0Name = L"emma";
//		//g_vecObjects[0].texture0Name = L"emma";
//	}
//}


#endif