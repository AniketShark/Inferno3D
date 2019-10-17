#ifndef PlyUtility_H_
#define PlyUtility_H_

#include <string>
#include<vector>
#include "Vertex.h"
#include "Vector.h"
#include "Element.h"
#include "Shape.h"

class PlyUtility
{
public:
	PlyUtility();
	PlyUtility(const PlyUtility&);
	virtual ~PlyUtility();	// Rule of thumb: make destructor virtual

	bool LoadPlyFile( std::string );

	int GetNumberOfVertices(void);
	int GetNumberOfElements(void);
	int GetNumberOfTextures(void);

	Vertex GetVertexAtIndex( int index );
	Element GetElementAtIndex( int index );
	void ScaleModelTo(float scale);

	bool GetTriangleVerticesAt( int index, Vertex &v1, Vertex &v2, Vertex &v3);
	void PushVertex(Vertex v);
	void PushElement(Element tri);
	void CalculateTangentBinormal(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3);
	std::vector<std::string> textureFileNames;
	std::vector<Triangle> m_TriangleListWithPoints;
	Vector3 m_Min;
	Vector3 m_Max;

protected:

	int m_numberOfVertices;
	std::vector< Vertex > m_vecVertices;
	int m_numberOfElements;
	std::vector< Element > m_vecTriangles;

	Vertex* m_VertexArray;
	DWORD* m_IndexArray;

	bool m_bContainsNormals;
	bool m_bContainsTextureCoords;
	bool m_bContainsTextureCoordsWithIndex;
	bool m_bContainsVertexColors;
};


class RenderInfo
{

public:
	RenderInfo();
	RenderInfo(const RenderInfo&);
	~RenderInfo();

	std::string mFileName;
	int mNoOfVertices;
	int mNoOfIndices;
	int mNoOfTris;
	int mNoofTextures;

	int mVertexBufferStartIndex;
	int mVertexBufferEndIndex;

	int mIndexBufferStartIndex;
	int mIndexBufferEndIndex;

	void operator= (const RenderInfo&);

};
#endif