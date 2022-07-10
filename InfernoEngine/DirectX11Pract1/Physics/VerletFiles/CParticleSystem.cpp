#include "CParticleSystem.h"
#include <math.h>

CRITICAL_SECTION criticalSection;


CParticleSystem::CParticleSystem()
{
	m_numParticles = 0;
	this->m_Reset();
}

void CParticleSystem::m_Reset( void )
{
	this->fTimeStep = 0.1f;
	this->vGravity = CVector( 0.0f, 0.0f, 0.0f );
	this->numIterations = 5;

	this->m_vec_x.clear();
	this->m_vec_oldx.clear();
	this->m_vec_a.clear();
	this->m_vec_orig.clear();
	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		this->m_vec_x.push_back(CVector( 0.0f, 0.0f, 0.0f )); // Current positions
		this->m_vec_oldx.push_back(CVector( 0.0f, 0.0f, 0.0f )); // Previous positions
		this->m_vec_a.push_back(CVector( 0.0f, 0.0f, 0.0f )); // Force accumulators
		this->m_vec_orig.push_back(CVector( 0.0f, 0.0f, 0.0f ));
	}
	InitializeCriticalSection(&criticalSection);
}

void CParticleSystem::Kerplode( float maxAcceleration, CVector direction )
{
	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		this->m_vec_a[ index ] = this->m_vec_x[ index ] - this->startLocation;
		this->m_vec_a[ index ].Normalize();
		this->m_vec_a[ index ] *= getRand< float >( 0.0f, maxAcceleration );

		this->m_vec_a[ index ].x *= direction.x;
		this->m_vec_a[ index ].y *= direction.y;
		this->m_vec_a[ index ].z *= direction.z;
		// 
		// Add gravity back...
		this->m_vec_a[ index ].y -= 1.0f;
	}
}

void CParticleSystem::RandomPerturb( CVector directionConstraint )
{
	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		this->m_vec_x[ index ].x += getRand<float>( -directionConstraint.x, directionConstraint.x );
		this->m_vec_x[ index ].y += getRand<float>( -directionConstraint.y, directionConstraint.y );
		this->m_vec_x[ index ].z += getRand<float>( -directionConstraint.z, directionConstraint.z );
	}
}

void CParticleSystem::SatisfyConstraints( void )
{
	for ( int itCount = 0; itCount != this->numIterations; itCount++ )
	{
		int numConstraints = this->m_vecConstraints.size();
		for ( int index = 0; index != numConstraints; index++ )
		{

			// Apply constraints...
			if ( index > static_cast<int>( this->m_vecConstraints.size() ) )
			{
				int x = 0;
			}
			CConstraint& c = this->m_vecConstraints[ index ];
			// 
			unsigned int indexA = c.particleA;
			unsigned int indexB = c.particleB;

			if ( indexA > this->m_numParticles )
			{
				int stop = 0;
			}
			if ( indexB > this->m_numParticles )
			{
				int stop = 0;
			}

			CVector& x1 =  this->m_vec_x[ indexA ];
			CVector& x2 =  this->m_vec_x[ indexB ];
			CVector delta = x2 - x1;
			float deltaMag = delta.Magnitude();
			float deltalength = sqrt( deltaMag * deltaMag );
			float diff=( deltalength - c.restlength ) / deltalength;
			x1 += delta * 0.5 * diff;
			x2 -= delta * 0.5 * diff;
		}
	}
	// Clamp the corners of the cloth...
	//for (int i = 0;i < 10;i++)
	//{
	//	this->m_vec_x[ i ] = CVector( m_vec_orig[i].x, m_vec_orig[i].y, 0.0f );
	//}
	
	//this->m_vec_x[ 9 ] = CVector( +0.0f, this->startLocation.y, 0.0f );
}

void CParticleSystem::AddConstraint( CConstraint theConstraint )
{
	this->m_vecConstraints.push_back( theConstraint );
}

void CParticleSystem::ClearConstraints( void )
{
	this->m_vecConstraints.clear();
}

