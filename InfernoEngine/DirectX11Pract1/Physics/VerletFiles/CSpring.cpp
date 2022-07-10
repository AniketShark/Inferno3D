#include "CSpring.h"

CSpring::CSpring() 
{
	this->HookesConstant = 0.0f;
	this->restingLength = 0.0f;
	//
	this->m_ID = this->m_NextID++;
}

CSpring::CSpring( CVector start, CVector end, float HookesConstant )	// Also calls CalculateRestingLengthFromCurrent()
{
	this->start = start;
	this->end = end;
	this->HookesConstant = HookesConstant;
	//
	this->m_ID = this->m_NextID++;
	//
	this->CalculateRestingLengthFromCurrent();
}


CSpring::CSpring( const CSpring &rhs )	// Copy constructor
{
	this->name = rhs.name;
	this->HookesConstant  = rhs.HookesConstant;
	this->restingLength = rhs.restingLength;
	this->start = rhs.start;
	this->end = rhs.end;
	this->m_ID = rhs.m_ID;
}

CSpring& CSpring::operator=(const CSpring &rhs)
{
		// Check for self-assignment *IMPORTANT*
	if ( this == &rhs )	return *this;
	// Else...copy values
	this->name = rhs.name;
	this->HookesConstant  = rhs.HookesConstant;
	this->restingLength = rhs.restingLength;
	this->start = rhs.start;
	this->end = rhs.end;
	this->m_ID = rhs.m_ID;

	return *this;
}

unsigned int CSpring::m_NextID = 1;		// NOTE: There is no "0" game object - start at 1 (0 means there's a problem)

unsigned int CSpring::GetID(void)		// ID is created when object is created
{
	return this->m_ID;
}

// Calculates the resting length from the current end locations.
void CSpring::CalculateRestingLengthFromCurrent( void )
{
	this->restingLength = this->GetCurrentLength();
}

float CSpring::GetCurrentLength( void )
{
	return CVector::Distance( this->start, this->end );
}

CVector CSpring::CalculateCurrentForce( void ) // Returns a force based on deltalenght vs resting lenght
{
	// F = -kx
	// (x is different from resting length)
	CVector direction = end - start;
	direction.Normalize();
	float deltaX = this->GetStretchDelta();
	CVector force = direction * ( deltaX * -(this->HookesConstant) ) ;
	return force;
}


float CSpring::GetStretchDelta( void )			// Current length - original length
{
	float delta = this->GetCurrentLength() - this->restingLength;
	return delta;
}
	
