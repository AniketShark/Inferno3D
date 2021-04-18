#include "RenderToTexture.h"


RenderToTexture::RenderToTexture()
{
	mTextureWidth  = 0;
	mTextureHeight = 0;

	mTargetTexture = NULL;
	mTargetSRV = NULL;
	mTargetRTV = NULL;

	mTargetDepthMap = NULL;
	mDepthMapSRV = NULL;
	mDepthRTV = NULL;

	mDepthMapDSV = NULL;
	mDeviceContext = NULL;

	mViewProjectionMatrix = NULL;

	mDepthMapFormat = DXGI_FORMAT_R32_TYPELESS;
	mTextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

}

RenderToTexture::~RenderToTexture()
{
	if(mTargetRTV) mTargetRTV->Release();
	if(mDepthRTV) mDepthRTV->Release();
}

bool RenderToTexture::Initialize(ID3D11Device* device,int textureWidth,int textureHeight,DXGI_FORMAT format)
{
	HRESULT result;
	mTextureWidth = textureWidth;
	mTextureHeight = textureHeight;
	mTextureFormat = format;

	
	if(!m_CreateDepthMap(device))
	{
		MessageBox(NULL,L"ERROR: Could not create Target Depth Texture.",L"Error",MB_OK);

		return false;
	}

	if(!m_CreateColorMap(device))
	{
		MessageBox(NULL,L"ERROR: Could not create Target Texture.",L"Error",MB_OK);
		return false;
	}
	
	return true;
}

bool RenderToTexture::m_CreateDepthMap(ID3D11Device* device)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = mTextureWidth;
	texDesc.Height = mTextureHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = mDepthMapFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	device->CreateTexture2D(&texDesc, 0, &mTargetDepthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(mTargetDepthMap,&dsvDesc, &mDepthMapDSV);
	if(FAILED(result))
	{
		MessageBox(NULL,L"ERROR: Could not create Target Depth Texture.",L"Error",MB_OK);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(mTargetDepthMap, &srvDesc, &mDepthMapSRV);

	if(FAILED(result))
	{
		MessageBox(NULL,L"ERROR: Could not create Target Depth Texture.",L"Error",MB_OK);
		return false;
	}

	// View saves a reference to the texture so we can
	mTargetDepthMap->Release();

	return true;
}

bool RenderToTexture::m_CreateColorMap(ID3D11Device* device)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.Width     = mTextureWidth;
	texDesc.Height    = mTextureHeight;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format    = mTextureFormat;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage          = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags      = D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags      = D3D11_RESOURCE_MISC_GENERATE_MIPS;	

	result = device->CreateTexture2D(&texDesc, 0, &mTargetTexture);

	if(FAILED(result))
	{
		return false;
	}

	// Null description means to create a view to all mipmap levels
	// using the format the texture was created with.
	result = device->CreateRenderTargetView(mTargetTexture, 0, &mTargetRTV);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateShaderResourceView(mTargetTexture, 0, &mTargetSRV);
	if(FAILED(result))
	{
		return false;
	}

	// View saves a reference to the texture so we can
	// release our reference.
	mTargetTexture->Release();

	return true;
}

void RenderToTexture::SetDeviceContext(ID3D11DeviceContext* deviceContext)
{
	mDeviceContext = deviceContext;
}

void RenderToTexture::ClearRenderTarget(ID3D11DeviceContext* renderTarget, float color[4])
{
	renderTarget->ClearRenderTargetView(mTargetRTV,color);
}

void RenderToTexture::RenderObjectToTexture(GameObject* go)
{
	
	Begin();

	go->renderer->Update();

	End();

}

void RenderToTexture::Begin()
{
	ID3D11RenderTargetView* renderTargets[1] = {mTargetRTV};
	// md3dDevice->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);
	mDeviceContext->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);

	// md3dDevice->RSSetViewports(1, &mViewport);
	//mDeviceContext->RSSetViewports(1, &);

	// only clear if we actually created a color map.
	if( this->mTargetRTV)
	{
		// md3dDevice->ClearRenderTargetView(mColorMapRTV, BLACK);

		//float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		//md3dDeviceContext->ClearRenderTargetView(mColorMapRTV, black);
		float black[4] = { 0, 0, 0, 1.0f };
		mDeviceContext->ClearRenderTargetView(mTargetRTV, black);
	}

	// md3dDevice->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	mDeviceContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderToTexture::End()
{
	// After we have drawn to the color map, have the hardware generate
	// the lower mipmap levels.

	if( this->mTargetRTV )
	{
		//md3dDevice->GenerateMips(mColorMapSRV);
		mDeviceContext->GenerateMips(mTargetSRV);
	}
}

ID3D11ShaderResourceView* RenderToTexture::GetSRVOfTargetTexture()
{
	return mTargetSRV;
}

void RenderToTexture::SetViewProjectionMatrix(DirectX::XMMATRIX* matrix)
{
	mViewProjectionMatrix = matrix;
}

int RenderToTexture::GetTextureWidth()
{
	return mTextureWidth;
}

int RenderToTexture::GetTextureHeight()
{
	return mTextureHeight;
}

void RenderToTexture::Release()
{

}