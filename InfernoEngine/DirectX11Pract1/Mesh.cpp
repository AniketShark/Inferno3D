// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-08-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-10-2014
// ***********************************************************************
// <copyright file="Mesh.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#include "Mesh.h"

/// <summary>
/// Initializes a new instance of the <see cref="Mesh" /> class.
/// </summary>
Mesh::Mesh() 
{
	//mVerts(NULL),
	//  mOrigVerts(NULL)
	mIndices = NULL;
	mNoOfVertices = 0;
	mNumIndices = 0;
	mSkeleton = (NULL);
	mInitialBonePose = (NULL);
	mInitialGlobalPose = (NULL);
	mNoOfBones = (0);

	 mIsInGlobalVB = false;
	 mIsStatic = false;
	 mTextureArray = NULL;
	 mVertices = NULL;
}

	/// <summary>
/// Finalizes an instance of the <see cref="Mesh" /> class.
/// </summary>
Mesh::~Mesh()
{
	//if(mVerts) delete [] mVerts;
	if(mIndices) delete [] mIndices;

    if(mSkeleton) delete [] mSkeleton;
    if(mInitialBonePose) delete [] mInitialBonePose;
    if(mInitialGlobalPose) delete [] mInitialGlobalPose;

    //if(mOrigVerts) delete [] mOrigVerts;
	if(mTextureArray)
		delete mTextureArray;
	if(mVertices)
		delete mVertices;
}


/// <summary>
/// Operator=s the specified other.
/// </summary>
/// <param name="">The .</param>
void Mesh::operator= (const Mesh* other)
{
	mModelName = other->mModelName;
	mFileType = other->mFileType;

	mIsStatic = other->mIsStatic;
	mIsInGlobalVB = other->mIsInGlobalVB;
	mIndices = other->mIndices;
	mNoOfVertices = other->mNoOfVertices;
	mNumIndices = other->mNumIndices;
	mNumTextures = other->mNumTextures;
	if(other->mTextureArray != NULL)
		mTextureArray = other->mTextureArray;
	if(other->mTextureNameList.size() > 0)
	{
		for (int i = 0; i < other->mTextureNameList.size();i++)
		{
			mTextureNameList.push_back(other->mTextureNameList[i]);
		}
	}

	mRenderInfo = other->mRenderInfo;

	//////////////////////////////////////////////////
	// fbx specific Data structures
	//////////////////////////////////////////////////
	//if(other->mVerts)
	//	mVerts = other->mVerts;
	if(other->mSkeleton)
		mSkeleton = other->mSkeleton;
	if(other->mInitialBonePose)
		mInitialBonePose = other->mInitialBonePose;
	if(other->mInitialGlobalPose)
		mInitialGlobalPose = other->mInitialGlobalPose;

	mNoOfBones = other->mNoOfBones;
	//if(other->mOrigVerts)
	//	mOrigVerts = other->mOrigVerts;

	//////////////////////////////////////////////////
	// Ply model specific Data structures
	//////////////////////////////////////////////////
	if(other->mVertices)
		mVertices = other->mVertices;
}
/// <summary>
/// Operator=s the specified other.
/// </summary>
/// <param name="">The .</param>
void Mesh::operator= (const Mesh& other)
{
	mModelName = other.mModelName;
	mFileType = other.mFileType;

	mIsStatic = other.mIsStatic;
	mIsInGlobalVB = other.mIsInGlobalVB;
	mIndices = other.mIndices;
	mNoOfVertices = other.mNoOfVertices;
	mNumIndices = other.mNumIndices;
	mNumTextures = other.mNumTextures;
	if(other.mTextureArray != NULL)
		mTextureArray = other.mTextureArray;
	if(other.mTextureNameList.size() > 0)
	{
		for (int i = 0; i < other.mTextureNameList.size();i++)
		{
			mTextureNameList.push_back(other.mTextureNameList[i]);
		}
	}

	mRenderInfo = other.mRenderInfo;

	//////////////////////////////////////////////////
	// fbx specific Data structures
	//////////////////////////////////////////////////
	//if(other.mVerts)
	//	mVerts = other.mVerts;
	if(other.mSkeleton)
		mSkeleton = other.mSkeleton;
	if(other.mInitialBonePose)
		mInitialBonePose = other.mInitialBonePose;
	if(other.mInitialGlobalPose)
		mInitialGlobalPose = other.mInitialGlobalPose;

	int  mNumBones = other.mNoOfBones;
	//if(other.mOrigVerts)
	//	mOrigVerts = other.mOrigVerts;

	//////////////////////////////////////////////////
	// Ply model specific Data structures
	//////////////////////////////////////////////////
	if(other.mVertices)
		mVertices = other.mVertices;
}
