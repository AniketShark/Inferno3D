// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-08-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="Renderer.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************


#include "Renderer.h"
#include "ShaderManager.h"
#include "SharedInfo.h"
#include "Shape.h"
#include "GameObjectManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="Renderer" /> class.
/// </summary>
Renderer::Renderer()
{
	mMesh  = NULL;
	mVertexBuffer = NULL;
	mIndexBuffer = NULL; 
	mPrivateConstantBuffer = NULL;
	mVertexShader = NULL;
	mPixelShader = NULL;
	mVertexLayout = NULL;
	mRasterizerState = NULL;
	mRenderTexture = NULL;
	mThisGO = new SharedInfo();
	mMaterial = new Material;
	mGpuSkinnig = false;
	mIsRenderTexture = false;
	
}

/// <summary>
/// Finalizes an instance of the <see cref="Renderer" /> class.
/// </summary>
Renderer::~Renderer()
{
	if(mVertexBuffer)
		mVertexBuffer->Release();
	if(mIndexBuffer)
		mIndexBuffer->Release();
	if(mPrivateConstantBuffer)
		mPrivateConstantBuffer->Release();

	if(mVertexShader)
		mVertexShader->Release();
	if(mPixelShader)
		mPixelShader->Release();
	if(mVertexLayout)
		mVertexLayout->Release();
	if(mRenderTexture)
		mRenderTexture->Release();

	if(mCurrentGlobalPose)
		delete mCurrentGlobalPose;
	if(mCurrentBones)
		delete mCurrentBones;
	if(mThisGO)
		delete mThisGO;
	if(mMesh)
		delete mMesh;
	if(mCurrentBoneSkinningMatrix)
		delete [] mCurrentBoneSkinningMatrix;

	if(mMaterial != NULL)
		delete mMaterial;
}

/// <summary>
/// Initializes this instance.
/// </summary>
void Renderer::Initialize()
{
	if(mRasterizerState == NULL)
		SetRasterizerState(g_pRS_SolidCullBack);
	if(mVertexShader == NULL)
		SetVertexShader(g_pVertexShader); 
	if(mVertexLayout == NULL)
		SetVertexLayout(g_pVertexLayout);
	if(mPixelShader == NULL)
		SetPixelShader(g_pPixelShaderLightingOnly);
}

/// <summary>
/// Updates this instance.
/// </summary>
void Renderer::Update()
{
	if(mMesh != NULL)
	{
		if(mMesh->mFileType ==  FBX && mMesh->mNoOfBones > 0)
			UpdateSkinningInfo();
		Draw();
	}

}

/// <summary>
/// Destroys this instance.
/// </summary>
void Renderer::Destroy()
{

}

/// <summary>
/// Gets the type.
/// </summary>
/// <returns>ComponentType.</returns>
ComponentType Renderer::GetType()
{
	return m_type;
}

Material* Renderer::GetMaterial()
{
	return mMaterial;
}
void Renderer::SetDiffuseColor(Vector4 color)
{
	if(mMaterial != NULL)
	{
		mMaterial->mDiffuse.x = color.x; 
		mMaterial->mDiffuse.y = color.y; 
		mMaterial->mDiffuse.z = color.z; 
		mMaterial->mDiffuse.w = color.w; 

		if(color.w < 1.0f)
			mIsTransperant = true;
		else
			mIsTransperant = false;
	}
	else
		MessageBox(NULL,L"The Object Doesn't Have Material",L"Material Error",MB_OK);
}
void Renderer::SetAmbientColor(Vector4 color)
{
	if(mMaterial != NULL)
	{
		mMaterial->mAmbient.x = color.x; 
		mMaterial->mAmbient.y = color.y; 
		mMaterial->mAmbient.z = color.z; 
		mMaterial->mAmbient.w = color.w; 
	}
	else
		MessageBox(NULL,L"The Object Doesn't Have Material",L"Material Error",MB_OK);
}

/// <summary>
/// Sets the render information.
/// </summary>
void Renderer::SetRenderInfo()
{
	if(mMesh != NULL)
	{
		mRenderInfo = mMesh->mRenderInfo;
	}
}

