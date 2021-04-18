
#include "Shape.h"

std::vector<Triangle> mTriangleList;
void LoadPointerArrayFromVector(std::vector<Vertex*>& verticeVector,std::vector<WORD>& indicesVector,Vertex* vertexArray,WORD* indexArray);
/// <summary>
/// Initializes a new instance of the <see cref="Line"/> class.
/// </summary>
Line::Line(){};
/// <summary>
/// Finalizes an instance of the <see cref="Line"/> class.
/// </summary>
Line::~Line(){};

/// <summary>
/// Initializes a new instance of the <see cref="Triangle"/> class.
/// </summary>
Triangle::Triangle(){}
/// <summary>
/// Finalizes an instance of the <see cref="Triangle"/> class.
/// </summary>
Triangle::~Triangle(){}

/// <summary>
/// Initializes a new instance of the <see cref="Triangle"/> class.
/// </summary>
/// <param name="">The .</param>
Triangle::Triangle(const Triangle& other)
{
	this->mPointA = other.mPointA;
	this->mPointB = other.mPointB;
	this->mPointC = other.mPointC;
	this->mNormal = other.mNormal;
	this->mCenter = other.mCenter;
}


/// <summary>
/// Moves the specified direction.
/// </summary>
/// <param name="direction">The direction.</param>
/// <param name="delta">The delta.</param>
void Triangle::Move(Vector3 direction,float delta)
{
	//this->transform.position = this->transform.position + direction*delta;
	//this->mPointA =  this->mPointA + direction*delta;
	//this->mPointB =  this->mPointB + direction*delta;
	//this->mPointC =  this->mPointC + direction*delta;
}

/// <summary>
/// The m point a
/// </summary>
void Triangle::CalculateCenter()
{
	Vector3 center = Vector3::Zero;

	center.x = (this->mPointA.x + this->mPointB.x +this->mPointC.x)/3;
	center.y = (this->mPointA.y + this->mPointB.y +this->mPointC.y)/3; 
	center.z = (this->mPointA.z + this->mPointB.z +this->mPointC.z)/3;

	this->mCenter = center;
}

void CreatePlane(int noOfRows,int noOfColumns,int width,int depth,ShapeData& shapeData)
{
	float vertexCount = noOfRows*noOfColumns;
	shapeData.mNoOfVertices = vertexCount;
	float faceCount = (noOfRows-1) * (noOfColumns-1) * 2;
	float zoffset = 20;
	float halfWidth = width*0.5f;
	float halfdepth = depth*0.5f;

	float dx = width/(noOfColumns-1);
	float dz = depth/(noOfRows-1);

	float du = 1.0f/(noOfColumns-1);
	float dv = 1.0f/(noOfRows-1);

	//shapeData.mVertices = new Vertex[static_cast<int>(vertexCount)];
	//generating vertices's

	for (int i = 0; i <noOfColumns ; i++)
	{
		float z = (halfdepth - i * dz) + zoffset;  
		for (int j = 0; j  < noOfRows;j++)
		{
			float x = -halfWidth + j*dx;

			Vertex* tempVertex = new Vertex;

			tempVertex->m_Position = DirectX::XMFLOAT4(x,0,z,1);
			tempVertex->m_Normal = DirectX::XMFLOAT4(0,1,0,1);
			tempVertex->m_Tangent = DirectX::XMFLOAT4(1,0,0,1);
			tempVertex->m_TexCoord.x = j*du;
			tempVertex->m_TexCoord.y = i*dv;

			shapeData.mVeticesVector.push_back(tempVertex);
			//////////////////////////////////////////////////////////////////////////

			//shapeData.mVertices[i*noOfColumns+j].m_Position = XMFLOAT4(x,0,z,1);

			//shapeData.mVertices[i*noOfColumns+j].m_Normal = XMFLOAT4(0,1,0,1);
			//shapeData.mVertices[i*noOfColumns+j].m_Tangent = XMFLOAT4(1,0,0,1);


			//shapeData.mVertices[i*noOfColumns+j].m_TexCoord.x = j*du;
			//shapeData.mVertices[i*noOfColumns+j].m_TexCoord.y = i*dv;
		}
	}

	//generating Indices
	//shapeData.mIndices = new WORD[static_cast<int>(faceCount*3)];
	shapeData.mNoOfIndices = faceCount*3;
	int k = 0;
	for (int i = 0; i <noOfColumns-1 ; i++)
	{
		
		for (int j = 0; j  < noOfRows-1;j++)
		{

			shapeData.mIndicesVector.push_back(i * noOfRows+j);
			shapeData.mIndicesVector.push_back(i * noOfRows+j+1) ;
			shapeData.mIndicesVector.push_back((i+1) * noOfRows+j);
			shapeData.mIndicesVector.push_back((i+1) * noOfRows+j);
			shapeData.mIndicesVector.push_back(i * noOfRows+j+1) ;
			shapeData.mIndicesVector.push_back((i+1) * noOfRows+j+1);

			//////////////////////////////////////////////////////////////////////////
			//shapeData.mIndices[k] = i * noOfRows+j;
			//shapeData.mIndices[k + 1] = i * noOfRows+j+1;
			//shapeData.mIndices[k + 2] = (i+1) * noOfRows+j;
			//shapeData.mIndices[k + 3] = (i+1) * noOfRows+j;
			//shapeData.mIndices[k + 4] =  i * noOfRows+j+1;
			//shapeData.mIndices[k + 5] = (i+1) * noOfRows+j+1;

			k += 6; //next quad
		}
	}
	shapeData.mNoOfVertices = shapeData.mVeticesVector.size();
	shapeData.mNoOfIndices = shapeData.mIndicesVector.size();

	shapeData.mVertices = new Vertex[shapeData.mNoOfVertices];
	shapeData.mIndices  = new WORD[shapeData.mNoOfIndices];
	LoadPointerArrayFromVector(shapeData.mVeticesVector,shapeData.mIndicesVector,shapeData.mVertices,shapeData.mIndices);
	
}

void LoadPointerArrayFromVector(std::vector<Vertex*>& verticeVector,std::vector<WORD>& indicesVector,Vertex* vertexArray,WORD* indexArray)
{
	//vertexArray = new Vertex[verticeVector.size()];
	//indexArray = new WORD[indicesVector.size()];
	
	for(int vindex = 0; vindex < verticeVector.size();vindex++)
	{
		vertexArray[vindex].m_Position = verticeVector[vindex]->m_Position; 
		vertexArray[vindex].m_Normal = verticeVector[vindex]->m_Normal;
		vertexArray[vindex].m_Tangent = verticeVector[vindex]->m_Tangent;
		vertexArray[vindex].m_TexCoord.x = verticeVector[vindex]->m_TexCoord.x;
		vertexArray[vindex].m_TexCoord.y = verticeVector[vindex]->m_TexCoord.y;

		
	}
	for(int iindex = 0; iindex < indicesVector.size();iindex++)
	{
		indexArray[iindex] = indicesVector[iindex];
	}

	verticeVector.clear();
	indicesVector.clear();
	
}

