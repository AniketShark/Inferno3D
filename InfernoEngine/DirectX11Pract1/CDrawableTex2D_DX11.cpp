#include "CDrawableTex2D_DX11.h"

CDrawableTex2D_DX11::CDrawableTex2D_DX11():mWidth (0),mHeight (0), mColorMapFormat (DXGI_FORMAT_UNKNOWN), md3dDevice(0), mColorMapSRV(0), mColorMapRTV(0), mDepthMapSRV(0), mDepthMapDSV(0)
{
	ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));
}

CDrawableTex2D_DX11::~CDrawableTex2D_DX11()
{
	//md3dDevice->Release();
	/*Release(mColorMapRTV);
	ReleaseCOM(mDepthMapSRV);
	ReleaseCOM(mDepthMapDSV);*/
	if ( this->mColorMapRTV )	{ mColorMapRTV->Release(); }
	if ( this->mDepthMapSRV )	{ mDepthMapSRV->Release(); }
	if ( this->mDepthMapDSV )	{ mDepthMapDSV->Release(); }

}

ID3D11ShaderResourceView* CDrawableTex2D_DX11::colorMap()
{
	return mColorMapSRV;
}

ID3D11ShaderResourceView* CDrawableTex2D_DX11::depthMap()
{
	return mDepthMapSRV;
}

void CDrawableTex2D_DX11::init(ID3D11Device* device, ID3D11DeviceContext* contextDevice, UINT width, UINT height, bool hasColorMap, DXGI_FORMAT colorFormat)
{
	this->mWidth = width;
	this->mHeight = height;

	this->mColorMapFormat = colorFormat;

	this->md3dDevice = device;
	this->md3dDeviceContext = contextDevice;

	this->buildDepthMap();

	if(hasColorMap)
	{
		this->buildColorMap();
	}

	this->mViewport.TopLeftX = 0;
	this->mViewport.TopLeftY = 0;

	this->mViewport.Width = static_cast<FLOAT>( width );
	this->mViewport.Height = static_cast<FLOAT>( height );

	this->mViewport.MinDepth = 0.0f;
	this->mViewport.MaxDepth = 1.0f;

}

void CDrawableTex2D_DX11::buildDepthMap()
{
	ID3D11Texture2D* depthMap = 0;
	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//hr(
	md3dDevice->CreateTexture2D(&texDesc, 0, &depthMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
   // HR(
	md3dDevice->CreateDepthStencilView(depthMap,&dsvDesc, &mDepthMapDSV);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
   // HR(
	md3dDevice->CreateShaderResourceView(depthMap, &srvDesc, &mDepthMapSRV);

    // View saves a reference to the texture so we can
	depthMap->Release();
}

void CDrawableTex2D_DX11::buildColorMap()
{
    ID3D11Texture2D* colorMap = 0;

    D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

    texDesc.Width     = mWidth;
    texDesc.Height    = mHeight;
    texDesc.MipLevels = 0;
    texDesc.ArraySize = 1;
    texDesc.Format    = mColorMapFormat;
    texDesc.SampleDesc.Count   = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage          = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags      = D3D11_BIND_RENDER_TARGET |
                             D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags      = D3D11_RESOURCE_MISC_GENERATE_MIPS;	

    //HR(
	md3dDevice->CreateTexture2D(&texDesc, 0, &colorMap);

    // Null description means to create a view to all mipmap levels
    // using the format the texture was created with.
   // HR(
	md3dDevice->CreateRenderTargetView(colorMap, 0, &mColorMapRTV);
    //HR(
	md3dDevice->CreateShaderResourceView(colorMap, 0, &mColorMapSRV);

    // View saves a reference to the texture so we can
    // release our reference.
	colorMap->Release();
}


void CDrawableTex2D_DX11::begin()
{

    ID3D11RenderTargetView* renderTargets[1] = {this->mColorMapRTV};
   // md3dDevice->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);
	md3dDeviceContext->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);

   // md3dDevice->RSSetViewports(1, &mViewport);
	md3dDeviceContext->RSSetViewports(1, &this->mViewport);

    // only clear if we actually created a color map.
    if( this->mColorMapRTV )
	{
	// md3dDevice->ClearRenderTargetView(mColorMapRTV, BLACK);

		//float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		//md3dDeviceContext->ClearRenderTargetView(mColorMapRTV, black);
		float cornflowerblue[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		md3dDeviceContext->ClearRenderTargetView(mColorMapRTV, cornflowerblue);
	}

   // md3dDevice->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	md3dDeviceContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDrawableTex2D_DX11::end()
{
    // After we have drawn to the color map, have the hardware generate
    // the lower mipmap levels.

    if( this->mColorMapSRV )
	{
            //md3dDevice->GenerateMips(mColorMapSRV);
		md3dDeviceContext->GenerateMips(this->mColorMapSRV);
	}
}




