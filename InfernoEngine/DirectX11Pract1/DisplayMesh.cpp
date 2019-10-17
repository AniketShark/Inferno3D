#include "DisplayMesh.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"
#include "HelperFuncs.h"
#include "Shader.h"
#include <new>

/////////////////////////////////////////////////
// Written by Daniel Brewer
// djbrewer@fanshawec.ca
// Use this code at your own risk. It is indented only as a learning aid.

DisplayMesh::DisplayMesh() : 
    Entity(),
	mMesh(NULL),
    mAnimation(NULL),
    mChannelMap(NULL),
    mDisplayMode(DM_MESH_ONLY),

    mCurrentVerts(NULL),
    mCurrentBones(NULL),
    mCurrentGlobalPose(NULL),
    mBoneVerts(NULL),
    mBoneIndices(NULL),

    mDrawBindPose(true),
    mSingleBoneSkinning(true),
    mFrameInterpolate(false),
    mDebugRotation(0.0f),

    mAnimTime(0.0f),
    mAnimRate(1.0f),
    mCurrentFrame(0.0f),

    mVertexShader(NULL),
    mPixelShader(NULL),
    mVertexLayout(NULL),
    mVertexBuffer(NULL),
    mIndexBuffer(NULL),
    mConstantBuffer(NULL),
    mSkelVertexBuffer(NULL),
    mSkelIndexBuffer(NULL)
{
}

DisplayMesh::~DisplayMesh()
{
	mMesh = NULL;

    if(mChannelMap) delete [] mChannelMap;
    if(mCurrentVerts) delete [] mCurrentVerts;
    if(mCurrentBones) delete [] mCurrentBones;
    if(mCurrentGlobalPose) delete [] mCurrentGlobalPose;
    if(mBoneVerts) delete [] mBoneVerts;
    if(mBoneIndices) delete [] mBoneIndices;

    if( mSkelVertexBuffer ) mSkelVertexBuffer->Release();
    if( mSkelIndexBuffer ) mSkelIndexBuffer->Release();
    if( mConstantBuffer ) mConstantBuffer->Release();
    if( mVertexBuffer ) mVertexBuffer->Release();
    if( mIndexBuffer ) mIndexBuffer->Release();
    if( mVertexLayout ) mVertexLayout->Release();
    if( mVertexShader ) mVertexShader->Release();
    if( mPixelShader ) mPixelShader->Release();
}

void DisplayMesh::SetMesh(Mesh* pMesh)
{
	mMesh = pMesh;
}

