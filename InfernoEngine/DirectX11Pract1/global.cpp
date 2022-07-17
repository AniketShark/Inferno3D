#include "global.h"


Input g_input;
int g_LastLoadedModelEndIndex = 0;

int g_LastIndexLoaded = 0;

int g_LastIndexBufferIndex = 0;
int g_LastVertexBufferIndex = 0;

int g_TriangleIndexBufferSize = 0;
int g_NoOfVerticesInTriangleBuffer = 0;

int g_BallIndexBufferSize = 0;
int g_NoOfVerticesInBallBuffer = 0;

float g_globalDeltaTime = 0.0f;

//////////////////////////////////////////////////////////////////////////
// Global Static Vertex and Index Arrays
//////////////////////////////////////////////////////////////////////////
int g_TotalNoOfVertices = 0;
int g_TotalNoOfTriangles = 0;
int g_TotalNoOfIndices = 0;

//Vertex* g_GlobalStaticVertexArray = NULL;
//WORD* g_GlobalStaticIndexArray = NULL;

ConstantBuffer g_pGlobalConstantBuffer;
ChangingBuffer g_pGlobalChangingBuffer;
AnimationMatrices g_pAnimationMatrices;


UINT DIFFUSE_TEXTURE_1 = 0;
UINT DIFFUSE_TEXTURE_2 = 1;
UINT SKYBOX_TEXTURE = 2;
UINT BUMPMAP_TEXTURE = 3;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11DepthStencilView* g_pDepthStencilView = NULL;
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11VertexShader*     g_pSkinnigVertexShader = NULL;


ID3D11Texture2D*		g_pDepthStencilBuffer = NULL;
ID3D11Texture2D*		g_pBlurredTexture = NULL;


//All pixel shader
ID3D11PixelShader*      g_pPixelShaderLightingOnly = NULL;
ID3D11PixelShader*      g_pPixelShaderSingleTextureOnly = NULL;
ID3D11PixelShader*      g_pPixelShaderTwoTexturesOnly = NULL;
ID3D11PixelShader*      g_pPixelShaderSingleTexturePlusLightingOnly  = NULL;
ID3D11PixelShader*      g_pPixelShaderTwoTexturePlusLightingOnly = NULL;
ID3D11PixelShader*      g_pPixelShaderSkybox = NULL;
ID3D11PixelShader*      g_pPixelShaderNormalMap = NULL;
ID3D11PixelShader*      g_pPixelShaderDiscardTransperacy = NULL;
ID3D11PixelShader*      g_pPixelShaderDiscardTransperacyReverse = NULL;


ID3D11PixelShader*        g_pPixelShaderSolid = NULL;
ID3D11InputLayout*        g_pVertexLayout = NULL;
ID3D11Buffer*             g_pVertexBuffer = NULL;
ID3D11Buffer*             g_pIndexBuffer = NULL;
ID3D11Buffer*             g_pConstantBuffer = NULL;
ID3D11Buffer*			  g_pChangingBuffer = NULL;
ID3D11Buffer*			  g_pBoneMatrixBuffer = NULL;
ID3D11Buffer*             g_pLightBuffer = NULL;

ID3D11SamplerState*       g_pSamplerStateLinear = NULL;
ID3D11SamplerState*		  g_pSamplerStateAnisotropic= NULL;

//Texture views
ID3D11ShaderResourceView* g_pShaderRVTex1 = NULL;
ID3D11ShaderResourceView* g_pShaderRVTex2 = NULL; 
ID3D11ShaderResourceView* g_pBlurredTextureResource = NULL; 
ID3D11ShaderResourceView*	g_pCubeMapTexture = NULL ;
ID3D11UnorderedAccessView*	g_pBlurredUAV = NULL;

ID3D11RasterizerState*  g_pRasterizerState = NULL;
ID3D11RasterizerState*  g_pRS_WireFrameNoCull= NULL;
ID3D11RasterizerState*  g_pRS_SolidCullBack= NULL;
ID3D11RasterizerState*	g_pRS_SolidCullNone = NULL;
ID3D11RasterizerState*  g_pRS_SolidCullFront= NULL;
ID3D11RasterizerState*  g_pRS_CullClockwise = NULL;

// Depth-stencil states
ID3D11DepthStencilState* g_pDSStateNormal = NULL;
ID3D11DepthStencilState* g_pDSStateMaskStencil = NULL;
ID3D11DepthStencilState* g_pDSStateDrawInside = NULL;
ID3D11DepthStencilState * g_pDSStateDrawOutside = NULL;


//blend states
ID3D11BlendState*      g_pBlendState_Transparent = NULL;
ID3D11BlendState*      g_pBlendState_Normal = NULL;
ID3D11BlendState*      g_pBlendState_DontWriteToRenderTarget = NULL;


DirectX::XMMATRIX                g_World;
DirectX::XMMATRIX                g_View;
DirectX::XMMATRIX                g_Projection;
