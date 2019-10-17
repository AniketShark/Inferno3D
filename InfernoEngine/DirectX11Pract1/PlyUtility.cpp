

#include "PlyUtility.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Helper.h"

using namespace std;

PlyUtility::PlyUtility()
{
	m_bContainsNormals = false;
	m_bContainsTextureCoords = false;
	m_bContainsTextureCoordsWithIndex = false;
	m_bContainsVertexColors = false;
	m_Min = Vector3::Zero;
	m_Max = Vector3::Zero;
}

PlyUtility::~PlyUtility()
{


}

PlyUtility::PlyUtility(const PlyUtility& otherPlyLoader)
{
	this->m_Max = otherPlyLoader.m_Max;
	this->m_Min = otherPlyLoader.m_Min;
	this->m_numberOfElements = otherPlyLoader.m_numberOfElements;
	this->m_numberOfVertices = otherPlyLoader.m_numberOfVertices;
	this->m_vecTriangles = otherPlyLoader.m_vecTriangles;
	this->m_vecVertices = otherPlyLoader.m_vecVertices;
	this->textureFileNames = otherPlyLoader.textureFileNames;

}

bool PlyUtility::LoadPlyFile(std::string fileName)
{
	ifstream plyFile(fileName);

	if(!plyFile.is_open())
	{
		return false;
	}

	string tempString;

	while(tempString != "vertex")
	{
		plyFile >> tempString;
		if(tempString == "TextureFile")
		{
			plyFile>>tempString;
			textureFileNames.push_back(tempString);
		}
	}
	this->m_numberOfVertices = 0;
	plyFile>> this->m_numberOfVertices; // loading no of vertices

	while(tempString != "face")
	{
		plyFile >> tempString;

		if(tempString == "nx")
			m_bContainsNormals = true;
		if(tempString == "red")
			m_bContainsVertexColors = true;
		if(tempString == "texture_u" || tempString == "s" || tempString == "u")
			m_bContainsTextureCoords = true;
	}

	this->m_numberOfElements = 0;
	plyFile >> this->m_numberOfElements;

	vector<Element> triangles; 

	while(tempString != "end_header")
	{
		plyFile >> tempString;

		if(tempString == "texcoord")
			m_bContainsTextureCoordsWithIndex = true;
	}

	vector<Vertex> vertices;
	for(int index = 0; index != this->m_numberOfVertices;index++)
	{
		Vertex tempVertex;

		plyFile >> tempVertex.m_Position.x;
		plyFile >> tempVertex.m_Position.y;
		plyFile >> tempVertex.m_Position.z;

		XMFLOAT4 pos = tempVertex.m_Position;

		if(pos.x > m_Max.x)
			m_Max.x = tempVertex.m_Position.x;
		if(pos.y > m_Max.y)
			m_Max.y = tempVertex.m_Position.y;
		if(pos.z > m_Max.z)
			m_Max.z = tempVertex.m_Position.z;

		if(pos.x < m_Min.x)
			m_Min.x = tempVertex.m_Position.x;
		if(pos.y < m_Min.y)
			m_Min.y = tempVertex.m_Position.y;
		if(pos.z < m_Min.z)
			m_Min.z = tempVertex.m_Position.z;

		if(m_bContainsNormals)
		{
			plyFile >> tempVertex.m_Normal.x;
			plyFile >> tempVertex.m_Normal.y;
			plyFile >> tempVertex.m_Normal.z;
		}

		if(m_bContainsTextureCoords)
		{
			plyFile >> tempVertex.m_TexCoord.x;
			plyFile >> tempVertex.m_TexCoord.y;

		}
		m_vecVertices.push_back(tempVertex);
	}	

	for(int triangleIndex = 0; triangleIndex != this->m_numberOfElements; triangleIndex++)
	{
		Element tempElement;
		Triangle tempTriangle;
		int tempInt;
		plyFile >> tempInt;
		plyFile>>tempElement.index_v1;
		plyFile>>tempElement.index_v2;
		plyFile>>tempElement.index_v3;

		tempTriangle.mPointA = Helper::ConvertXMFLOAT4TOVector3(GetVertexAtIndex(tempElement.index_v1).m_Position);
		tempTriangle.mPointB = Helper::ConvertXMFLOAT4TOVector3(GetVertexAtIndex(tempElement.index_v2).m_Position);
		tempTriangle.mPointC = Helper::ConvertXMFLOAT4TOVector3(GetVertexAtIndex(tempElement.index_v3).m_Position);
		
		tempElement.restlength1 = Vector3::Distance(tempTriangle.mPointA,tempTriangle.mPointB);
		tempElement.restlength2 = Vector3::Distance(tempTriangle.mPointB,tempTriangle.mPointC);
		tempElement.restlength3 = Vector3::Distance(tempTriangle.mPointC,tempTriangle.mPointA);

		tempTriangle.mNormal =  Helper::ConvertXMFLOAT4TOVector3(GetVertexAtIndex(tempElement.index_v1).m_Normal);

		tempTriangle.CalculateCenter();

		if(m_bContainsTextureCoordsWithIndex)
		{
			plyFile >> tempInt;
			plyFile>>m_vecVertices[tempElement.index_v1].m_TexCoord.x;
			plyFile>>m_vecVertices[tempElement.index_v1].m_TexCoord.y;

			plyFile>>m_vecVertices[tempElement.index_v2].m_TexCoord.x;
			plyFile>>m_vecVertices[tempElement.index_v2].m_TexCoord.y;

			plyFile>>m_vecVertices[tempElement.index_v3].m_TexCoord.x;
			plyFile>>m_vecVertices[tempElement.index_v3].m_TexCoord.y;
		}

		if(m_bContainsTextureCoords || m_bContainsTextureCoordsWithIndex)
		{
			CalculateTangentBinormal(m_vecVertices[tempElement.index_v1],m_vecVertices[tempElement.index_v2],m_vecVertices[tempElement.index_v3]);
		}

		m_vecTriangles.push_back(tempElement);
		m_TriangleListWithPoints.push_back(tempTriangle);
	}

	return true;
}