HRESULT DisplayMesh::Initialize()
{
    HRESULT hr = S_OK;

	if(!mMesh)
	{
		return(E_FAIL);
	}

    mCurrentVerts = new (std::nothrow) SimpleVertex[mMesh->mNumVerts];
    mCurrentGlobalPose = new (std::nothrow) _XMFLOAT4X4[mMesh->mNumBones];
    mCurrentBones = new (std::nothrow) JointPose[mMesh->mNumBones];
    mBoneVerts = new (std::nothrow) SimpleVertex[mMesh->mNumBones * 2];
    mBoneIndices = new (std::nothrow) WORD[mMesh->mNumBones * 2];

    if(!mCurrentVerts || !mCurrentGlobalPose || !mCurrentBones || !mBoneVerts || !mBoneIndices)
    {
        return(E_FAIL);
    }
    for(int i = 0; i < mMesh->mNumVerts; ++i)
    {
        mCurrentVerts[i].Pos = mMesh->mVerts[i].Pos;
        mCurrentVerts[i].Color = mMesh->mVerts[i].Color;
    }
    int b = 0;
    for(int i = 0; i < mMesh->mNumBones; ++i)
    {
        mCurrentGlobalPose[i] = mMesh->mOrigGlobalPose[i];
        mCurrentBones[i] = mMesh->mOrigBones[i];

        XMFLOAT3 tempPoint(0.0f, 0.0f, 0.0f);
        XMVECTOR temp = XMLoadFloat3(&tempPoint);
        XMMATRIX m = XMLoadFloat4x4(&mCurrentGlobalPose[i]);
        XMStoreFloat3(&mBoneVerts[b].Pos, XMVector3Transform(temp, m));

        if(mMesh->mSkeleton[i].parent >= 0)
            m = XMLoadFloat4x4(&mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
        XMStoreFloat3(&mBoneVerts[b+1].Pos, XMVector3Transform(temp, m));
        
        mBoneVerts[b].Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
        mBoneVerts[b+1].Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
        
        mBoneIndices[b] = b;
        mBoneIndices[b+1] = b+1;
        b += 2;
    }

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"Shaders/BasicShader.fx", L"VS", L"vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return(hr);
    }

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &mVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return(hr);
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &mVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return(hr);

    // Set the input layout
    g_pImmediateContext->IASetInputLayout( mVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Shaders/BasicShader.fx", L"PS", L"ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return(hr);
    }

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &mPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return(hr);

    // Create vertex buffer
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( SimpleVertex ) * mMesh->mNumVerts;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = mCurrentVerts;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
    if( FAILED( hr ) )
        return(hr);

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create index buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * mMesh->mNumIndices;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = mMesh->mIndices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &mIndexBuffer );
    if( FAILED( hr ) )
        return(hr);

    // Set index buffer
    g_pImmediateContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create vertex buffer for drawing bones
	//ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * mMesh->mNumBones * 2;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    InitData.pSysMem = mBoneVerts;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &mSkelVertexBuffer );
    if( FAILED( hr ) )
        return(hr);

    // Set vertex buffer
    UINT stride2 = sizeof( SimpleVertex );
    UINT offset2 = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &mSkelVertexBuffer, &stride2, &offset2 );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

    // Create index buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * mMesh->mNumBones * 2;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
    InitData.pSysMem = mBoneIndices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &mSkelIndexBuffer );
    if( FAILED( hr ) )
        return(hr);

    // Set index buffer
    g_pImmediateContext->IASetIndexBuffer( mSkelIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

    // Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FBXConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &mConstantBuffer );
    if( FAILED( hr ) )
        return(hr);

    return(S_OK);
}

