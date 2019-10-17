#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct FBXConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

struct Bone
{
    _XMFLOAT4X4 mInverseBindPoseMatrix;
    int      parent;   
    char     name[28];

    Bone() : parent(0) 
    { name[0]='\0'; }
};

struct BonePose
{
    XMFLOAT3 translation;
    XMFLOAT4 rotation;
    XMFLOAT3 scale;

    XMMATRIX GetTransform() const;
};

struct SimpleSkinnedVertex
{
    XMFLOAT3      Pos;              
    XMFLOAT4      Color;

    unsigned char BoneIndex[4];    
    float         BoneWeight[4];

    SimpleSkinnedVertex() : Pos(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f) 
    { BoneIndex[0] = BoneIndex[1] = BoneIndex[2] = BoneIndex[3] = 0; BoneWeight[0] = BoneWeight[1] = BoneWeight[2] = BoneWeight[3] = 0.0f; }
};


extern FBXConstantBuffer          g_cb;

#endif
