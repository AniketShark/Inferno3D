// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-11-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="PrefabManager.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#ifndef PrefabManager_H_
#define PrefabManager_H_

#include "GameObjectManager.h"
#include "PrefabManager.h"
#include "jsoncons/json.hpp"
#include <string>


using jsoncons::json;

extern std::string prefabsListFileName;

extern std::string prefabFilesPath;

extern std::map<std::string,json> m_PrefabMap;

void CreatePrefab(GameObject* gameObject);

void LoadPrefabsFromFile(std::string fileName,GameObject* newObject);


#endif