

#include "ShaderStructure.hlsl"

#define MAX_LIGHTS 10

Texture2D txDiffuse1 : register(t0);
Texture2D txDiffuse2 : register(t1); 
TextureCube skyBoxTexture : register( t2 );
Texture2D bumpMap : register( t3 );


SamplerState samLinear : register( s0 );

SamplerState samAnisotropic: register( s1 );

//--------------------------------------------------------------------------------------
// Constant Buffer Variables 
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix InverseTransposedWorld;
	float4 EyePosition;
	uint NoOfLights;
	uint NoOfBones;
	uint GpuSkinning;
	uint Padding;
	LightInfo lights[MAX_LIGHTS];
}

cbuffer ChangingBuffer: register( b1 )
{
	Material objectMaterial;
	float4 transperacyOn;
}

cbuffer AnimationMatrices: register( b2 )
{
	float4x4 boneMatrices[128];
}


struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Norm : NORMAL;
	float4 Color : COLOR0;
	float2 Texcoord: TEXCOORD0;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;

	//Animation Stuff
	uint4 BoneIndex : JOINTINDEXES;    
    float4 BoneWeight : JOINTWEIGHTS;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION;
    float4 Norm : NORMAL;
	float4 Color : COLOR0;
	float2 TexCord : TEXCOORD0;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	matrix matFinalMVP = mul(World,View);
    matFinalMVP = mul( matFinalMVP, Projection );

	output.Pos = mul(input.Pos,matFinalMVP);
    output.WorldPos = mul( input.Pos, World );
	output.Norm = mul(input.Norm,World);
    output.Norm = mul( input.Norm, InverseTransposedWorld);
    output.Color = objectMaterial.diffuse;
	output.TexCord = input.Texcoord;
	output.Tangent = input.Tangent;
	output.Binormal = input.Binormal;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float4 lightColor = float4(0,0,0,0);

	for(int index = 0;index != NoOfLights; index++)
	{
		if(lights[index].typeOfLight == 0.0f)
		lightColor +=  ParellelLight(objectMaterial,lights[index],input.WorldPos,input.Norm,EyePosition);
		if(lights[index].typeOfLight == 1.0f)
		lightColor +=  PointLight(objectMaterial,lights[index],input.WorldPos,input.Norm,EyePosition);
	}

	return input.Color * lightColor ;
}


