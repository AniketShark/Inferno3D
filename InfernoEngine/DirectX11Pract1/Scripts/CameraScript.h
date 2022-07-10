#ifndef CameraScript_H_
#define CameraScript_H_

#include "IBehavior.h"
#include "SharedInfo.h"
#include "global.h"
#include "Vector.h"
#include "CHRTimer.h"
#include "Console.h"

class CameraScript : public IBehaviour
{
public:
	Vector3 position;
	Vector3 rotation;
	float rotationX,rotationY,rotationZ;
	float moveSpeed;
	float rotationSpeed;
	Vector3 moveDirection;
	CameraScript()
	{
		mThisGO = new SharedInfo();
		rotationX = 0;
		rotationY = 0;
		rotationZ = 0;
		moveSpeed = 100;
		rotationSpeed = 5;
		m_type = Script;
	}
	~CameraScript()
	{
		if(mThisGO != NULL)
			delete mThisGO;
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();
	virtual IBehaviour* Clone();
};

void CameraScript::Initialize()
{

}

void CameraScript::Update()
{
	g_input.Frame();
	
	if(mThisGO != NULL)
	{
		if(g_input.GetKey(DIK_NUMPAD4))
		{
			mThisGO->transform->Translate(Vector3::Left,moveSpeed * g_globalDeltaTime);
		}
		else if(g_input.GetKey(DIK_NUMPAD6))
		{
			mThisGO->transform->Translate(Vector3::Right,moveSpeed * g_globalDeltaTime);
		}
		if(g_input.GetKey(DIK_NUMPAD8))
		{
			mThisGO->transform->Translate(Vector3::Forward,moveSpeed * g_globalDeltaTime);
		}
		else if(g_input.GetKey(DIK_NUMPAD5))
		{
			mThisGO->transform->Translate(Vector3::Backward,moveSpeed * g_globalDeltaTime);
		}
		if(g_input.GetKey(DIK_NUMPAD9))
		{
			mThisGO->transform->Translate(Vector3::Up, moveSpeed * g_globalDeltaTime);
		}
		else if(g_input.GetKey(DIK_NUMPAD7))
		{
			mThisGO->transform->Translate(Vector3::Down, moveSpeed * g_globalDeltaTime);
		}

		std::stringstream newatream;
		newatream<<"Position "<< mThisGO->transform->position.x<<","<< mThisGO->transform->position.y<<","<< mThisGO->transform->position.z;
		std::string str = newatream.str();
		std::wstring w = std::wstring(str.begin(),str.end());
		SetWindowText(g_hWnd,w.c_str());
		//ConsolePrint(newatream.str());
	}
}

void CameraScript::Destroy()
{

}

ComponentType CameraScript::GetType()
{
	return m_type;
}

IBehaviour* CameraScript::Clone()
{
	CameraScript* instance = new CameraScript;
	return instance;
}
#endif