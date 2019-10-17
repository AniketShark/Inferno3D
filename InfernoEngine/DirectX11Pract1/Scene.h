#ifndef Scene_H_
#define Scene_H_
#include "global.h"
#include "GameObjectManager.h"
//#include "PrefabManager.h"
#include <vector>
#include <map>

extern std::vector<GameObject*> registeredGameObjects;
extern std::vector<LightInfo*> lightsInScene;
extern std::map<ComponentType,IBehaviour*> tempComponentMap;
extern std::map<std::string,GameObject*> prefabGameObjectMap;
extern std::string prefabLocation;

class Scene 
{
public:
	Scene();
	~Scene();
	 
	//static void LoadAssets();
	static void LoadAssets(bool threaded);
	static void LoadPrefabs();
	static void Initialize();
	static void Update();
	static void FixedUpdate();
	static void CleanUpScene();
	static void RegisterGameObject(GameObject* go);
	static void UnRegisterGameObject(GameObject* go);
	static void RegisterLight(LightInfo* lightInfo);
	static void UpdateSceneLights();
	
private:
	
	 
};

#endif 