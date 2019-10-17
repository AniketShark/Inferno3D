// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-08-2014
// ***********************************************************************
// <copyright file="Shader.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef Shader_H_
#define Shader_H_


#include "global.h"
#include <map>
#include "Material.h"
#include "Texture.h"
#include "Helper.h"

bool CompileShaderFromFile( std::wstring shaderFileName, 
	std::wstring entryPoint, 
	std::wstring shaderModel, 
	ID3DBlob** ppBlobOut);

/// <summary>
/// Class Shader.
/// </summary>
class Shader
{
public:
	Shader();

	~Shader();

	/// <summary>
	/// The m shader name
	/// </summary>
	std::wstring mShaderName;
	/// <summary>
	/// The m vertex shader entry ponit
	/// </summary>
	std::wstring mVertexShaderEntryPonit;
	/// <summary>
	/// The m pixel shader entry point
	/// </summary>
	std::wstring mPixelShaderEntryPoint;
	/// <summary>
	/// The m no of textures shader needs
	/// </summary>
	int mNoOfTexturesShaderNeeds;
	/// <summary>
	/// The m shader identifier
	/// </summary>
	int mShaderId;
	/// <summary>
	/// The m needs normal map
	/// </summary>
	bool mNeedsNormalMap;

	/// <summary>
	/// The m specular color
	/// </summary>
	XMFLOAT4 mSpecularColor;
	/// <summary>
	/// The m diffuse color
	/// </summary>
	XMFLOAT4 mDiffuseColor;
	/// <summary>
	/// The m ambient color
	/// </summary>
	XMFLOAT4 mAmbientColor;

	/// <summary>
	/// The m vertex shader
	/// </summary>
	ID3D11VertexShader* mVertexShader;
	/// <summary>
	/// The m pixel shader
	/// </summary>
	ID3D11PixelShader* mPixelShader;
	/// <summary>
	/// The m rasterizer state
	/// </summary>
	ID3D11RasterizerState* mRasterizerState;

	/// <summary>
	/// The m array of textures
	/// </summary>
	Texture* mArrayOfTextures;

	void CopyPropertiesFromShader(Shader&);

	void CreateVertexAndPixelShader();
};



#endif