/// <summary>
/// Sets the dynamic vertex and index buffer.
/// </summary>
void Renderer::SetDynamicVertexAndIndexBuffer()
{
	HRESULT hr = S_OK;

	if(!mMesh)
	{
		return;
	}
	if(mMesh->mFileType == FBX && mMesh->mNoOfBones > 0 && mIsDeformable)
	{
		mCurrentGlobalPose = new (std::nothrow) DirectX::XMFLOAT4X4[mMesh->mNoOfBones];
		mCurrentBoneSkinningMatrix = new DirectX::XMFLOAT4X4[128];
		mCurrentBones = new (std::nothrow) BonePose[mMesh->mNoOfBones];
		mBoneIndices = new (std::nothrow) WORD[mMesh->mNoOfBones * 2];

		//Change
		mCurrentVertices = new (std::nothrow) Vertex[mMesh->mNoOfVertices];
		mNewBoneVerts = new (std::nothrow) Vertex[mMesh->mNoOfBones * 2];

		if(!mCurrentVertices || !mCurrentGlobalPose || !mCurrentBones || !mNewBoneVerts || !mBoneIndices)
		{
			return;
		}
		for(int i = 0; i < mMesh->mNoOfVertices; ++i)
		{
			mCurrentVertices[i].m_Position = mMesh->mVertices[i].m_Position;
			mCurrentVertices[i].m_Color = mMesh->mVertices[i].m_Color;
		}
		int b = 0;
		for(int i = 0; i < mMesh->mNoOfBones; ++i)
		{
			mCurrentGlobalPose[i] = mMesh->mInitialGlobalPose[i];
			mCurrentBones[i] = mMesh->mInitialBonePose[i];

			DirectX::XMFLOAT3 tempPoint(0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR temp = XMLoadFloat3(&tempPoint);
			DirectX::XMMATRIX m = XMLoadFloat4x4(&mCurrentGlobalPose[i]);
			XMStoreFloat4(&mNewBoneVerts[b].m_Position, DirectX::XMVector3Transform(temp, m));

			if(mMesh->mSkeleton[i].parent >= 0)
				m = XMLoadFloat4x4(&mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
			XMStoreFloat4(&mNewBoneVerts[b+1].m_Position, XMVector3Transform(temp, m));

			mNewBoneVerts[b].m_Color= DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			mNewBoneVerts[b+1].m_Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

			mBoneIndices[b] = b;
			mBoneIndices[b+1] = b+1;
			b += 2;
		}
	}

	if(mMesh->mFileType == PLY && mIsDeformable)
	{
		mCurrentVertices = new Vertex[mMesh->mNoOfVertices];
		mStaticVertexBuffer = new Vertex[mMesh->mNoOfVertices];
		for (int i = 0; i < mMesh->mNoOfVertices; i++)
		{
			mCurrentVertices[i].m_Position = mMesh->mVertices[i].m_Position;
			mCurrentVertices[i].m_Normal = mMesh->mVertices[i].m_Normal;
			mCurrentVertices[i].m_TexCoord = mMesh->mVertices[i].m_TexCoord;
			mCurrentVertices[i].m_Tangent = mMesh->mVertices[i].m_Tangent;

			mStaticVertexBuffer[i].m_Position = mMesh->mVertices[i].m_Position;
			mStaticVertexBuffer[i].m_Normal = mMesh->mVertices[i].m_Normal;
			mStaticVertexBuffer[i].m_TexCoord = mMesh->mVertices[i].m_TexCoord;
			mStaticVertexBuffer[i].m_Tangent = mMesh->mVertices[i].m_Tangent;
		}
	}
}

/// <summary>
/// Updates the skinning information.
/// </summary>
void Renderer::UpdateSkinningInfo()
{
	//////use skinning
	//////1-bone skinning
	////if(mSingleBoneSkinning)
	////{
	// for(int i = 0; i < mMesh->mNumVerts; ++i)
	// {
	//	 int b = mMesh->mVerts[i].JointIndex[0];

	//	 XMVECTOR v = XMLoadFloat3(&mMesh->mVerts[i].Pos);

	//	 // Convert vertex into local bone space by multiplying it 
	//	 // by the inverse bind-pose transform
	//	 XMMATRIX invBoneM = XMLoadFloat4x4(&mMesh->mSkeleton[b].invBindPose);
	//	 v = XMVector3Transform(v, invBoneM); 

	//	 // Convert back to current pose in model-space by multiplying
	//	 // it by the bone's current transform
	//	 XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[b]);
	//	 v = XMVector3Transform(v, curBoneM);

	//	 XMStoreFloat3(&mCurrentVerts[i].Pos, v);
	// }
	//}
	// else
	//{
	//multi-bone skinning
	//for(int i = 0; i < mMesh->mNumVerts; ++i)
	//{
	//	XMVECTOR vb = XMLoadFloat3(&mMesh->mVerts[i].Pos);
	//	XMVECTOR vc = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	//	for(int j = 0; j < 4; ++j)
	//	{
	//		int b = mMesh->mVerts[i].BoneIndex[j];

	//		// Convert vertex into local bone space by multiplying it 
	//		// by the inverse bind-pose transform
	//		XMMATRIX invBoneM = XMLoadFloat4x4(&mMesh->mSkeleton[b].invBindPose);
	//		XMVECTOR v = XMVector3Transform(vb, invBoneM); 

	//		// Convert back to current pose in model-space by multiplying
	//		// it by the bone's current transform
	//		XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[b]);
	//		v = XMVector3Transform(v, curBoneM);

	//		vc += v * mMesh->mVerts[i].BoneWeight[j];
	//	}
	//	XMStoreFloat3(&mCurrentVerts[i].Pos, vc);
	//}


	//Change

	if(mGpuSkinnig)
	{
		g_pGlobalConstantBuffer.mGpuSkinnig = 1;

		for(int boneArrIndex = 0; boneArrIndex < mMesh->mNoOfBones; ++boneArrIndex)
		{
			// Bones inverse bind pose matrix
			DirectX::XMMATRIX invBoneMatrix = XMLoadFloat4x4(&mMesh->mSkeleton[boneArrIndex].mInverseBindPoseMatrix);

			// Convert back to current pose in model-space by multiplying
			// it by the bone's current transform
			DirectX::XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[boneArrIndex]);

			//Skinning Matrices for this Pose
			DirectX::XMMATRIX skinningMatrix = XMMatrixTranspose(invBoneMatrix * curBoneM);

			//XMStoreFloat4x4(&mCurrentBoneSkinningMatrix[boneArrIndex],skinningMatrix);
			XMStoreFloat4x4(&g_pAnimationMatrices.mBoneMatrices[boneArrIndex],skinningMatrix);

		}
	}
	else
	{
		 g_pGlobalConstantBuffer.mGpuSkinnig = 0;
		for(int vertexIndex = 0; vertexIndex < mMesh->mNoOfVertices; ++vertexIndex)
		{

			DirectX::XMVECTOR vetexPositionVector = XMLoadFloat4(&mMesh->mVertices[vertexIndex].m_Position);
			DirectX::XMVECTOR vc = DirectX::XMVectorZero();

			for(int boneArrIndex = 0; boneArrIndex < 4; ++boneArrIndex)
			{
				//int b = mMesh->mVerts[i].BoneIndex[j];
				//Change
				int boneIndex = mMesh->mVertices[vertexIndex].BoneIndex[boneArrIndex];

				// Convert vertex into local bone space by multiplying it 
				// by the inverse bind-pose transform
				DirectX::XMMATRIX invBoneMatrix = XMLoadFloat4x4(&mMesh->mSkeleton[boneIndex].mInverseBindPoseMatrix);

				//vertex position in local space of bone
				DirectX::XMVECTOR v = XMVector3Transform(vetexPositionVector, invBoneMatrix);

				// Convert back to current pose in model-space by multiplying
				// it by the bone's current transform
				DirectX::XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[boneIndex]);
				v = DirectX::XMVector3Transform(v, curBoneM);

				int boneWeight = mMesh->mVertices[vertexIndex].BoneWeight[boneArrIndex];
				v  = DirectX::XMVectorScale(v, boneWeight);
				vc = DirectX::XMVectorAdd(vc,v);
			}
			XMStoreFloat4(&mCurrentVertices[vertexIndex].m_Position, vc);
		}
	}
}


