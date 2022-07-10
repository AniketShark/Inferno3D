#include "ShaderManager.h"
#include"Helper.h"

std::map<std::wstring,Shader> m_ShaderList;


bool LoadShadersFromDirectory(std::wstring shaderDataFile)
{

	std::wifstream file(shaderDataFile);

	if(!file.is_open())
	{
		MessageBox(NULL,L"Shader Loading failed",L"Error",MB_OK);
		return false;
	}

	std::wstring tempString;

	while (tempString != L"</ShaderSource>")
	{
		file>>tempString;
		if(tempString == L"<Shader>")
		{
			Shader shader;
			file>>shader.mShaderName;
			file>>shader.mVertexShaderEntryPonit;
			file>>shader.mPixelShaderEntryPoint;
			file>>shader.mNoOfTexturesShaderNeeds;
			shader.CreateVertexAndPixelShader();
			shader.mShaderId =	((int)Helper::RandomBetween0n1())%10000;
			m_ShaderList[shader.mShaderName] = shader;
		}
	}

	return true;
}

Shader* FindShader(std::wstring shaderName)
{
	std::map<std::wstring,Shader>::iterator shaderIt = m_ShaderList.find(shaderName);

	if(shaderIt != m_ShaderList.end())
	{
		return &shaderIt->second;

	}
	else 
		return NULL;
}