int PlyUtility::GetNumberOfVertices()
{
	return this->m_numberOfVertices;
}

int PlyUtility::GetNumberOfElements()
{
	return this->m_numberOfElements;
}

int PlyUtility::GetNumberOfTextures()
{
	return textureFileNames.size();
}

Vertex PlyUtility::GetVertexAtIndex(int index)
{
	return this->m_vecVertices[index];
}

Element PlyUtility::GetElementAtIndex(int index)
{
	return this->m_vecTriangles[index];
}

void PlyUtility::PushVertex(Vertex v)
{
	m_vecVertices.push_back(v);
}

void PlyUtility::PushElement(Element tri)
{
	m_vecTriangles.push_back(tri);
}

void PlyUtility::ScaleModelTo(float scale)
{
	std::vector<Vertex>::iterator vertexIt;
	for (vertexIt = m_vecVertices.begin(); vertexIt != m_vecVertices.end();vertexIt++)
	{
		vertexIt->ScaleVertexTo(Vector3::One*scale);
	}
}

void PlyUtility::CalculateTangentBinormal(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3)
{
	Vector3 vector1, vector2;
	XMFLOAT2 tuVector, tvVector;
	XMFLOAT4 tangent,binormal;
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1.x = vertex2.m_Position.x - vertex1.m_Position.x;
	vector1.y = vertex2.m_Position.y - vertex1.m_Position.y;
	vector1.z = vertex2.m_Position.z - vertex1.m_Position.z;

	vector2.x = vertex3.m_Position.x - vertex1.m_Position.x;
	vector2.y = vertex3.m_Position.y - vertex1.m_Position.y;
	vector2.z = vertex3.m_Position.z - vertex1.m_Position.z;

	// Calculate the tu and tv texture space vectors.
	tuVector.x = vertex2.m_TexCoord.x - vertex1.m_TexCoord.x;
	tvVector.x = vertex2.m_TexCoord.y - vertex1.m_TexCoord.y;

	tuVector.y = vertex3.m_TexCoord.x - vertex1.m_TexCoord.x;
	tvVector.y = vertex3.m_TexCoord.y - vertex1.m_TexCoord.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector.x * tvVector.y - tuVector.y * tvVector.x);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector.y * vector1.x - tvVector.x * vector2.x) * den;
	tangent.y = (tvVector.y * vector1.y - tvVector.x * vector2.y) * den;
	tangent.z = (tvVector.y * vector1.z - tvVector.x * vector2.z) * den;
	tangent.w = 1.0f;

	binormal.x = (tuVector.x * vector2.x - tuVector.y * vector1.x) * den;
	binormal.y = (tuVector.x * vector2.y - tuVector.y * vector1.y) * den;
	binormal.z = (tuVector.x * vector2.z - tuVector.y * vector1.z) * den;
	binormal.w = 1.0f;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;
	
	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	vertex1.m_Tangent = tangent;
	vertex2.m_Tangent = tangent;
	vertex3.m_Tangent = tangent;

	vertex1.m_Binormal = binormal;
	vertex2.m_Binormal = binormal;
	vertex3.m_Binormal = binormal;

	return;
}


//////////////////////////////////////////////////////////////////////////
// RenderInfo class
//////////////////////////////////////////////////////////////////////////

RenderInfo::RenderInfo()
{

}

RenderInfo::~RenderInfo()
{

}

RenderInfo::RenderInfo(const RenderInfo& other)
{
	this->mVertexBufferStartIndex = other.mVertexBufferStartIndex;
	this->mIndexBufferStartIndex = other.mIndexBufferStartIndex;
	this->mVertexBufferEndIndex = other.mVertexBufferEndIndex;
	this->mIndexBufferEndIndex = other.mIndexBufferEndIndex;
	this->mNoOfVertices = other.mNoOfVertices;
	this->mNoOfTris = other.mNoOfTris;
	this->mNoofTextures = other.mNoOfVertices;
	this->mFileName = other.mFileName;
}


void RenderInfo::operator= (const RenderInfo& other)
{
	this->mVertexBufferStartIndex = other.mVertexBufferStartIndex;
	this->mIndexBufferStartIndex = other.mIndexBufferStartIndex;
	this->mVertexBufferEndIndex = other.mVertexBufferEndIndex;
	this->mIndexBufferEndIndex = other.mIndexBufferEndIndex;
	this->mNoOfVertices = other.mNoOfVertices;
	this->mNoOfTris = other.mNoOfTris;
	this->mNoofTextures = other.mNoOfVertices;
	this->mFileName = other.mFileName;
}