/// <summary>
/// Initializes the vertex and index buffer.
/// </summary>
void Renderer::InitVertexAndIndexBuffer()
{
	return;
}

/// <summary>
/// Creates the vertex,index buffers and creates constant buffers.
/// </summary>
void Renderer::CreateBuffers()
{
	HRESULT hr = S_OK;			
	D3D11_BUFFER_DESC bd;

	if(!mMesh->mIsInGlobalVB)
	{
		//Change
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( Vertex ) * this->mMesh->mNoOfVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );

		InitData.pSysMem = this->mCurrentVertices;

		hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &this->mVertexBufferVertex);

		if( FAILED( hr ) )
		{
			MessageBox(NULL,L"ERROR: Unable to create Vertex vertex buffer",L"ERROR",MB_OK);
			//return false;
		}

		//if(mMesh->mFileType == PLY)
		//{
		//	D3D11_BUFFER_DESC bd;
		//	ZeroMemory( &bd, sizeof(bd) );
		//	bd.Usage = D3D11_USAGE_DEFAULT;
		//	bd.ByteWidth = sizeof( Vertex ) * this->mMesh->mNoOfVertices;
		//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//	bd.CPUAccessFlags = 0;
		//	D3D11_SUBRESOURCE_DATA InitData;
		//	ZeroMemory( &InitData, sizeof(InitData) );
		//	if(mIsDeformable)
		//		InitData.pSysMem = this->mDynamicVertexBufferPly;
		//	else
		//		InitData.pSysMem = this->mMesh->mPlyVertices;

		//	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &this->mVertexBuffer);

		//	if( FAILED( hr ) )
		//	{
		//		MessageBox(NULL,L"ERROR: Unable to create vertex buffer",L"ERROR",MB_OK);
		//		//return false;
		//	}
		//}

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( WORD ) * this->mMesh->mNumIndices;       
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = this->mMesh->mIndices;

		hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &this->mIndexBuffer );
		if( FAILED( hr ) )
		{
			MessageBox(NULL,L"ERROR: Unable to create index buffer",L"ERROR",MB_OK);
		}
	}
	else
	{
		mVertexBufferVertex = g_pVertexBuffer;
		mIndexBuffer = g_pIndexBuffer;
	}

	mPrivateConstantBuffer = g_pConstantBuffer;
	mPrivateChangingBuffer = g_pChangingBuffer;
	mPrivateAnimationMatixBuffer = g_pBoneMatrixBuffer;

	if( FAILED( hr ) )
		return;
}

