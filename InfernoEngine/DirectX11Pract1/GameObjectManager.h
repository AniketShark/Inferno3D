// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-12-2014
// ***********************************************************************
// <copyright file="GameObjectManager.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef GameObjectManager_H_
#define GameObjectManager_H_

#include "global.h"
#include <fstream>
#include <map>
#include <windows.h>
#include "GameObject.h"
#include "Model_Inf.h"

extern std::string MODELS_LOCATION; 
extern std::map<std::string,Mesh*> mStaticMeshList;

extern std::map<std::string,Mesh*> mDynamicMeshList;

extern CRITICAL_SECTION assetLoadingCriticalSection;
extern CRITICAL_SECTION fbxCriticalSection;
extern bool threaded;

bool LoadModelsFromConfigFileThreaded(std::string configFile);

bool LoadModelsFromConfigFileNormal(std::string configFile);

bool CreateGlobalVertexBufferAndIndexBuffer();

bool LoadDataInVBAndIB(Model_Inf &inputModel,Vertex* vertexArray,DWORD* indexArray);

bool LoadTexturesAndCreateResourceViews(Mesh* mesh);

void CreateObject(std::string objectName);

bool CreateConstantBuffers(Vertex* vertexArray,WORD* indexArray);

void CreateObjectInstance(Model_Inf & modelObject,std::string modelName,Vector3 position,Vector3 rotation,Vector3 scale);

bool LevelBuilder(std::string fileName);

Mesh* GetMeshByName(std::string fileName);

void SaveMeshToList(Mesh* mesh);


#endif 