#include "global.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "resource.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <time.h>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "Console.h"
#include <algorithm>
#include "Scene.h"
#include "ScriptsManager.h"
#include "PrefabManager.h"
#include "ObjectFactory.h"
#include "RenderToTexture.h"

bool once = false;

Shader* shaderToBeUsed;

float collisionScale = 0.01f;

bool collisionMeshGenerated = false;

std::wstring orderOfDraw;

std::string currenAnimationName;

bool switchAnimation = false;

CHRTimer timer;

GameObject MainCamera;
 
UINT width; 
UINT height;

int numRows = 20;
int numCols = 20;


std::vector<GameObject*> listOfGameobjectsOnScreen;

GameObject* ground;
GameObject* teapot;
GameObject* crate;
GameObject* crate1;
CameraScript* camerascript;

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
void LoadAllShaders();
DWORD dwThreadId;

bool CreateDepthStencilStates()
{
	HRESULT hr;
	
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;


	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	hr  = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &g_pDSStateNormal);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the Normal DSState.",L"Error",MB_OK);
		return false;
	}	

	// Set Mask in stencil buffer
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	hr  = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &g_pDSStateMaskStencil);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the Normal DSState.",L"Error",MB_OK);
		return false;
	}

	//Draw Inside Objects
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	hr  = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &g_pDSStateDrawInside);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the Normal DSState.",L"Error",MB_OK);
		return false;
	}


	//Draw Outside Objects
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;


	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	hr  = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &g_pDSStateDrawOutside);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the Normal DSState.",L"Error",MB_OK);
		return false;
	}

	return true;
}

bool CreateRasterizerStates()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC descrip;
	ZeroMemory( &descrip, sizeof( D3D11_RASTERIZER_DESC ) );	// Clear it
	// Set up the "rasterizer state" so we can not "cull" the "back facing" triangles
	descrip.CullMode = D3D11_CULL_NONE;
	// Prepare the "rasterizer state" so that we can change to wire frame, etc.
	descrip.FillMode = D3D11_FILL_WIREFRAME;

	::g_pd3dDevice->CreateRasterizerState( &descrip, &(g_pRS_WireFrameNoCull));

	descrip.CullMode = D3D11_CULL_BACK;
	descrip.FillMode = D3D11_FILL_SOLID;
	::g_pd3dDevice->CreateRasterizerState( &descrip, &(g_pRS_SolidCullBack) );

	descrip.CullMode = D3D11_CULL_NONE;
	descrip.FillMode = D3D11_FILL_SOLID;
	::g_pd3dDevice->CreateRasterizerState( &descrip, &(g_pRS_SolidCullNone) );

	// Used for the sky box
	descrip.CullMode = D3D11_CULL_FRONT;		/// i.e. we'll draw the "back" facing triangles
	descrip.FillMode = D3D11_FILL_SOLID;
	::g_pd3dDevice->CreateRasterizerState( &descrip, &(g_pRS_SolidCullFront) );

	// Used for clockwise culling
	descrip.FillMode = D3D11_FILL_SOLID;
	descrip.CullMode = D3D11_CULL_BACK;
	descrip.FrontCounterClockwise = true;
	descrip.DepthClipEnable = true;
	::g_pd3dDevice->CreateRasterizerState( &descrip, &(g_pRS_CullClockwise));


	if(FAILED(hr))
	{
		MessageBox(0, L"RS STATE Creation - Failed",L"Error", MB_OK);
		return false;
	}
	return true;
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	
	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitDevice() ) )
	{
		CleanupDevice();
		return 0;
	} 

	Scene::LoadAssets(false);
	MainCamera.AddComponent(CameraComp);
	MainCamera.AttachScript<CameraScript>();

	ground = new GameObject;
	teapot = new GameObject;
	crate = new GameObject;
	crate1 = new GameObject;

	
	//ground->AddComponent(RenderComp); 
	//ground->renderer->SetDiffuseColor(Vector4(1,1,1,1)); 
	//ground->renderer->SetPixelShader(g_pPixelShaderSingleTextureOnly);
	//ground->renderer->LoadPlane(20,20,200,200);

	float scale = 10;
	crate->AddComponent(RenderComp);
	crate->AddComponent(ColliderComp);
	crate->AttachScript<PlayerScript>();
	crate->mName = "Crate0";
	crate->renderer->LoadMesh("Crate.ply");
	crate->transform.scale = Vector3::One * scale;
	crate->transform.position = Vector3(-20,(scale*0.5f),0);
	crate->renderer->SetPixelShader(g_pPixelShaderSingleTextureOnly);
	

	Scene::Initialize();
	MainCamera.transform.position = Vector3(0,20, -100);
	MainCamera.camera->SetLookAtTransform(&teapot->transform);
	ground->transform.position =  Vector3(0, 0, 0);

	MSG msg = {0};
	timer.Start();
	while( WM_QUIT != msg.message )
	{ 
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timer.Stop(); 
			g_globalDeltaTime = timer.GetElapsedSeconds();
			mGlobalDeltaTime = g_globalDeltaTime;
			mTimeSinceStart += g_globalDeltaTime;
			timer.Start();
			Render();
		}
	}

	CleanupDevice();

	return ( int )msg.wParam;
}

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
	if( !RegisterClassEx( &wcex ) )
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 4: 3D Spaces", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL );
	if( !g_hWnd )
		return E_FAIL;

	g_input.Initialize(hInstance,g_hWnd,width,height);

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------

HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( g_hWnd, &rc );
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED( hr ) )
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if( FAILED( hr ) )
		return hr;


	// Depth Buffer Setting
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof( descDepth ) );
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = g_pd3dDevice->CreateTexture2D( &descDepth,
		NULL, 
		&g_pDepthStencilBuffer );
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the depth-stencil buffer.",L"Error",MB_OK);
		return false;
	}

	// Creating DSState
	if(!CreateDepthStencilStates())
	{
		MessageBox(NULL,L"ERROR: Could Not Create the Depth Stencil States.",L"Error",MB_OK);
	}

	//Set depth stencil state
	g_pImmediateContext->OMSetDepthStencilState(g_pDSStateNormal, 1);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencilBuffer, 
		&descDSV, 
		&g_pDepthStencilView );
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Could not create the depth-stencil buffer.",L"Error",MB_OK);
		return false;
	}	

	g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the view port
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports( 1, &vp );

	//////////////////////////////////////////////////////////////////////////
	//  Model LoadingPart
	//////////////////////////////////////////////////////////////////////////

	//Load Shaders from directory
	LoadShadersFromDirectory(L"Shaders/ShaderSource.dat");

	LoadAllShaders();

	

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerStateLinear);
	if( FAILED( hr ) )
		return hr;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerStateAnisotropic);
	if( FAILED( hr ) )
		return hr;

	
	if(!CreateRasterizerStates())
	{
		MessageBox(NULL, L"Can't create Rasterizer state", L"Error", MB_OK );
		return false;
	}

	// Transparent blend state
	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable  = false;
	transparentDesc.IndependentBlendEnable = false;
	transparentDesc.RenderTarget[0].BlendEnable           = true;
	transparentDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if ( FAILED(g_pd3dDevice->CreateBlendState(&transparentDesc, &g_pBlendState_Transparent)))
	{
		MessageBox(NULL, L"Can't create blend state", L"Error", MB_OK );
		return -1;
	}


	// Dont write to render target blend state
	D3D11_BLEND_DESC noRenderTargetWritesDesc = {0};
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;
	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable    = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	if ( FAILED(g_pd3dDevice->CreateBlendState(&noRenderTargetWritesDesc, &g_pBlendState_DontWriteToRenderTarget)))
	{
		MessageBox(NULL, L"Can't create blend state Don't Render to target", L"Error", MB_OK );
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//  Creating unordered access view for blurred texture 
	//////////////////////////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC blurredTextureDesc;

	blurredTextureDesc.Width = width;
	blurredTextureDesc.Height = height;
	blurredTextureDesc.MipLevels = 1;
	blurredTextureDesc.ArraySize = 1;
	blurredTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	blurredTextureDesc.SampleDesc.Count = 1;
	blurredTextureDesc.SampleDesc.Quality = 0;
	blurredTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	blurredTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	blurredTextureDesc.CPUAccessFlags = 0;
	blurredTextureDesc.MiscFlags = 0;


	hr = g_pd3dDevice->CreateTexture2D(&blurredTextureDesc,0,&g_pBlurredTexture);
	if ( FAILED(hr))
	{
		MessageBox(NULL, L"Can't create Blurred Texture2D ", L"Error", MB_OK );
		return -1;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;


	hr =  g_pd3dDevice->CreateShaderResourceView(g_pBlurredTexture,&srvDesc,&g_pBlurredTextureResource); 
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Can't create Blurred Shader Resource Views ", L"Error", MB_OK );
		return -1;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavvDesc;
	uavvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	uavvDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavvDesc.Texture2D.MipSlice = 0;

	hr =  g_pd3dDevice->CreateUnorderedAccessView(g_pBlurredTexture,&uavvDesc,&g_pBlurredUAV); 
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Can't create Blurred Unordered access view ", L"Error", MB_OK );
		return -1;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState();
	if(g_pDepthStencilBuffer)
		g_pDepthStencilBuffer->Release();
	if(g_pDepthStencilView)
		g_pDepthStencilView->Release();

	if( g_pConstantBuffer ) g_pConstantBuffer->Release();
	if( g_pVertexBuffer ) g_pVertexBuffer->Release();
	if( g_pIndexBuffer ) g_pIndexBuffer->Release();
	if( g_pVertexLayout ) g_pVertexLayout->Release();
	if( g_pVertexShader ) g_pVertexShader->Release();
	if( g_pPixelShaderLightingOnly ) g_pPixelShaderLightingOnly->Release();
	if( g_pRenderTargetView ) g_pRenderTargetView->Release();
	if( g_pSwapChain ) g_pSwapChain->Release();
	if( g_pImmediateContext ) g_pImmediateContext->Release();
	if( g_pd3dDevice ) g_pd3dDevice->Release();
	if(g_pBlurredTexture)g_pBlurredTexture->Release();
	if(g_pBlurredTextureResource) g_pBlurredTextureResource->Release();
	if(g_pBlurredUAV) g_pBlurredUAV->Release();
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;
		// Handle keyboard input...

	case WM_CHAR:
		{

			

		}

		break;
	case WM_KEYDOWN:
		{
			float offset = 0.1f;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}

void Render()
{
	float ClearColor[4] = { 0.1f, 0.1f, 0.5f, 1.0f }; // red,green,blue,alpha
	float blendFactor[4] = { 0, 0, 0, 0 }; 
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
	//Clears Depth buffer to max depth 1.0f and stencil buffer to 0
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,1.0f ,0 );
	g_World = DirectX::XMMatrixIdentity();
	Scene::Update();
	g_pSwapChain->Present( 0, 0 );
}

DWORD WINAPI LoadAssets(LPVOID lpParam)
{
	collisionMeshGenerated = true;
	return 0; 
}

void LoadAllShaders()
{
	shaderToBeUsed = FindShader(L"Shaders/LightingAndTexturingShader.hlsl");
	g_pVertexShader = shaderToBeUsed->mVertexShader;
	g_pPixelShaderLightingOnly = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/SingleTextureOnly.hlsl");
	g_pPixelShaderSingleTextureOnly = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/TwoTexturesOnly.hlsl");
	g_pPixelShaderTwoTexturesOnly = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/SingleTexturePlusLightingOnly.hlsl");
	g_pPixelShaderSingleTexturePlusLightingOnly = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/TwoTexturesPlusLightingOnly.hlsl");
	g_pPixelShaderTwoTexturePlusLightingOnly = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/Skybox.hlsl");
	g_pPixelShaderSkybox = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/TwoTexturesNormalMapOnly.hlsl");
	g_pPixelShaderNormalMap = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/DiscardTransperancy.hlsl");
	g_pPixelShaderDiscardTransperacy = shaderToBeUsed->mPixelShader;

	shaderToBeUsed = FindShader(L"Shaders/DiscardTransperancyReverse.hlsl");
	g_pPixelShaderDiscardTransperacyReverse = shaderToBeUsed->mPixelShader;
}
 
