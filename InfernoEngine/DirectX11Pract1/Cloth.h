//// ***********************************************************************
//// Assembly         : InfernoEngine
//// Author           : Aniket Kayande
//// Created          : 02-15-2014
////
//// Last Modified By : Aniket Kayande
//// Last Modified On : 02-17-2014
//// ***********************************************************************
//// <copyright file="Cloth.h" company="Inferno Studio">
////     Copyright (c) Inferno Studio. All rights reserved.
//// </copyright>
//// <summary></summary>
//// ***********************************************************************


#ifndef Cloth_H_
#define Cloth_H_
//
//
#include "IBehavior.h"
#include "VerletFiles\CParticleSystem.h"
#include "Mesh.h"
#include "xnacollision.h"
#include "Vector.h"

///// <summary>
///// Class Cloth.
///// </summary>
class Cloth : public IBehaviour
{
public:
	Cloth();
	~Cloth();
	/// <summary>
	/// The m no of rows
	/// </summary>
	int mNoOfRows;
	/// <summary>
	/// The m no of column
	/// </summary>
	int mNoOfColumn;
	/// <summary>
	/// The m step
	/// </summary>
	int mStep;
	/// <summary>
	/// No of iterations for varlet solver
	/// </summary>
	int mNoOfIterations;
	/// <summary>
	/// Attached mesh
	/// </summary>
	Mesh* mMesh;

	//////////////////////////////////////////////////////////////////////////
	// Threading Variables
	//////////////////////////////////////////////////////////////////////////
	bool mStartSimulation;
	HANDLE mTheradHandle;
	DWORD mThreadID;	
	LPDWORD mThread;
	//////////////////////////////////////////////////////////////////////////
	//  For Strings of beads
	//////////////////////////////////////////////////////////////////////////

	bool mIsStringOfBeads;
	int mNoOfObjects;
	
	virtual void Initialize();
	void InitCloth(int mNoOfRows,int mNoOfColumn,int mStep);
	//Updates Component every frame
	virtual void Update();
	//Destroys and releases all the resources when called
	virtual void Destroy();
	//clones this instance
	virtual IBehaviour* Clone();
	virtual ComponentType GetType();
	void Kerplode();
	void RandomPurturb();
	Vector3 GetParticleAt(int);
	Vector3 GetParticleAtOLD(int);
	void StiffnessFactor(int noOfIterations);
	void ApplyAccelerationInDirection(Vector3 direction,float power);
	CParticleSystem m_InternalParticleSystem;
	XNA::AxisAlignedBox* mCrateCollider;
	XNA::Sphere* mPointColliders;
	int mNoOfColliders;
private:
	void VerletFunc();
	int CalculateIndex( int row, int col, int numRows );

	int m_ParticleCount;
	int m_MaxIndex;
};
#endif