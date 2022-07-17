// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 01-24-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-17-2014
// ***********************************************************************
// <copyright file="Shape.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef Shape_H_
#define Shape_H_

#include <vector>
#include "Vertex.h"

/// <summary>
/// Struct ShapeData
/// </summary>
struct ShapeData
{
	/// <summary>
	/// The m no of vertices
	/// </summary>
	unsigned int mNoOfVertices;
	/// <summary>
	/// The m no of indices
	/// </summary>
	unsigned int mNoOfIndices;
	/// <summary>
	/// The m vertices
	/// </summary>
	Vertex* mVertices;
	/// <summary>
	/// The m indices
	/// </summary>
	WORD* mIndices;
	std::vector<Vertex*> mVeticesVector;

	std::vector<WORD> mIndicesVector;
};
/// <summary>
/// Class Line.
/// </summary>
class Line
{
public:
	Line();
	~Line();
	/// <summary>
	/// The m start
	/// </summary>
	DirectX::XMFLOAT4 mStart;
	/// <summary>
	/// The m end
	/// </summary>
	DirectX::XMFLOAT4 mEnd;
};

/// <summary>
/// Class Triangle.
/// </summary>
class Triangle
{
public:
	Triangle();
	Triangle(const Triangle&); 
	~Triangle();

	DirectX::XMFLOAT4 mNormal;
	DirectX::XMFLOAT4 mCenter;
	DirectX::XMFLOAT4 mPointA;
	DirectX::XMFLOAT4 mPointB;
	DirectX::XMFLOAT4 mPointC;

	Line mLineAB;
	Line mLineBC;
	Line mLineCA;

	void Move(DirectX::XMFLOAT4 direction,float delta);
	void CalculateCenter();
};

void CreatePlane(int noOfRows,int noOfColumns,int width,int height,ShapeData& shapeData);

extern std::vector<Triangle> mTriangleList;
extern int vertexOffset;
extern int indexOffset;

#endif