void CParticleSystem::SetParticleArraySize(int size)
{
	this->m_vec_x.reserve( size );
	this->m_vec_oldx.reserve( size );
	this->m_vec_a.reserve( size );
}

void CParticleSystem::SetParticleCount(int count)
{
	m_numParticles = count;
}

void CParticleSystem::SetIterationsCount(int count)
{
	numIterations = count;
}

void CParticleSystem::Reset()
{
	m_Reset();
}

void CParticleSystem::CreateParticleAt(CVector position)
{
	CVector vecZero;
	CVector vectemp = CVector(position.x,position.y,position.z);
	this->m_vec_x.push_back( vectemp );
	this->m_vec_oldx.push_back( vectemp );
	this->m_vec_a.push_back( vecZero );
	this->m_vec_orig.push_back(vectemp);
}

CParticleSystem::CParticleSystem( CVector startLocation )
{
	this->m_Reset();
	this->startLocation = startLocation;
}

//void CParticleSystem::CreateRandomParticles(  CVector bboxmin, CVector bboxmax  )
//{
//	for ( int index = 0; index != NUM_PARTICLES; index++ )
//	{
//		this->m_x[ index ] = CVector( getRand<float>( bboxmin.x, bboxmax.x ) + this->startLocation.x,
//			                          getRand<float>( bboxmin.y, bboxmax.y ) + this->startLocation.y,
//							          getRand<float>( bboxmin.z, bboxmax.z ) + this->startLocation.z );
//
//		this->m_oldx[ index ] = this->m_x[ index ];
//		//this->m_oldx[ index ] = this->startLocation;
//	}
//}


void CParticleSystem::ApplyAccelerationToAllParticles( CVector gravity )
{
	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		this->m_vec_a[ index ] = gravity;
	}
}

void  CParticleSystem::ApplyAccelerationToParticleAt(int index, CVector gravity )
{
	if(index >= m_numParticles)
		return;
	this->m_vec_a[ index ] = gravity;
}

void CParticleSystem::CreateRandomParticles( CVector bboxmin, CVector bboxmax, unsigned int numParticles )
{
	this->m_numParticles = numParticles;

	this->m_vec_x.reserve( this->m_numParticles );
	this->m_vec_oldx.reserve( this->m_numParticles );
	this->m_vec_a.reserve( this->m_numParticles );

	CVector vectemp;
	CVector vecZero;

	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		vectemp = CVector( getRand<float>( bboxmin.x, bboxmax.x ) + this->startLocation.x,
			               getRand<float>( bboxmin.y, bboxmax.y ) + this->startLocation.y,
						   getRand<float>( bboxmin.z, bboxmax.z ) + this->startLocation.z );

		this->m_vec_x.push_back( vectemp );
		this->m_vec_oldx.push_back( vectemp );
		this->m_vec_a.push_back( vecZero );
		this->m_vec_orig.push_back(vectemp);
		//this->m_vec_x[ index ] = vectemp;
		//this->m_vec_oldx[ index ] = this->m_vec_x[ index ];
	}
}

// Verlet integration step
void CParticleSystem::Verlet( void ) 
{
	for ( int index = 0; index != this->m_numParticles; index++ )
	{
		CVector& x = this->m_vec_x[ index ];
		CVector temp = x;
		CVector& oldx = this->m_vec_oldx[ index ];
		CVector& a = this->m_vec_a[ index ];
		x += x - oldx + ( a * fTimeStep * fTimeStep );
		oldx = temp;
	}
}

int CParticleSystem::getNumberOfParticles( void )
{
	return static_cast< int >( this->m_vec_x.size() );
}

CVector& CParticleSystem::getParticle( std::vector< CVector >::size_type index )
{
	//EnterCriticalSection(&criticalSection);
	return this->m_vec_x.at( index );
	//LeaveCriticalSection(&criticalSection);
}

CVector& CParticleSystem::getParticleOld( std::vector< CVector >::size_type index )
{
	return this->m_vec_oldx.at( index );
}

CVector& CParticleSystem::getParticleAcceleration( std::vector< CVector >::size_type index )
{
	return this->m_vec_a.at( index );
}