void DisplayMesh::Update(float delta)
{
    if(mAnimation)
    {
        float oldTime = mAnimTime;
        mAnimTime = oldTime;
        //find the appropriate frame
        mAnimTime += delta * mAnimRate;
        while(mAnimTime >= mAnimation->mDuration)
        {
            mAnimTime -= mAnimation->mDuration;
        }
        int frame = 0;
        while(mAnimTime > mAnimation->mKeys[frame].mTime && frame < mAnimation->mNumKeys)
        {
            ++frame;
        }

        if(frame >= mAnimation->mNumKeys)
        {
            frame = 0;
        }
        if(!mFrameInterpolate)
        {
            mCurrentFrame = static_cast<float>(frame);
            // we've got the frame, so update the animation   
            for(int i = 0; i < mMesh->mNumBones; ++i)
            {
                if(mChannelMap[i] != -1)
                {
                    JointPose* jnt = &(mAnimation->mKeys[frame].mBones[mChannelMap[i]]);
                    mCurrentBones[i].translation = jnt->translation;
                    mCurrentBones[i].rotation = jnt->rotation;
                    mCurrentBones[i].scale = jnt->scale;
                }
            }
        }
        else
        {
            //frame interpolation
            int prevFrame = 0;
			float t = 0;
			if(frame > 0)
			{
				prevFrame = frame - 1;
				t = (mAnimTime - mAnimation->mKeys[prevFrame].mTime) / (mAnimation->mKeys[frame].mTime - mAnimation->mKeys[prevFrame].mTime);
			}

			mCurrentFrame = static_cast<float>(frame) + t; //this is just for show in the title-bar

            // we've got the frame, so update the animation   
            for(int i = 0; i < mMesh->mNumBones; ++i)
            {
                if(mChannelMap[i] != -1)
                {
                    JointPose* jntA = &(mAnimation->mKeys[prevFrame].mBones[mChannelMap[i]]);
					JointPose* jntB = &(mAnimation->mKeys[frame].mBones[mChannelMap[i]]);

					XMStoreFloat3(&mCurrentBones[i].translation, XMVectorLerp(XMLoadFloat3(&jntA->translation), XMLoadFloat3(&jntB->translation), t));
                    XMStoreFloat4(&mCurrentBones[i].rotation, XMQuaternionSlerp(XMLoadFloat4(&jntA->rotation), XMLoadFloat4(&jntB->rotation), t));
                    XMStoreFloat3(&mCurrentBones[i].scale, XMVectorLerp(XMLoadFloat3(&jntA->scale), XMLoadFloat3(&jntB->scale), t));
                }
            }
        }
    }
    else
    {
        //some test rotation of bones to see the skinning work
        XMVECTOR r = XMQuaternionRotationRollPitchYaw(mDebugRotation, 0.0f, 0.0f);
        for(int i = 1; i < mMesh->mNumBones; ++i)
        {
            XMVECTOR q = XMLoadFloat4(&mMesh->mOrigBones[i].rotation);

            q = XMQuaternionMultiply(q, r);

            XMStoreFloat4(&mCurrentBones[i].rotation, q);
        }
    }

    //update the current pose
    for(int i = 0; i < mMesh->mNumBones; ++i)
    {
        XMMATRIX m = mCurrentBones[i].GetTransform();
        if(mMesh->mSkeleton[i].parent >= 0)
        {
            XMMATRIX c = XMLoadFloat4x4(&mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
            m *= c;
        }
        
        XMStoreFloat4x4(&mCurrentGlobalPose[i], m);
    }

    //update the skeleton debug draw lines
    int b = 0;
    for(int i = 0; i < mMesh->mNumBones; ++i)
    {
        XMFLOAT3 tempPoint(0.0f, 0.0f, 0.0f);
        XMVECTOR temp = XMLoadFloat3(&tempPoint);
        XMMATRIX m = XMLoadFloat4x4(&mCurrentGlobalPose[i]);
        XMStoreFloat3(&mBoneVerts[b].Pos, XMVector3Transform(temp, m));

        if(mMesh->mSkeleton[i].parent >= 0)
            m = XMLoadFloat4x4(&mCurrentGlobalPose[mMesh->mSkeleton[i].parent]);
        XMStoreFloat3(&mBoneVerts[b+1].Pos, XMVector3Transform(temp, m));
        b += 2;
    }

    //update the vertices's
    if(mDrawBindPose)
    {
        for(int i = 0; i < mMesh->mNumVerts; ++i)
        {
            mCurrentVerts[i].Pos = mMesh->mVerts[i].Pos;
        }
    }
    else
    {
        //use skinning
        //1-bone skinning
        if(mSingleBoneSkinning)
        {
            for(int i = 0; i < mMesh->mNumVerts; ++i)
            {
                int b = mMesh->mVerts[i].JointIndex[0];
        
                XMVECTOR v = XMLoadFloat3(&mMesh->mVerts[i].Pos);

                // Convert vertex into local bone space by multiplying it 
                // by the inverse bind-pose transform
                XMMATRIX invBoneM = XMLoadFloat4x4(&mMesh->mSkeleton[b].invBindPose);
                v = XMVector3Transform(v, invBoneM); 

                // Convert back to current pose in model-space by multiplying
                // it by the bone's current transform
                XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[b]);
                v = XMVector3Transform(v, curBoneM);

                XMStoreFloat3(&mCurrentVerts[i].Pos, v);
            }
        }
        else
        {
            //multi-bone skinning
			for(int i = 0; i < mMesh->mNumVerts; ++i)
            {
				XMVECTOR vb = XMLoadFloat3(&mMesh->mVerts[i].Pos);
				XMVECTOR vc = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
				for(int j = 0; j < 4; ++j)
				{
					int b = mMesh->mVerts[i].JointIndex[j];
        
					// Convert vertex into local bone space by multiplying it 
					// by the inverse bind-pose transform
					XMMATRIX invBoneM = XMLoadFloat4x4(&mMesh->mSkeleton[b].invBindPose);
					XMVECTOR v = XMVector3Transform(vb, invBoneM); 

					// Convert back to current pose in model-space by multiplying
					// it by the bone's current transform
					XMMATRIX curBoneM = XMLoadFloat4x4(&mCurrentGlobalPose[b]);
					v = XMVector3Transform(v, curBoneM);

					vc += v * mMesh->mVerts[i].JointWeight[j];
				}
                XMStoreFloat3(&mCurrentVerts[i].Pos, vc);
			}
        }
    }
}

