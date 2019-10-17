#ifndef BunnyScript_H_
#define BunnyScript_H_

#include "IBehavior.h"
#include "SharedInfo.h"
#include "global.h"
#include "CHRTimer.h"
#include "GameObject.h"
#include "xnacollision.h"

class BunnyScript: public IBehaviour
{
public:

	float rotationX,rotationY,rotationZ;
	bool madePrefab;
	GameObject* camera;
	int noOfParticles;
	int clothStiffness;
	float factor;
	
	BunnyScript()
	{
		mThisGO = new SharedInfo();
		rotationX = 0;
		rotationY = 0;
		rotationZ = 0;
		m_type = Script;
		factor = 0.001f;
	}
	~BunnyScript()
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

void BunnyScript::Initialize()
{
	
}

void BunnyScript::Update()
{
	g_input.Frame();
	
	if(g_input.GetKey(DIK_1))
	{
		factor += factor;
		mThisGO->light->mLightInfo->attenuation = XMFLOAT4(factor,factor,factor,1);
	}
	if(g_input.GetKey(DIK_2))
	{
		factor -= factor;
		mThisGO->light->mLightInfo->attenuation =  XMFLOAT4(factor,factor,factor,1);
	}

	if(g_input.GetKey(DIK_Q))
	{
		mThisGO->light->mLightInfo->typeOfLight = 0;
	}
	if(g_input.GetKey(DIK_E))
	{
		mThisGO->light->mLightInfo->typeOfLight = 1;
	}

}

void BunnyScript::Destroy()
{
	if(mThisGO)
		delete mThisGO;
}

ComponentType BunnyScript::GetType()
{
	return m_type;
}

IBehaviour* BunnyScript::Clone() 
{
	BunnyScript* instance = new BunnyScript;
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