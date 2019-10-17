#include "CConstraint.h"

CConstraint::CConstraint()
{
	this->particleA = this->particleB = 0;
	this->restlength = 0.0f;
}

CConstraint::CConstraint( const CConstraint &rhs )	// Copy constructor
{
	this->particleA = rhs.particleA;
	this->particleB = rhs.particleB;
	this->restlength = rhs.restlength;
}

CConstraint& CConstraint::operator=(const CConstraint &rhs)
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &rhs )	return *this;
	// Else...copy values
	this->particleA = rhs.particleA;
	this->particleB = rhs.particleB;
	this->restlength = rhs.restlength;

	return *this;
}
