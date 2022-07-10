#include "Shader.h"
#include "Helper.h"

// This is based on the helper file from the DirectX 11 SDK
// Uses all wstring (instead of a mix of unicode and ASCII)
// Returns the compiled shader in ppBlobOut.
// false if error with error text in 'error' wstring.
bool CompileShaderFromFile( std::wstring shaderFileName, 
	std::wstring entryPoint, 
	std::wstring shaderModel, 
	ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	if ( true )	
		dwShaderFlags |= D3DCOMPILE_DEBUG;

	// 
	ID3DBlob* pErrorBlob = NULL;
	std::string ASCIIentryPoint = Helper::UNICODEtoASCII(entryPoint);
	std::string ASCIIshaderModel = Helper::UNICODEtoASCII(shaderModel);
	hr = D3DCompileFromFile( 
		shaderFileName.c_str(), 
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ASCIIentryPoint.c_str(), 
		ASCIIshaderModel.c_str(), 
		dwShaderFlags,  
		NULL,
		ppBlobOut, 
		&pErrorBlob );

	if( FAILED(hr) )
	{
		if( pErrorBlob != NULL )
		{
			OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
			std::string ASCIIerror( (char*)pErrorBlob->GetBufferPointer() );
			std::wstring error = Helper::ASCIItoUNICODE ( ASCIIerror );
			MessageBox(NULL,error.c_str(),L"Error",MB_OK);
		}
		if( pErrorBlob ) pErrorBlob->Release();
		return false;
	}
	// Release the blob if was OK
	if( pErrorBlob ) pErrorBlob->Release();

	return true;
}


/// <summary>
/// Initializes a new instance of the <see cref="Shader"/> class.
/// </summary>
Shader::Shader()
{

}

//Shader::Shader(const Shader& otherShader)
//{
//	this->mNoOfTexturesShaderNeeds = otherShader.mNoOfTexturesShaderNeeds;
//	delete this->mArrayOfTextures;
//	mArrayOfTextures = new Texture[otherShader.mNoOfTexturesShaderNeeds];
//	for (int i = 0; i != otherShader.mNoOfTexturesShaderNeeds; i++)
//	{
//		mArrayOfTextures[i] = otherShader.mArrayOfTextures[i];  
//	}
//	this->mArrayOfTextures = otherShader.mArrayOfTextures;
//	this->mShaderName = otherShader.mShaderName;
//
//	this->mVertexShader = otherShader.mVertexShader;
//	this->mPixelShader = otherShader.mPixelShader;
//	this->mVertexShaderEntryPonit = otherShader.mVertexShaderEntryPonit;
//	this->mPixelShaderEntryPoint = otherShader.mPixelShaderEntryPoint;
//}









/// <summary>
/// Copies the properties from shader.
/// </summary>
/// <param name="">The .</param>
void Shader::CopyPropertiesFromShader(Shader& otherShader)
{
	this->mNoOfTexturesShaderNeeds = otherShader.mNoOfTexturesShaderNeeds;
	delete this->mArrayOfTextures;
	mArrayOfTextures = new Texture[otherShader.mNoOfTexturesShaderNeeds];
	for (int i = 0; i != otherShader.mNoOfTexturesShaderNeeds; i++)
	{
		mArrayOfTextures[i] = otherShader.mArrayOfTextures[i];  
	}
	this->mArrayOfTextures = otherShader.mArrayOfTextures;
	this->mShaderName = otherShader.mShaderName;

	this->mVertexShader = otherShader.mVertexShader;
	this->mPixelShader = otherShader.mPixelShader;
	this->mVertexShaderEntryPonit = otherShader.mVertexShaderEntryPonit;
	this->mPixelShaderEntryPoint = otherShader.mPixelShaderEntryPoint;
}



/// <summary>
/// Finalizes an instance of the <see cref="Shader"/> class.
/// </summary>
Shader::~Shader()
{
	//delete mArrayOfTextures;
}

/// <summary>
/// Creates the vertex and pixel shader.
/// </summary>
void Shader ::CreateVertexAndPixelShader()
{
	HRESULT hr;
	// Compile the vertex shader

	ID3DBlob* pVSBlob = NULL;

	hr = CompileShaderFromFile(this->mShaderName, this->mVertexShaderEntryPonit, L"vs_4_0", &pVSBlob);
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		//return hr;
	}

	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &this->mVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
		//return hr;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,56,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,72,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"JOINTINDEXES",0,DXGI_FORMAT_R8G8B8A8_UINT,0,88,D3D11_INPUT_PER_VERTEX_DATA, 0	},
		{"JOINTWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,92,D3D11_INPUT_PER_VERTEX_DATA, 0}

		
	};
	UINT numElements = ARRAYSIZE( layout );


	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),	pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	//if( FAILED( hr ) )
	//	return hr;
	g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(this->mShaderName, this->mPixelShaderEntryPoint, L"ps_4_0", &pPSBlob );
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		//return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &this->mPixelShader);
	if( FAILED( hr ) )
	{	
		pPSBlob->Release();
		//return hr;
	}

}