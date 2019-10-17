#ifndef RenderableObject_H_
#define RenderableObject_H_


#include "global.h"
#include "Shader.h"
#include "Material.h"
#include <vector>
#include "FBXData.h"
#include "GameObject.h"
#include "Mesh.h"

class RenderableObject : public GameObject
{
public:
	RenderableObject();
	~RenderableObject();
	bool mIsStatic;
	Mesh* mMesh;
	Material mMaterial;
	void InitVertexAndIndexBuffer();
	void Draw();
	
private:

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer; 
	ID3D11Buffer* m_PrivateConstantBuffer;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mVertexLayout;

	
};

std::vector<RenderableObject> m_RenderableObjectList;

#endif