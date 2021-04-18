#include "Vertex.h"
#include <iostream> 


Vertex::Vertex()
{
	m_Position = DirectX::XMFLOAT4(0,0,0,1);
	m_Color =    DirectX::XMFLOAT4(0,0,0,1);
	m_Normal =   DirectX::XMFLOAT4(0,0,0,1);
	m_Tangent =  DirectX::XMFLOAT4(0,0,0,1);
	m_TexCoord = DirectX::XMFLOAT2(0,0);

	{
		BoneIndex[0] = BoneIndex[1] = BoneIndex[2] = BoneIndex[3] = 0; 
		BoneWeight[0] = BoneWeight[1] = BoneWeight[2] = BoneWeight[3] = 0.0f; 
	}

}

Vertex::Vertex(const Vertex& other)
{
	m_Position = other.m_Position;
	m_Normal = other.m_Normal;
	m_TexCoord = other.m_TexCoord;
	m_Color = other.m_Color;
	m_Tangent = other.m_Tangent;
	m_Binormal = other.m_Binormal;

	{
		BoneIndex[0] = other.BoneIndex[0];
		BoneIndex[1] = other.BoneIndex[1];
		BoneIndex[2] = other.BoneIndex[2]; 
		BoneIndex[3] = other.BoneIndex[3];

		BoneWeight[0] = other.BoneWeight[0];
		BoneWeight[1] = other.BoneWeight[1];
		BoneWeight[2] = other.BoneWeight[2]; 
		BoneWeight[3] = other.BoneWeight[3];
	}
}

Vertex::Vertex(float xVal,float yVal,float zVal)
{
	m_Position.x = xVal;
	m_Position.y = yVal;
	m_Position.z = zVal;
}


Vertex::~Vertex()
{

}

void Vertex::operator= (const Vertex& other)
{
	m_Position = other.m_Position;
	m_Normal = other.m_Normal;
	m_TexCoord = other.m_TexCoord;
	m_Color = other.m_Color;
	m_Tangent = other.m_Tangent;
	m_Binormal = other.m_Binormal;

	{
		BoneIndex[0] = other.BoneIndex[0];
		BoneIndex[1] = other.BoneIndex[1];
		BoneIndex[2] = other.BoneIndex[2]; 
		BoneIndex[3] = other.BoneIndex[3];

		BoneWeight[0] = other.BoneWeight[0];
		BoneWeight[1] = other.BoneWeight[1];
		BoneWeight[2] = other.BoneWeight[2]; 
		BoneWeight[3] = other.BoneWeight[3];
	}
}


void Vertex::ScaleVertexTo(Vector3 scale)
{
	this->m_Position.x *= scale.x;
	this->m_Position.y *= scale.y;
	this->m_Position.z *= scale.z;
}
