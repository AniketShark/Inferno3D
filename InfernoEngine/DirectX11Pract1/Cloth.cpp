#include "Cloth.h"
#include "SharedInfo.h"
#include "Helper.h"

#include <math.h>
/// <summary>
/// Initializes a new instance of the <see cref="Cloth"/> class.
/// </summary>
CRITICAL_SECTION SimulationCriticalSection;
DWORD WINAPI SatisfyConstraints( PVOID pvParam);
bool startSimulation = false;
CParticleSystem* particleSysRef;
Cloth::Cloth()
{
	mMesh = NULL;
	mStartSimulation = false;
	mThisGO = new SharedInfo;
}
/// <summary>
/// Finalizes an instance of the <see cref="Cloth"/> class.
/// </summary>
Cloth::~Cloth()
{
	DeleteCriticalSection(&criticalSection);
}

int Cloth::CalculateIndex( int row, int col, int numRows )
{
	int index = (col * numRows) + row;
	if ( index >= m_MaxIndex || index < 0 )
	{
		int x = 0;
	}
	return index;
}

DWORD WINAPI SatisfyConstraints( PVOID pvParam)
{
	//while(true)
	//{
		particleSysRef->Verlet();
		particleSysRef->SatisfyConstraints();
	//}

	return 0;
}




/// <summary>
/// The m no of column
/// </summary>
void Cloth::Initialize()
{
	
}

void Cloth::InitCloth(int noofrows,int noofcolumns,int step)
{
	mNoOfRows = noofrows;
	mNoOfColumn = noofcolumns;
	m_ParticleCount = mNoOfRows * mNoOfColumn;
	mStep = step;

	if(mThisGO)
	{
		mMesh =  mThisGO->renderer->mMesh;
		m_ParticleCount = mMesh->mNoOfVertices;
	}
	m_InternalParticleSystem.SetParticleCount(m_ParticleCount);
	m_InternalParticleSystem.SetIterationsCount(50);
	mPointColliders = new XNA::Sphere[m_ParticleCount];
	VerletFunc();
	particleSysRef = &m_InternalParticleSystem;
	
	mNoOfColliders = m_ParticleCount;
}

/// <summary>
/// Updates this instance.
/// </summary>
void Cloth::Update()
{
	startSimulation = true;
	m_InternalParticleSystem.Verlet();
	m_InternalParticleSystem.SatisfyConstraints();

	//mTheradHandle = CreateThread(NULL,	
	//	0,
	//	SatisfyConstraints,
	//	&startSimulation,   //PASSING Elephant pointer as PARAMETER
	//	0,
	//	(DWORD*) &mThread);
	//WaitForSingleObject(mTheradHandle,INFINITE);

	if(mThisGO->renderer->mMesh != NULL)
	{
		for(int i = 0; i != m_ParticleCount; i++)
		{
			//CVector vec = m_InternalParticleSystem.getParticle(i);
			mThisGO->renderer->mCurrentVertices[i].m_Position.x = m_InternalParticleSystem.getParticle(i).x;
			mThisGO->renderer->mCurrentVertices[i].m_Position.y = m_InternalParticleSystem.getParticle(i).y;
			mThisGO->renderer->mCurrentVertices[i].m_Position.z = m_InternalParticleSystem.getParticle(i).z;

			mPointColliders[i].Center.x =  m_InternalParticleSystem.getParticle(i).x;
			mPointColliders[i].Center.y =  m_InternalParticleSystem.getParticle(i).y; 
			mPointColliders[i].Center.z =  m_InternalParticleSystem.getParticle(i).z; 

			
		}
		for(int i = 0; i != m_ParticleCount; i++)
		{
			if(XNA::IntersectSphereAxisAlignedBox(&mPointColliders[i],mCrateCollider))
			{
				m_InternalParticleSystem.getParticle( i ).y = mThisGO->renderer->mCurrentVertices[i].m_Position.y +10 ; //0.0f;
				m_InternalParticleSystem.getParticleOld( i ).y = mThisGO->renderer->mCurrentVertices[i].m_Position.y +10; //0.0f;
			}
		}
	}
}
/// <summary>
/// Destroys this instance.
/// </summary>
void Cloth::Destroy()
{

}

ComponentType Cloth::GetType()
{
	return ClothComp;
}
/// <summary>
/// Clones this instance.
/// </summary>
/// <returns>IBehaviour *.</returns>
IBehaviour* Cloth::Clone()
{
	return NULL;
}

