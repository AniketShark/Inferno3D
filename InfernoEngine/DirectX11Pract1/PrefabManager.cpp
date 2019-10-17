// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-11-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="PrefabManager.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#include "PrefabManager.h"
#include "JsonInterface.h"

/// <summary>
/// The prefabs list file name
/// </summary>
std::string prefabsListFileName = "PrefabsList.json";
/// <summary>
/// The prefab files path
/// </summary>
std::string prefabFilesPath = "Prefabs/";

/// <summary>
/// The m_ prefab map
/// </summary>
std::map<std::string,json> m_PrefabMap;

using jsoncons::json;

/// <summary>
/// Creates the prefab.
/// </summary>
/// <param name="gameObject">The game object.</param>
void CreatePrefab(GameObject* gameObject)
{

	std::string objectName =  gameObject->renderer->mMesh->mModelName;

	//json obj(json::an_array);
	json objectInfo;

	if(gameObject->renderer->mMesh->mFileType == FBX)
		objectInfo["FBX"] = true;
	if(gameObject->renderer->mMesh->mFileType == PLY)
		objectInfo["PLY"] = true;
	objectInfo["Exp"] = (rand()/RAND_MAX) * 1000; 
	objectInfo["Health"] = (rand()/RAND_MAX) * 100;
	objectInfo["Level"] =(rand()/RAND_MAX) * 30;

	objectInfo["PosX"] = gameObject->transform.position.x;
	objectInfo["PosY"] = gameObject->transform.position.y;
	objectInfo["PosZ"] = gameObject->transform.position.z;

	//saving rotation data
	objectInfo["RotX"] = gameObject->transform.worldEulerRotation.x;
	objectInfo["RotY"] = gameObject->transform.worldEulerRotation.y;
	objectInfo["RotZ"] = gameObject->transform.worldEulerRotation.z;

	//saving scale data
	objectInfo["ScaX"] = gameObject->transform.scale.x;
	objectInfo["ScaY"] = gameObject->transform.scale.y;
	objectInfo["ScaZ"] = gameObject->transform.scale.z;


	objectInfo["ModelLocation"] = "Models/" + objectName;

	objectInfo["Mesh"] = objectName;

	std::string fileSavePath = prefabFilesPath + objectName + ".json";
	std::string str = prefabFilesPath + prefabsListFileName;
	json prefabEntry;
	prefabEntry[objectName] = objectName;

	SaveObjectToJson(objectInfo,fileSavePath);

	SaveObjectToJson(prefabEntry,str);
} 

/// <summary>
/// Loads the prefabs from file.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <param name="newObject">The new object.</param>
void LoadPrefabsFromFile(std::string fileName,GameObject* newObject)
{
	//std::string actualFilePath = prefabFilesPath + prefabsListFileName;
	//json loadValue = LoadJsonFile(actualFilePath);

	//GameObject* newObject = new GameObject;

	//for (auto it = loadValue.begin_members();it != loadValue.end_members();it++)
	//{
		//std::string fileToLoad = prefabFilesPath + it->value().as_string()+".json";

		json val = LoadJsonFile(prefabFilesPath + fileName+".json");

		std::string objectName = "";
		if(!val.is_empty())
		{
			for(auto it = val.begin_members(); it != val.end_members();it++)
			{
				//////////////////////////////////////////////////////////////////////////
				// Position Data
				//////////////////////////////////////////////////////////////////////////
				if(it->name() == "PosX")
				{
					newObject->transform.position.x = it->value().as_double();
				}
				if(it->name() == "PosY")
				{
					newObject->transform.position.y = it->value().as_double();
				}
				if(it->name() == "PosZ")
				{
					newObject->transform.position.z = it->value().as_double();
				}

				if(it->name() == "Exp")
				{
					newObject->exp = it->value().as_int();
				}
				if(it->name() == "Level")
				{
					newObject->level = it->value().as_int();
				}
				if(it->name() == "Health")
				{
					newObject->health = it->value().as_int();
				}
				//////////////////////////////////////////////////////////////////////////
				// Rotation Data
				//////////////////////////////////////////////////////////////////////////
				if(it->name() == "RotX")
				{
					newObject->transform.worldEulerRotation.x = it->value().as_double();
				}
				if(it->name() == "RotY")
				{
					newObject->transform.worldEulerRotation.y = it->value().as_double();
				}
				if(it->name() == "RotZ")
				{
					newObject->transform.worldEulerRotation.z = it->value().as_double();
				}

				/////////////////////////////////////////////////////////////////////////
				// Scale Data
				//////////////////////////////////////////////////////////////////////////
				if(it->name() == "ScaX")
				{
					newObject->transform.scale.x = it->value().as_double();
				}
				if(it->name() == "ScaY")
				{
					newObject->transform.scale.y = it->value().as_double();
				}
				if(it->name() == "ScaZ")
				{
					newObject->transform.scale.z = it->value().as_double();
				}
				if(it->name() == "ModelLocation")
				{
					//newObject->transform.position.x = it->value().as_double();
				}

				//////////////////////////////////////////////////////////////////////////
				// Adding Components
				//////////////////////////////////////////////////////////////////////////
				/*if(it->name() == "RenderComp")
				{
					newObject->AddComponent(RenderComp);
				}
				if(it->name() == "CameraComp")
				{
					newObject->AddComponent(CameraComp);
				}
				if(it->name() == "AnimationComp")
				{
					newObject->AddComponent(AnimationComp);
				}
				if(it->name() == "FBX")
				{
					
				}
				if(it->name() == "PLY")
				{
					
				}*/
			}//for(val)
		}

			//prefabList[objectName] = newObject;
		//} //if(val.hasMember)
}


