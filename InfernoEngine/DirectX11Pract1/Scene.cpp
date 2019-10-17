// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-27-2014
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="Scene.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#include "Scene.h"
#include "PrefabManager.h"
#include <process.h>
/// <summary>
/// The registered game objects
/// </summary>
std::vector<GameObject*> registeredGameObjects;

/// <summary>
/// The registered light objects
/// </summary>
std::vector<LightInfo*> lightsInScene;
/// <summary>
/// The temporary component map
/// </summary>
std::map<ComponentType,IBehaviour*> tempComponentMap;
/// <summary>
/// The m_ prefab game object map
/// </summary>
std::map<std::string,GameObject*> prefabGameObjectMap;

/// <summary>
/// The prefab location
/// </summary>
std::string prefabLocation = "Prefabs/";


CRITICAL_SECTION saveSection;

/// <summary>
/// Initializes a new instance of the <see cref="Scene" /> class.
/// </summary>
Scene::Scene()
{
}
/// <summary>
/// Finalizes an instance of the <see cref="Scene" /> class.
/// </summary>
Scene::~Scene()
{

}

/// <summary>
/// Loads the assets.
/// </summary>
void Scene::LoadAssets(bool threadedLoad)
{
	//Load Models from Directory
	if(threadedLoad)
	{
		threaded = true;
		if(!LoadModelsFromConfigFileThreaded("ModelsConfig.txt"))
		{
			MessageBox(NULL,L"Config file read failed",L"Error",MB_OK); 
			return;
		}
	}
	else
	{
		threaded = false;
		if(!LoadModelsFromConfigFileNormal("ModelsConfig.txt"))
		{
			MessageBox(NULL,L"Config file read failed",L"Error",MB_OK); 
			return;
		}
	}
	
	//LoadPrefabs();
}

/// <summary>
/// Loads the prefabs.
/// </summary>
void Scene::LoadPrefabs()
{
	//LoadPrefabsFromFile("file",m_PrefabGameObjectMap);
}

/// <summary>
/// Initializes this instance.
/// </summary>
void Scene::Initialize()
{
	std::vector<GameObject*>::iterator sceneObjectIt;
	for (sceneObjectIt = registeredGameObjects.begin(); sceneObjectIt != registeredGameObjects.end();sceneObjectIt++)
	{
		tempComponentMap = (*sceneObjectIt)->GetComponentMap();
		if(tempComponentMap.size() != 0)
		{
			std::map<ComponentType,IBehaviour*>::iterator componentIt;
			for(componentIt = tempComponentMap.begin(); componentIt != tempComponentMap.end();componentIt++)
			{
				componentIt->second->Initialize();
			}
		}
	}
}

/// <summary>
/// Updates this instance.
/// </summary>
void Scene::Update()
{
	UpdateSceneLights();
	std::vector<GameObject*>::iterator sceneObjectIt;
	for (sceneObjectIt = registeredGameObjects.begin(); sceneObjectIt != registeredGameObjects.end();sceneObjectIt++)
	{
		tempComponentMap = (*sceneObjectIt)->GetComponentMap();
		if(tempComponentMap.size() != 0)
		{
			std::map<ComponentType,IBehaviour*>::iterator componentIt;
			for(componentIt = tempComponentMap.begin(); componentIt != tempComponentMap.end();componentIt++)
			{
				componentIt->second->Update();
			}
		}
	}
	
}
/// <summary>
/// Fixed the update.
/// </summary>
void Scene::FixedUpdate()
{

}

/// <summary>
/// Cleans up scene.
/// </summary>
void Scene::CleanUpScene()
{

}
/// <summary>
/// Registers the game object.
/// </summary>
/// <param name="go">The go.</param>
void Scene::RegisterGameObject(GameObject* go)
{
	registeredGameObjects.push_back(go);
}

/// <summary>
/// Unregister game object.
/// </summary>
/// <param name="go">The go.</param>
void Scene::UnRegisterGameObject(GameObject* go)
{

}

void Scene::RegisterLight(LightInfo* lightInfo)
{
	lightsInScene.push_back(lightInfo);
}

void Scene::UpdateSceneLights()
{
	g_pGlobalConstantBuffer.mNoOfLights = lightsInScene.size();
	g_pGlobalConstantBuffer.mPadding = 1;

	for (int index = 0; index < lightsInScene.size();index++)
	{
		g_pGlobalConstantBuffer.mlights[index].attenuation = lightsInScene[index]->attenuation;
		g_pGlobalConstantBuffer.mlights[index].power = lightsInScene[index]->power;
		g_pGlobalConstantBuffer.mlights[index].typeOfLight = lightsInScene[index]->typeOfLight;
		g_pGlobalConstantBuffer.mlights[index].vLightDirection = lightsInScene[index]->vLightDirection;
		g_pGlobalConstantBuffer.mlights[index].vPosition = lightsInScene[index]->vPosition;
		g_pGlobalConstantBuffer.mlights[index].range = lightsInScene[index]->range;
		g_pGlobalConstantBuffer.mlights[index].padding = lightsInScene[index]->padding;
		g_pGlobalConstantBuffer.mlights[index].material.mDiffuse = lightsInScene[index]->material.mDiffuse;
		g_pGlobalConstantBuffer.mlights[index].material.mAmbient = lightsInScene[index]->material.mAmbient;
	}
}

/// <summary>
/// Loads the game object form prefab recursively.
/// </summary>
/// <param name="val">The value.</param>
/// <param name="gameObject">The game object.</param>
void LoadGameObjectFormPrefabRecursively(json val,GameObject* gameObject)
{
	for (auto it = val.begin_members(); it != val.end_members(); it++)
	{
		if(it->name() == "Mesh")
		{
			gameObject->AddComponent(RenderComp);
		}
	}
}