void Cloth::VerletFunc()
{
	int m_MaxIndex = (mNoOfRows-1) * (mNoOfColumn - 1);
	
	float boxExtent = 50;
	m_InternalParticleSystem.startLocation = CVector( 0.0f, 25.0f, 0.0f );
	if(mMesh != NULL)
	{
		for (int i = 0; i < mMesh->mNoOfVertices;i++)
		{
			CVector position = CVector(mThisGO->renderer->mCurrentVertices[i].m_Position.x,
				mThisGO->renderer->mCurrentVertices[i].m_Position.y,
				mThisGO->renderer->mCurrentVertices[i].m_Position.z); 
			m_InternalParticleSystem.CreateParticleAt(position);

			mPointColliders[i].Center.x = position.x;
			mPointColliders[i].Center.y = position.y;
			mPointColliders[i].Center.z = position.z;

			mPointColliders[i].Radius = 0.5f;

		}
		for(int j = 0; j != mMesh->mNoOfElements;j++)
		{
			CConstraint tempConstraint;
			tempConstraint.particleA = mMesh->mElements[j].index_v1;
			tempConstraint.particleB = mMesh->mElements[j].index_v2;
			tempConstraint.restlength = mMesh->mElements[j].restlength1;
			m_InternalParticleSystem.AddConstraint(tempConstraint);

			CConstraint tempConstraint2;
			tempConstraint.particleA = mMesh->mElements[j].index_v2;
			tempConstraint.particleB = mMesh->mElements[j].index_v3;
			tempConstraint.restlength = mMesh->mElements[j].restlength2;
			m_InternalParticleSystem.AddConstraint(tempConstraint2);

			CConstraint tempConstraint3;
			tempConstraint.particleA = mMesh->mElements[j].index_v3;
			tempConstraint.particleB = mMesh->mElements[j].index_v1;
			tempConstraint.restlength = mMesh->mElements[j].restlength3;
			m_InternalParticleSystem.AddConstraint(tempConstraint3);
		}
	}
	else
	{
		/*m_InternalParticleSystem.CreateRandomParticles( CVector( -boxExtent, -boxExtent, -boxExtent ), 
			CVector( boxExtent, boxExtent, boxExtent ),
			mNoOfRows * mNoOfColumn );*/
	}
	
	m_InternalParticleSystem.vGravity = CVector( 0.0f, -1.0f, 0.0f );
	m_InternalParticleSystem.ApplyAccelerationToAllParticles( CVector( 0.0f, -1.0f, 0.0f ));


	//float step = mStep;

	//CConstraint tempConstraint;
	//tempConstraint.restlength = step;

	//for ( int row = 0; row != mNoOfRows; row++ )
	//{
	//	for ( int col = 0; col != mNoOfColumn; col++ )
	//	{
	//		int index = CalculateIndex( row, col, mNoOfRows );
	//		// No top constraint if at top...
	//		if ( row > 0 )
	//		{	// Current row & previous row
	//			tempConstraint.particleA = index;
	//			tempConstraint.particleB = CalculateIndex( row - 1, col, mNoOfRows ); 
	//			m_InternalParticleSystem.AddConstraint( tempConstraint );
	//		}
	//		// No bottom constraint if at bottom...
	//		if ( row < (mNoOfRows - 1) )
	//		{	// Current row & next row
	//			tempConstraint.particleA = index;
	//			tempConstraint.particleB = CalculateIndex( row + 1, col, mNoOfRows );
	//			m_InternalParticleSystem.AddConstraint( tempConstraint );
	//		}
	//		// No left constraint if at left...
	//		if ( col > 0 )
	//		{	// Current col & previous col
	//			tempConstraint.particleA = index;
	//			tempConstraint.particleB = CalculateIndex( row, col - 1, mNoOfRows );
	//			m_InternalParticleSystem.AddConstraint( tempConstraint );
	//		}
	//		// No right constraint if at right...
	//		if ( col < (col - 1) )
	//		{	// Current col & next col...
	//			tempConstraint.particleA = index;
	//			tempConstraint.particleB = CalculateIndex( row, col + 1, mNoOfRows );
	//			m_InternalParticleSystem.AddConstraint( tempConstraint );
	//		}
	//	}
	//}
}

Vector3 Cloth::GetParticleAt(int index)
{
	
	Vector3 temp;
	if(index < m_ParticleCount)
	{
		temp.x  = m_InternalParticleSystem.getParticle(index).x;
		temp.y  = m_InternalParticleSystem.getParticle(index).y;
		temp.z  = m_InternalParticleSystem.getParticle(index).z;
	}
	else
		temp = Vector3::Zero;
	return temp;
}

Vector3 Cloth::GetParticleAtOLD(int index)
{
	Vector3 temp;
	if(index < m_ParticleCount)
	{
		temp.x  = m_InternalParticleSystem.getParticleOld(index).x;
		temp.y  = m_InternalParticleSystem.getParticleOld(index).y;
		temp.z  = m_InternalParticleSystem.getParticleOld(index).z;
	}
	else
		temp = Vector3::Zero;
	return temp;
}

void Cloth::StiffnessFactor(int noOfIterations)
{
	m_InternalParticleSystem.numIterations = noOfIterations; 
}

void Cloth::ApplyAccelerationInDirection(Vector3 direction,float power)
{
	
	for (int index = 0; index < m_ParticleCount ; index++)
	{
		Vector3 dir = direction* Helper::RandomBetween0n1() * power;
		CVector tor;
		tor.x = dir.x;
		tor.y = dir.y;
		tor.z = dir.z;
		m_InternalParticleSystem.ApplyAccelerationToParticleAt(index,tor);
	}
}

