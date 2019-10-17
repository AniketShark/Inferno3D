// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-09-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-10-2014
// ***********************************************************************
// <copyright file="Mesh.h" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#ifndef MESH_H
#define MESH_H

#include "HelperFuncs.h"
#include "Vertex.h"
#include "FbxHandler.h"
#include "PlyUtility.h"
#include "Texture.h"

/// <summary>
/// Enum FILETYPE
/// </summary>
enum FILETYPE
{
	/// <summary>
	/// The ply
	/// </summary>
	PLY,
	/// <summary>
	/// The FBX
	/// </summary>
	FBX,
	/// <summary>
	/// The Procedural
	/// </summary>
	PROCEDURAL
};


/// <summary>
/// Class Mesh.
/// </summary>
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	
	//////////////////////////////////////////////////
	// Common data structures for all type of meshes
	//////////////////////////////////////////////////
	
	std::string      mModelName;
	FILETYPE         mFileType;

	bool			 mIsStatic;
	bool			 mIsInGlobalVB;
	WORD*            mIndices;
	int              mNoOfVertices;
	int              mNumIndices;
	Texture*		 mTextureArray;
	std::vector<std::string> mTextureNameList;			
	int				 mNumTextures;
	RenderInfo		 mRenderInfo;

	//////////////////////////////////////////////////
	// fbx specific Data structures
	//////////////////////////////////////////////////
	
	Bone*                mSkeleton;
	/// <summary>
	/// Contains local transform of each bone with respect to its parents
	/// </summary>
	BonePose*            mInitialBonePose;
	/// <summary>
	/// holds the global matrix of each bone in particular pose in animation
	/// </summary>
	_XMFLOAT4X4*          mInitialGlobalPose;
	/// <summary>
	///Total no of bones in the fbx model
	/// </summary>
	int                   mNoOfBones;

	Vertex*  mVertices;

	PlyUtility* mPlyInfo;
	
	Element* mElements;
	int mNoOfElements;

	void operator= (const Mesh*);
	void operator= (const Mesh&);
};

#endif
