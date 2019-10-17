// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-08-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="Renderer.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef Renderer_H_
#define Renderer_H_

#include "global.h"
#include "PlyUtility.h"
#include "Component.h"
#include "IBehavior.h"
#include "HelperFuncs.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "xnacollision.h"
/// <summary>
/// Class Renderer.
/// </summary>
class Renderer : public IBehaviour
{
public:
	Renderer();
	~Renderer();
	
	XNA::AxisAlignedBox mCollider;

	bool mIsDeformable;  //whether mesh is deformable
	bool mGpuSkinnig;  //Toggles between cpu and gpu skinning
	bool mIsRenderTexture;
	Mesh* mMesh;
	RenderInfo mRenderInfo; //render info of the mesh
	FBXConstantBuffer* mConstantBuffer;
	
	virtual void Initialize();
	virtual void Update();
	virtual void Destroy();
	virtual ComponentType GetType();
	virtual IBehaviour* Clone();
	void SetRenderInfo();

	//Updates the skinning info(vertices's) of the model based on current bone positions
	void UpdateSkinningInfo();

	//Scales vertices's of the object uniformly
	void ScaleUniformaly(float factor);

	//Set the vertex array and index array for private vertex/index buffer
	void SetDynamicVertexAndIndexBuffer();

	//Sets the render info of the object which has its own vertex buffer
	void InitVertexAndIndexBuffer();

	void CreateBuffers();
	Material* GetMaterial();
	void SetDiffuseColor(Vector4 color);
	void SetAmbientColor(Vector4 color);
	void SetPixelShader(ID3D11PixelShader* pixelShader);
	void SetVertexShader(ID3D11VertexShader* vertexShader);
	void SetConstantBuffer(ID3D11Buffer* constantBuffer);
	void SetVertexLayout(ID3D11InputLayout* vertexLayout);
	void SetVertexStride(UINT vertexStride);
	void SetVertexOffset(UINT offset);
	void SetBlendState(ID3D11BlendState* blendState);
	void SetRenderTexture(ID3D11ShaderResourceView* textureSRV);
	void SetMainDiffuseTexture(ID3D11ShaderResourceView* textureSRV);
	void SetRasterizerState(ID3D11RasterizerState* rsState);
	void LoadMesh(std::string fileName);

	//Creates Plane procedurally
	void LoadPlane(int rows,int columns,int width,int height);
	// Draws Object on screen
	void Draw();


	//Change new current Vertices's
	Vertex* mCurrentVertices;

	// New Bone Vertices's
	Vertex* mNewBoneVerts;

	BonePose*              mCurrentBones; //saves changed bone positions

	_XMFLOAT4X4*            mCurrentGlobalPose; //saves changed bone matrix

	SimpleVertex*           mBoneVerts; //bone vertices's
	
	WORD*                   mBoneIndices; //bone indices

	_XMFLOAT4X4*            mCurrentBoneSkinningMatrix; //Skinning matrices for GPU skinning


	XMMATRIX      mExternalTransformationMatrix;

	XMMATRIX	  mTransformMatrix;
	/// <summary>
	/// Dynamic Vertex Buffer
	/// </summary>
	Vertex* mDynamicVertexBufferPly;
	/// <summary>
	/// Dynamic Vertex Buffer
	/// </summary>
	Vertex* mStaticVertexBuffer;

private:
	// Set vertex buffer
	UINT mVertexStride;
	UINT mVertexOffset;
	bool mIsTransperant;
	// vertex buffer for for simple vertex
	ID3D11Buffer* mVertexBuffer;

	ID3D11Buffer* mIndexBuffer; 

	// Vertex buffer for ply vertex which is actually used
	ID3D11Buffer* mVertexBufferVertex;

	ID3D11Buffer* mPrivateConstantBuffer;

	ID3D11Buffer* mPrivateChangingBuffer;

	ID3D11Buffer* mPrivateAnimationMatixBuffer; // constant buffer for Skinning matrices

	ID3D11VertexShader* mVertexShader;
	
	ID3D11PixelShader* mPixelShader;
	
	ID3D11InputLayout* mVertexLayout;
	
	ID3D11RasterizerState* mRasterizerState;

	ID3D11BlendState* mBlendState;

	ID3D11ShaderResourceView* mRenderTexture;

	Material* mMaterial;

};


#endif