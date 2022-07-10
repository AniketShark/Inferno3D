#ifndef _CParticleSystem_HG_
#define _CParticleSystem_HG_

// Inspired by 'Advanced Character Physics" http://web.archive.org/web/20070610223835/http://www.teknikus.dk/tj/gdc2001.htm

#include "BasicTypes.h"
#include "CConstraint.h"
#include <windows.h>
#include <process.h>
#include <vector>

extern CRITICAL_SECTION criticalSection;

//const int NUM_PARTICLES = 200;
//
class CParticleSystem
{
public:
	CParticleSystem();
	CParticleSystem( CVector startLocation );
	//
	void CreateRandomParticles( CVector bboxmin, CVector bboxmax, unsigned int numParticles );
	
	void ApplyAccelerationToAllParticles( CVector gravity );
	void ApplyAccelerationToParticleAt(int index, CVector gravity );


	void Kerplode( float maxAcceleration, CVector direction );
	void RandomPerturb( CVector directionConstraint );

	CVector& getParticle( std::vector< CVector >::size_type index );
	CVector& getParticleOld( std::vector< CVector >::size_type index );
	CVector& getParticleAcceleration( std::vector< CVector >::size_type index );
	int getNumberOfParticles( void );

	// My methods
	void CreateParticleAt(CVector position);
	void SetParticleArraySize(int size);
	void SetParticleCount(int count);
	void SetIterationsCount(int count);
	void Reset();
	
	CVector vGravity; // Gravity
	float fTimeStep;
	int numIterations;

	CVector startLocation;
	void TimeStep( void );
	void Verlet( void );
	void SatisfyConstraints( void );

	void AddConstraint( CConstraint theConstraint );
	void ClearConstraints( void );
	
private:
	void AccumulateForces( void );
	void m_Reset( void );

	std::vector< CVector > m_vec_x;
	std::vector< CVector > m_vec_oldx;
	std::vector< CVector > m_vec_a;
	std::vector< CVector > m_vec_orig;


	std::vector< CConstraint > m_vecConstraints;

	unsigned int m_numParticles;
};

#endif