/// <summary>
/// Sets the pixel shader.
/// </summary>
/// <param name="pixelShader">The pixel shader.</param>
void Renderer::SetPixelShader(ID3D11PixelShader* pixelShader)
{
	mPixelShader = pixelShader;
}
/// <summary>
/// Sets the vertex shader.
/// </summary>
/// <param name="vertexShader">The vertex shader.</param>
void Renderer::SetVertexShader(ID3D11VertexShader* vertexShader)
{
	mVertexShader = vertexShader;
}
void  Renderer::SetConstantBuffer(ID3D11Buffer* constantBuffer)
{
	mPrivateConstantBuffer = constantBuffer;
}
void Renderer::SetVertexLayout(ID3D11InputLayout* vertexLayout)
{
	mVertexLayout = vertexLayout;
}
void  Renderer::SetVertexStride(UINT vertexStride)
{
	mVertexStride = vertexStride;
}
void  Renderer::SetVertexOffset(UINT offset)
{
	mVertexOffset = offset;
}
void Renderer::SetRasterizerState(ID3D11RasterizerState* rsState)
{
	mRasterizerState = rsState;
}
void Renderer::SetBlendState(ID3D11BlendState* blendState)
{
	mBlendState = blendState;
}

void Renderer::SetRenderTexture(ID3D11ShaderResourceView* textureSRV)
{
	mRenderTexture = textureSRV;
}
void  Renderer::SetMainDiffuseTexture(ID3D11ShaderResourceView* textureSRV)
{
	
}

