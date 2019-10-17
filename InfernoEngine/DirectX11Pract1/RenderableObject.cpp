#include "RenderableObject.h"


RenderableObject::RenderableObject()
{
	 m_VertexBuffer = NULL;
	 m_IndexBuffer = NULL; 
	 m_PrivateConstantBuffer = NULL;

	mVertexShader = NULL;
	mPixelShader = NULL;
	mVertexLayout = NULL;
}
RenderableObject::~RenderableObject()
{

}


void RenderableObject::InitVertexAndIndexBuffer()
{

	//HRESULT hr;
	////bool result;
	//// Compile the vertex shader

	//ID3DBlob* pVSBlob = NULL;
	//std::wstring error;

	//hr = CompileShaderFromFile(L"Shaders/BasicShader.fx", L"VS", L"vs_4_0", &pVSBlob);
	//if( FAILED( hr ) )
	//{
	//	MessageBox( NULL,
	//		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
	//	//return hr;
	//}

	//hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &this->mVertexShader );
	//if( FAILED( hr ) )
	//{	
	//	pVSBlob->Release();
	//	//return hr;
	//}

	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//UINT numElements = ARRAYSIZE( layout );


	//// Create the input layout
	//hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),	pVSBlob->GetBufferSize(), &this->mVertexLayout);
	//pVSBlob->Release();
	////if( FAILED( hr ) )
	////	return hr;
	//g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	//ID3DBlob* pPSBlob = NULL;
	//hr = CompileShaderFromFile(L"Shaders/BasicShader.fx", L"PS", L"ps_4_0", &pPSBlob);
	//if( FAILED( hr ) )
	//{
	//	MessageBox( NULL,
	//		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
	//	//return hr;
	//}

	//// Create the vertex shader
	//hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &this->mPixelShader);
	//if( FAILED( hr ) )
	//{	
	//	pPSBlob->Release();
	//	//return hr;
	//}

	//D3D11_BUFFER_DESC bd;
	//ZeroMemory( &bd, sizeof(bd) );
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof( SimpleVertex ) * this->mesh->mNoOfVertices;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	//ZeroMemory( &InitData, sizeof(InitData) );
	//InitData.pSysMem = this->mesh->mVertexArray;

	//hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &this->m_VertexBuffer);

	//if( FAILED( hr ) )
	//{
	//	MessageBox(NULL,L"ERROR: Unable to create vertex buffer",L"ERROR",MB_OK);
	//	//return false;
	//}

	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof( DWORD ) * this->mesh->mNoOfIndices;       
	////bd.ByteWidth = sizeof( DWORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags = 0;
	//InitData.pSysMem = this->mesh->mIndexArray;
	//hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &this->m_IndexBuffer );
	//if( FAILED( hr ) )
	//{
	//	MessageBox(NULL,L"ERROR: Unable to create index buffer",L"ERROR",MB_OK);
	//	//return false;
	//}


	//// Create the constant buffer
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(PrivateConstantBuffer);
	//bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = 0;
	//hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &this->m_PrivateConstantBuffer);
	//if( FAILED( hr ) )
	//	return;
}


void RenderableObject::Draw()
{

	//XMMATRIX transform =  XMMatrixScaling(transform.scale.x,transform.scale.y,transform.scale.z) *  XMMatrixRotationQuaternion(transform.GetWorldRotationQuaternion()) * XMMatrixTranslation(transform.position.x,transform.position.y,transform.position.z) ;
	//mConstantBuffer.mWorld = XMMatrixTranspose( transform );
	//mConstantBuffer.mView = XMMatrixTranspose( g_View );
	//mConstantBuffer.mProjection = XMMatrixTranspose( g_Projection);
	//g_pImmediateContext->UpdateSubresource( mPrivateConstantBuffer, 0, NULL, &mConstantBuffer, 0, 0 );

	//// Set vertex buffer
	//UINT stride = sizeof( SimpleVertex );
	//UINT offset = 0;
	//g_pImmediateContext->IASetInputLayout( mVertexLayout );
	////g_pImmediateContext->UpdateSubresource( mVertexBuffer, 0, NULL, 0, 0, 0 );
	//g_pImmediateContext->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );
	//g_pImmediateContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//// Render a triangle
	//// Set the input layout

	//g_pImmediateContext->VSSetShader( mVertexShader, NULL, 0 );
	//g_pImmediateContext->VSSetConstantBuffers( 0, 1, &mPrivateConstantBuffer );
	//g_pImmediateContext->PSSetShader( mPixelShader, NULL, 0 );
	//g_pImmediateContext->DrawIndexed( mNoOfIndices, 0, 0 );
}