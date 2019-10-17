// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-08-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-15-2014
// ***********************************************************************
// <copyright file="GameObject.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef GameObject_H_
#define  GameObject_H_


#include <vector>
#include <map>
#include <exception>
#include "Object.h"
#include "Vector.h"
#include "Transform.h"
#include "Renderer.h"
#include "Animation.h"
#include "Helper.h"
#include "IBehavior.h"
#include "Camera.h"
#include "Cloth.h"
#include "Light.h"
#include "Collider.h"
/// <summary>
/// Class GameObject.
/// </summary>
class GameObject : public CoreObject
{
public:
	GameObject();
	GameObject(std::string plyFileName);
	GameObject(const GameObject&);
	bool operator== (GameObject go);
	void operator= (GameObject go);
	void operator= (GameObject* go);
	virtual ~GameObject();

	std::string Tag;
	int  noOfChilds; 
	GameObject* childs;
	
	int exp;
	int health;
	int level;

	//////////////////////////////////////////////////////////////////////////
	// Components
	//////////////////////////////////////////////////////////////////////////
	Transform transform;
	
	Renderer* renderer;
	
	Animation* animation;
	
	Camera* camera;
	
	Cloth* cloth;

	Light* light;
	
	Collider* collider;

	
	void AddComponent(ComponentType component);

	//////////////////////////////////////////////////////////////////////////
	// All Template functions
	//////////////////////////////////////////////////////////////////////////
	
	// Attaches script to the game object
	template <typename T>
	void AttachScript()
	{
		T* newScriptObject = new T;
		IBehaviour* newCastedPointer = NULL;
		try
		{
			newCastedPointer = dynamic_cast<IBehaviour*>(newScriptObject);
			newCastedPointer->transform =  &transform;
			m_InterlinkComponents(newCastedPointer);
			m_ComponentMap[Script] = newCastedPointer;
		}
		catch(std::exception &e)
		{
			std::string str = e.what();
			std::wstring error = std::wstring(str.begin(),str.end());
			MessageBox(NULL,error.c_str(),L"Error",MB_OK);
		}
	}

	// Finds and returns the component
	template <typename T>
	T* GetComponent()
	{
		std::map<ComponentType,IBehaviour*>::iterator componentIt;// = m_ComponentMap.find(component);

		for (componentIt = m_ComponentMap.begin(); componentIt != m_ComponentMap.end();componentIt++)
		{
			if(typeid(T).name() == typeid(*(componentIt->second)).name())
			{
				return dynamic_cast<T*>(componentIt->second);
			}
		}

		/*if(componentIt != m_ComponentMap.end())
		{
		return dynamic_cast<T*>(componentIt->second);
		}*/

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Registers to scene.
	/// </summary>
	void RegisterToScene();
	std::map<ComponentType,IBehaviour*> GetComponentMap();
	GameObject* Clone();
	void AddClonedComponent(ComponentType,IBehaviour*);
	void OnCollisionEnter(Collision collision);

private:
	std::map<ComponentType,IBehaviour*> m_ComponentMap;

	void m_InterlinkComponents(IBehaviour*);
};



#endif