/// <summary>
/// Draws this instance.
/// </summary>
void Renderer::Draw()
{
	if(mEnabled)
	{
		if(mMaterial->mDiffuse.w < 1.0f)
		{
			mIsTransperant = true;
			g_pGlobalChangingBuffer.mTransperacyOn = DirectX::XMFLOAT4(1,1,1,1);
		}
		else
		{
			mIsTransperant = false;
			g_pGlobalChangingBuffer.mTransperacyOn = DirectX::XMFLOAT4(0,0,0,0);
		}

		
		DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixScaling(transform->scale.x,transform->scale.y,transform->scale.z) *  // Scale
											DirectX::XMMatrixRotationQuaternion(transform->GetWorldRotationQuaternion()) * // Rotate
											DirectX::XMMatrixTranslation(transform->position.x,transform->position.y,transform->position.z); // Translate
		
		if(mMesh != NULL)
		{
			// Updating Constant buffer
			g_pGlobalConstantBuffer.mWorld = XMMatrixTranspose( transformMatrix );
			g_pGlobalConstantBuffer.mView = XMMatrixTranspose( g_View );
			g_pGlobalConstantBuffer.mProjection = XMMatrixTranspose( g_Projection);
			g_pGlobalConstantBuffer.mInverseTransposedWorld = Helper::InverseTranspose(transformMatrix);

			// Updating Changing buffer
			g_pGlobalChangingBuffer.mObjectMaterial.mDiffuse = mMaterial->mDiffuse;
			g_pGlobalChangingBuffer.mObjectMaterial.mAmbient = mMaterial->mAmbient;
			g_pGlobalChangingBuffer.mObjectMaterial.mSpecular = mMaterial->mSpecular;
			g_pGlobalChangingBuffer.mObjectMaterial.mMeshColor = mMaterial->mMeshColor;


			// Set vertex buffer
			UINT stride = sizeof( Vertex );
			UINT offset = 0;
			g_pImmediateContext->IASetInputLayout(mVertexLayout);
			g_pImmediateContext->IASetVertexBuffers( 0, 1, &mVertexBufferVertex, &stride, &offset );
			g_pImmediateContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
			g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

			g_pImmediateContext->UpdateSubresource(mPrivateConstantBuffer , 0, NULL, &g_pGlobalConstantBuffer, 0, 0 );
			g_pImmediateContext->UpdateSubresource(mPrivateChangingBuffer, 0, NULL, &g_pGlobalChangingBuffer, 0, 0 );

			if(mMesh->mFileType == FBX)
			{
				g_pImmediateContext->UpdateSubresource(mPrivateConstantBuffer , 0, NULL, &g_pGlobalConstantBuffer, 0, 0 );
				g_pImmediateContext->UpdateSubresource(mPrivateChangingBuffer, 0, NULL, &g_pGlobalChangingBuffer, 0, 0 );
				g_pImmediateContext->UpdateSubresource(mPrivateAnimationMatixBuffer, 0, NULL, &g_pAnimationMatrices, 0, 0 );
				if(!mGpuSkinnig)
					g_pImmediateContext->UpdateSubresource(mVertexBufferVertex, 0, NULL, mCurrentVertices, 0, 0 );

				g_pImmediateContext->VSSetConstantBuffers( 2,  1, &mPrivateAnimationMatixBuffer);
			}
			else
			{
					if(mMesh->mIsInGlobalVB)
					{
						if(mMesh->mTextureArray != NULL)
						{
							g_pImmediateContext->PSSetShaderResources(0,1,&mMesh->mTextureArray[0].mTexture);
						}
					}
					else
					{
						g_pImmediateContext->UpdateSubresource(mVertexBufferVertex, 0, NULL, mCurrentVertices, 0, 0 );
						
						if(mMesh->mTextureArray != NULL)
						{
							g_pImmediateContext->PSSetShaderResources(0,1,&mMesh->mTextureArray[0].mTexture);
						}
					}
				}

			// Setting Constant buffers for vertex and pixel shader 
			g_pImmediateContext->VSSetConstantBuffers( 0, 1, &mPrivateConstantBuffer );
			g_pImmediateContext->VSSetConstantBuffers( 1, 1, &mPrivateChangingBuffer );
			g_pImmediateContext->VSSetShader( mVertexShader, NULL, 0 );
			g_pImmediateContext->PSSetConstantBuffers( 0, 1, &mPrivateConstantBuffer );
			g_pImmediateContext->PSSetConstantBuffers( 1, 1, &mPrivateChangingBuffer );
			g_pImmediateContext->PSSetShader( mPixelShader, NULL, 0 );
			g_pImmediateContext->RSSetState(mRasterizerState);

			// if Render texture replace shader resource view by render texture
			if(mIsRenderTexture && mRenderTexture != NULL)
				g_pImmediateContext->PSSetShaderResources(0,1,&mRenderTexture);

			

			if(mIsTransperant)
			{
				float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
				g_pImmediateContext->OMSetBlendState(g_pBlendState_Transparent,blendFactor,0xffffffff); // Set the Blend State to transparency
			}

			if(mMesh->mIsInGlobalVB)
				g_pImmediateContext->DrawIndexed( mMesh->mRenderInfo.mNoOfIndices, mMesh->mRenderInfo.mIndexBufferStartIndex, mMesh->mRenderInfo.mVertexBufferStartIndex);
			else
				g_pImmediateContext->DrawIndexed( mMesh->mNumIndices, 0, 0 );

			if(mIsTransperant)
			{
				float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
				// Reset the Blend State
				g_pImmediateContext->OMSetBlendState(0,blendFactor,0xffffffff);
			}
		}
	}
}

