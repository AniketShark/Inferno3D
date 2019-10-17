#ifndef RenderToTexture_H_
#define RenderToTexture_H_

#include "global.h"
#include "GameObject.h"
class RenderToTexture
{
public:
	RenderToTexture();
	~RenderToTexture();

	bool Initialize(ID3D11Device* device,int textureWidth,int textureHeight,DXGI_FORMAT format);

	void RenderObjectToTexture(GameObject* go);

	

	void SetDeviceContext(ID3D11DeviceContext*);

	void ClearRenderTarget(ID3D11DeviceContext*, float[4]);

	ID3D11ShaderResourceView* GetSRVOfTargetTexture();

	void SetViewProjectionMatrix(XMMATRIX*);

	int GetTextureWidth();
	int GetTextureHeight();

	void Begin();
	void End();
	void Release();

private:

	int mTextureWidth,mTextureHeight;

	ID3D11Texture2D* mTargetTexture;
	ID3D11ShaderResourceView* mTargetSRV;
	ID3D11RenderTargetView* mTargetRTV;

	ID3D11Texture2D* mTargetDepthMap;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11RenderTargetView* mDepthRTV;

	ID3D11DepthStencilView* mDepthMapDSV;
	ID3D11DeviceContext* mDeviceContext;
	
	XMMATRIX* mViewProjectionMatrix;

	DXGI_FORMAT mDepthMapFormat;
	DXGI_FORMAT mTextureFormat;

	bool m_CreateDepthMap(ID3D11Device* device);
	bool m_CreateColorMap(ID3D11Device* device);

};

#endif