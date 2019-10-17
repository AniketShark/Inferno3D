#include "Texture.h"

Texture::Texture()
{
	mTexture = NULL;
}
Texture::Texture(const Texture& other)
{
	this->mTexture = other.mTexture;
	this->mTexturetype = other.mTexturetype;
	
}
Texture::~Texture()
{

}

void Texture::operator= (const Texture& other)
{
	this->mTexture = other.mTexture;
	this->mTexturetype = other.mTexturetype;
}

void Texture::operator =(const Texture* other)
{
	this->mTexture = other->mTexture;
	this->mTexturetype = other->mTexturetype;
}


bool Texture::Initialize(ID3D11Device* device,std::string fileName)
{
	HRESULT result = S_OK;
	std::wstring stemp = std::wstring(fileName.begin(), fileName.end());
	LPCWSTR sw = stemp.c_str();

	// Load the texture in.
	result =  D3DX11CreateShaderResourceViewFromFile(device, sw, NULL, NULL, &mTexture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}



void Texture::Shutdown()
{
	// Release the texture resource.
	if(mTexture)
	{
		mTexture->Release();
		mTexture = 0;
	}

	return;
}
