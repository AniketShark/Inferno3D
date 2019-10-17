#include "TextureManager.h"


std::map<std::string,Texture> m_TexturesList;

Texture* GetTextureByName(std::string textureName)
{
	std::map<std::string,Texture>::iterator textureIt;
	textureIt = m_TexturesList.find(textureName);
	if(textureIt != m_TexturesList.end())
	{
		return &textureIt->second;
	}

	return NULL;
}