/// <summary>
/// Scales the uniformaly.
/// </summary>
/// <param name="factor">The factor.</param>
void Renderer::ScaleUniformaly(float factor)
{
	for (int vertexIndex = 0; vertexIndex != mRenderInfo.mNoOfVertices; vertexIndex++)
	{
		mCurrentVertices[vertexIndex].m_Position.x *= factor;
		mCurrentVertices[vertexIndex].m_Position.y *= factor;
		mCurrentVertices[vertexIndex].m_Position.z *= factor;
	}
	for (int j = 0; j != mMesh->mNoOfElements; j++)
	{
		mMesh->mElements[j].restlength1 *= factor;
		mMesh->mElements[j].restlength2 *= factor;
		mMesh->mElements[j].restlength3 *= factor;
	}
}

/// <summary>
/// Loads the mesh.
/// </summary>
/// <param name="fileName">Name of the file.</param>
void Renderer::LoadMesh(std::string fileName)
{
	mMesh = GetMeshByName(fileName);
	if(mMesh == NULL)
	{
		MessageBox(NULL,L"The give mesh is not found the assets list",L"Mesh Getting Error",MB_OK);
		return;
	}
	SetRenderInfo();
	mIsDeformable = true;
	//if(mMesh->mFileType == FBX)
	//{
		SetDynamicVertexAndIndexBuffer();
	//}
	CreateBuffers();
}

void Renderer::LoadPlane(int rows,int columns,int width,int height)
{
	mMesh = new Mesh;
	ShapeData data;
	CreatePlane(rows,columns,width,height,data);

	mMesh->mNoOfVertices = data.mNoOfVertices;
	mMesh->mNumIndices = data.mNoOfIndices;
	mMesh->mVertices = data.mVertices;
	mMesh->mIndices = data.mIndices;
	mMesh->mFileType = PLY;
	mIsDeformable = true;

	if(mIsDeformable)
		SetDynamicVertexAndIndexBuffer();

	CreateBuffers();
	SetVertexLayout(g_pVertexLayout);
	SetVertexShader(g_pVertexShader);
	SetConstantBuffer(g_pConstantBuffer);
	SetVertexStride(sizeof(Vertex));
	SetVertexOffset(0);
	mMesh->mTextureArray = new Texture[1];
	mMesh->mTextureArray[0].Initialize(g_pd3dDevice, g_pImmediateContext, "Textures/Grass.jpg");
}

IBehaviour* Renderer::Clone()
{
	Renderer* instance = new Renderer;
	instance->LoadMesh(mMesh->mModelName);
	return instance;
}
