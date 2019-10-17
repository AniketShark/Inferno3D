

#ifndef global_H_
#define global_H_


#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "Material.h"
#include "Vertex.h"
#include "Input.h"

#define MAX_LIGHTS 10
#define MAX_BONES 255

extern UINT DIFFUSE_TEXTURE_1;
extern UINT DIFFUSE_TEXTURE_2;
extern UINT SKYBOX_TEXTURE;
extern UINT BUMPMAP_TEXTURE;


extern Input g_input;
extern int g_LastLoadedModelEndIndex;
extern int g_LastIndexLoaded;


extern int g_LastIndexBufferIndex;
extern int g_LastVertexBufferIndex;

extern int g_TriangleIndexBufferSize;
extern int g_NoOfVerticesInTriangleBuffer;

extern int g_BallIndexBufferSize;
extern int g_NoOfVerticesInBallBuffer;

extern float g_globalDeltaTime;

//////////////////////////////////////////////////////////////////////////
// Global Static Vertex and Index Arrays
//////////////////////////////////////////////////////////////////////////

extern int g_TotalNoOfTriangles;
extern int g_TotalNoOfIndices;
extern int g_TotalNoOfVertices;
extern Vertex* g_GlobalStaticVertexArray;
extern WORD* g_GlobalStaticIndexArray;


struct LightInfo
{
	XMFLOAT4 vPosition;
	XMFLOAT4 vLightDirection;
	XMFLOAT4 attenuation;
	float power;
	float range;
	float typeOfLight;
	float padding;
	Material material;

};

struct ConstantBuffer
{
	XMMATRIX  mWorld;
	XMMATRIX  mView;
	XMMATRIX  mProjection;
	XMMATRIX  mInverseTransposedWorld;
	XMFLOAT4  mEyePosition;
	UINT	  mNoOfLights;
	UINT      mNoOfBones;
	UINT      mGpuSkinnig; 
	UINT      mPadding;
	LightInfo mlights[MAX_LIGHTS];
};

struct ChangingBuffer
{
	Material mObjectMaterial;
	XMFLOAT4 mTransperacyOn;
};

struct AnimationMatrices
{
	_XMFLOAT4X4 mBoneMatrices[128];
};


extern ConstantBuffer g_pGlobalConstantBuffer;
extern ChangingBuffer g_pGlobalChangingBuffer;
extern AnimationMatrices g_pAnimationMatrices;


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
extern HINSTANCE               g_hInst;// = NULL;
extern HWND                    g_hWnd ;//= NULL;
extern D3D_DRIVER_TYPE         g_driverType;// = D3D_DRIVER_TYPE_NULL;
extern D3D_FEATURE_LEVEL       g_featureLevel;// = D3D_FEATURE_LEVEL_11_0;
extern ID3D11Device*           g_pd3dDevice;// = NULL;
extern ID3D11DeviceContext*    g_pImmediateContext;// = NULL;
extern IDXGISwapChain*         g_pSwapChain;// = NULL;
extern ID3D11RenderTargetView* g_pRenderTargetView;// = NULL;

extern ID3D11DepthStencilView* g_pDepthStencilView;
extern ID3D11Texture2D*		   g_pDepthStencilBuffer;
extern ID3D11Texture2D*		   g_pBlurredTexture;
extern ID3D11VertexShader*     g_pVertexShader;// = NULL;
extern ID3D11VertexShader*     g_pSkinnigVertexShader;// = NULL;


//All pixel shaders 

extern ID3D11PixelShader*      g_pPixelShaderLightingOnly;// = NULL;
extern ID3D11PixelShader*      g_pPixelShaderSingleTextureOnly;
extern ID3D11PixelShader*      g_pPixelShaderTwoTexturesOnly;
extern ID3D11PixelShader*      g_pPixelShaderSingleTexturePlusLightingOnly;
extern ID3D11PixelShader*      g_pPixelShaderTwoTexturePlusLightingOnly;
extern ID3D11PixelShader*      g_pPixelShaderSkybox;
extern ID3D11PixelShader*      g_pPixelShaderNormalMap;
extern ID3D11PixelShader*      g_pPixelShaderDiscardTransperacy;
extern ID3D11PixelShader*      g_pPixelShaderDiscardTransperacyReverse;
extern ID3D11PixelShader*      g_pPixelShaderDiscardTransperacyReverse;
extern ID3D11PixelShader*      g_pPixelShaderSolid;

//All Vertex Layouts
extern ID3D11InputLayout*      g_pVertexLayout;
// Constant Buffers
extern ID3D11Buffer*           g_pVertexBuffer;
extern ID3D11Buffer*           g_pIndexBuffer;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Buffer*		   g_pChangingBuffer;
extern ID3D11Buffer*		   g_pBoneMatrixBuffer;

//Sampler States
extern ID3D11SamplerState*     g_pSamplerStateLinear;
extern ID3D11SamplerState*     g_pSamplerStateAnisotropic;

//Texture resource views Map
extern ID3D11ShaderResourceView* g_pShaderRVTex1; 
extern ID3D11ShaderResourceView* g_pShaderRVTex2; 
extern ID3D11ShaderResourceView* g_pBlurredTextureResource; 

// Cube Map
extern ID3D11ShaderResourceView*	g_pCubeMapTexture;
extern ID3D11UnorderedAccessView*	g_pBlurredUAV;


//Rasterizer States
extern ID3D11RasterizerState*  g_pRasterizerState;
extern ID3D11RasterizerState*  g_pRS_WireFrameNoCull;
extern ID3D11RasterizerState*  g_pRS_SolidCullBack;
extern ID3D11RasterizerState*  g_pRS_SolidCullNone;
extern ID3D11RasterizerState*  g_pRS_SolidCullFront;
extern ID3D11RasterizerState*  g_pRS_CullClockwise;

// Depth-Stencil States
extern ID3D11DepthStencilState * g_pDSStateNormal;
extern ID3D11DepthStencilState * g_pDSStateMaskStencil;
extern ID3D11DepthStencilState * g_pDSStateDrawInside;
extern ID3D11DepthStencilState * g_pDSStateDrawOutside;

//Blend States
extern ID3D11BlendState*      g_pBlendState_Transparent;
extern ID3D11BlendState*      g_pBlendState_Normal;
extern ID3D11BlendState*      g_pBlendState_DontWriteToRenderTarget;

extern XMMATRIX                g_World;
extern XMMATRIX                g_View;
extern XMMATRIX                g_Projection;

#endif