void DisplayMesh::Draw()
{
    //
    // Update variables
    //
    g_cb.mWorld = XMMatrixTranspose( GetTransform() );
	g_cb.mView = XMMatrixTranspose( g_View);
	g_cb.mProjection = XMMatrixTranspose(g_Projection);
	g_pImmediateContext->UpdateSubresource( mConstantBuffer, 0, NULL, &g_cb, 0, 0 );

    if(mDisplayMode == DM_MESH_ONLY || mDisplayMode == DM_MESH_AND_SKEL)
    {
        // Set vertex buffer
        UINT stride = sizeof( SimpleVertex );
        UINT offset = 0;
        g_pImmediateContext->UpdateSubresource( mVertexBuffer, 0, NULL, mCurrentVerts, 0, 0 );
        g_pImmediateContext->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );
        g_pImmediateContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
        g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
   

        // Render a triangle
        // Set the input layout
        g_pImmediateContext->IASetInputLayout( mVertexLayout );
	    g_pImmediateContext->VSSetShader( mVertexShader, NULL, 0 );
        g_pImmediateContext->VSSetConstantBuffers( 0, 1, &mConstantBuffer );
	    g_pImmediateContext->PSSetShader( mPixelShader, NULL, 0 );
		g_pImmediateContext->RSSetState(g_pRS_WireFrameNoCull);
	    g_pImmediateContext->DrawIndexed( mMesh->mNumIndices, 0, 0 );
    }

    if(mDisplayMode == DM_SKEL_ONLY || mDisplayMode == DM_MESH_AND_SKEL)
    {
        // Set vertex buffer for bones
        UINT stride = sizeof( SimpleVertex );
        UINT offset = 0;
        g_pImmediateContext->UpdateSubresource( mSkelVertexBuffer, 0, NULL, mBoneVerts, 0, 0 );
        g_pImmediateContext->IASetVertexBuffers( 0, 1, &mSkelVertexBuffer, &stride, &offset );
        g_pImmediateContext->IASetIndexBuffer( mSkelIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
        g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

        // Set the input layout
        g_pImmediateContext->IASetInputLayout( mVertexLayout );
	    g_pImmediateContext->VSSetShader( mVertexShader, NULL, 0 );
        g_pImmediateContext->VSSetConstantBuffers( 0, 1, &mConstantBuffer );
	    g_pImmediateContext->PSSetShader( mPixelShader, NULL, 0 );
		
        g_pImmediateContext->DrawIndexed( mMesh->mNumBones * 2, 0, 0 );
    }

}

void DisplayMesh::SetDisplayMode(DisplayMode dm)
{
    mDisplayMode = dm;
}

DisplayMesh::DisplayMode DisplayMesh::GetDisplayMode() const
{
    return(mDisplayMode);
}

void DisplayMesh::PlayAnimation(SkeletalAnimation* anim)
{
    mAnimation = anim;
    mAnimTime = 0.0f;
    if(anim && mMesh)
    {
        //init channel-map
        if(!mChannelMap)
        {
            mChannelMap = new (std::nothrow) unsigned char[mMesh->mNumBones];
        }
        if(mChannelMap)
        {

            for(int i = 0; i < mMesh->mNumBones; ++i)
            {
                mChannelMap[i] = -1;
                for(int j = 0; j < anim->mNumBones; ++j)
                {
                    if(strcmp(mMesh->mSkeleton[i].name, anim->mSkeleton[j].name) == 0)
                    {
                        mChannelMap[i] = j;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if(mChannelMap) delete [] mChannelMap;
        mChannelMap = NULL;
    }
}

char* DisplayMesh::GetPlayingAnimation() const
{
    static char none[] = "None";
    if(mAnimation)
    {
        return(mAnimation->mName);
    }
    else
    {
        return(none);
    }
}

float DisplayMesh::GetCurrentAnimTime() const
{
    return(mAnimTime);
}

float DisplayMesh::GetCurrentAnimFrame() const
{
    return(mCurrentFrame);
}
