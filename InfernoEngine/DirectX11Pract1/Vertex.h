
#ifndef Vertex_H_
#define Vertex_H_

#include <windows.h>
#include <xnamath.h>
#include "Vector.h"

class Vertex
{

public:

	XMFLOAT4 m_Position;
	XMFLOAT4 m_Normal;
	XMFLOAT4 m_Color;
	XMFLOAT2 m_TexCoord;
	XMFLOAT4 m_Tangent;
	XMFLOAT4 m_Binormal;

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