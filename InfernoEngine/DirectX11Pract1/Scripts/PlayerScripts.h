#ifndef PlayerScripts_H_
#define PlayerScripts_H_


#include "IBehavior.h"
#include "SharedInfo.h"
#include "global.h"
#include "CHRTimer.h"

class PlayerScript : public IBehaviour
{
public:

	float rotationX,rotationY,rotationZ;
	bool madePrefab;
	GameObject* crate;
	PlayerScript()
	{
		mThisGO = new SharedInfo();
		rotationX = 0;
		rotationY = 0;
		rotationZ = 0;
		m_type = Script;
	}
	~PlayerScript()
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

void PlayerScript::Initialize()
{
	
}

void PlayerScript::Update()
{
	g_input.Frame();

	if(g_input.GetKey(DIK_G))
	{
		mGameObjectPointer->GetComponent<Renderer>()->mEnabled = true;
	}
	if(g_input.GetKey(DIK_T))
	{
		mGameObjectPointer->GetComponent<Renderer>()->mEnabled = false;
	}

	if(g_input.GetKey(DIK_W))
	{
		mThisGO->transform->position.z += 0.1f;
	}
	if(g_input.GetKey(DIK_S))
	{
		mThisGO->transform->position.z -= 0.1f;
	}
	if(g_input.GetKey(DIK_A))
	{
		mThisGO->transform->position.x -= 0.1f;
	}
	if(g_input.GetKey(DIK_D))
	{
		mThisGO->transform->position.x += 0.1f;
	}

	if (g_input.GetKey(DIK_Q))
	{
		mThisGO->transform->position.y -= 0.1f;
	}
	if (g_input.GetKey(DIK_E))
	{
		mThisGO->transform->position.y += 0.1f;
	}

	//if(g_input.GetKey(DIK_LCONTROL))
	//{
	//	if(g_input.GetKey(DIK_P) && !madePrefab)
	//	{
	//		
	//	}
	//}
}

void PlayerScript::Destroy()
{
	if(mThisGO)
		delete mThisGO;
}

ComponentType PlayerScript::GetType()
{
	return m_type;
}

IBehaviour* PlayerScript::Clone()
{
	PlayerScript* instance = new PlayerScript;
	return instance;
}


#endif