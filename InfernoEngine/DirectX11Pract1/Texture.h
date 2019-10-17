#ifndef Texture_H_
#define Texture_H_

#include "global.h"

class Texture
{

public:
	enum TextureType
	{
		DIFFUSEMAP,
		NORMALMAP
	};
	ID3D11ShaderResourceView* mTexture;
	Texture();
	Texture(const Texture&);
	~Texture();

	TextureType mTexturetype;
	void operator =(const Texture& other);
	void operator =(const Texture* other);
	bool Initialize(ID3D11Device*, std::string);
	void Shutdown();

};

#endif