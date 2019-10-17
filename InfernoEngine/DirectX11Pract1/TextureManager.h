#ifndef TextureManager_H_
#define TextureManager_H_

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Texture.h"
#include <map>

extern std::map<std::string,Texture> m_TexturesList;


Texture* GetTextureByName(std::string textureName);



#endif