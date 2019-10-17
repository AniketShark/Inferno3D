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
#include "Vector.h"
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
	Vector3 mStart;
	/// <summary>
	/// The m end
	/// </summary>
	Vector3 mEnd;
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
	Vector3 mNormal;
	Vector3 mCenter;
	Vector3 mPointA;
	Vector3 mPointB;
	Vector3 mPointC;
	

	Line mLineAB;
	Line mLineBC;
	Line mLineCA;

	void Move(Vector3 direction,float delta);
	void CalculateCenter();
};

void CreatePlane(int noOfRows,int noOfColumns,int width,int height,ShapeData& shapeData);

extern std::vector<Triangle> mTriangleList;
extern int vertexOffset;
extern int indexOffset;

#endif