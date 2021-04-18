
#ifndef Vertex_H_
#define Vertex_H_

#include <windows.h>
#include <DirectXMath.h>
//#include <xnamath.h>
#include "Vector.h"

class Vertex
{

public:

	DirectX::XMFLOAT4 m_Position;
	DirectX::XMFLOAT4 m_Normal;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT2 m_TexCoord;
	DirectX::XMFLOAT4 m_Tangent;
	DirectX::XMFLOAT4 m_Binormal;

	// animation variables
	unsigned char BoneIndex[4];    
	float         BoneWeight[4];

	Vertex();
	~Vertex();

	Vertex(float xVal,float yVal,float zVal);

	Vertex(const Vertex&);
	
	void operator= (const Vertex& );

	void ScaleVertexTo(Vector3 scale);

};

#endif 