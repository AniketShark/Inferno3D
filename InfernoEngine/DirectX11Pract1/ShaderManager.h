

#ifndef ShaderManager_H_
#define ShaderManager_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Shader.h"
#include "Helper.h"


extern std::map<std::wstring,Shader> m_ShaderList;

bool LoadShadersFromDirectory(std::wstring shaderDataFile);

Shader* FindShader(std::wstring shaderName);


#endif