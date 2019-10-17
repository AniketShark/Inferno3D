#include "CRigidBody.h"
#include <algorithm>
#include <math.h>

static const float FLOAT_ERROR_MARGIN = 0.0001f;

bool bIsFloatZero( float value )
{
	if ( value < FLOAT_ERROR_MARGIN )		
	{
		if ( value > -FLOAT_ERROR_MARGIN )	
		{
			return true;
		}
	}
	return false;
}

bool bIsVectorZero( const CVector &vec )
{
	if ( bIsFloatZero( vec.x ) && bIsFloatZero( vec.y) && bIsFloatZero( vec.z ) )
	{
		return true;
	}
	return false;
}

namespace ILOVEPHYSICS
{
	float absFloat( float x )
	{
		if ( x < 0.0f )	return -x;
		return x;
	}
}

float calculateDamping( const CVector &vel )
{
	float MaxValue = ILOVEPHYSICS::absFloat( vel.x );
	if ( ILOVEPHYSICS::absFloat( vel.y ) > MaxValue )		MaxValue = ILOVEPHYSICS::absFloat( vel.y );
	if ( ILOVEPHYSICS::absFloat( vel.z ) > MaxValue )		MaxValue = ILOVEPHYSICS::absFloat( vel.z );

	// MaxValue is largest velocity in x or y or z coords....
	//	...approx related to "real" velocity (which needs sqrt)
	
	if ( MaxValue >= 1.0f )
	{
		// Making this less than 1.0, like 0.5, reduces damping
		// Making it greater than 1.0, like 6.0, increases...
		float RnRDampingFactor = 1.0f;
		float HundredLesslogVel = 100.0f - ( log( MaxValue ) * RnRDampingFactor );
		float damping = HundredLesslogVel / 100.0f;	// 1.0 to little less than 1.0
		
		return damping;
	}
	return 1.0f;	// Don't damp (if vel is 1.0 or less)
}


CRigidBody::CRigidBody()
{
	this->m_mass = 0.0f;
	this->m_inverseMass = 0.0f;
	this->m_IntegrationMode = EULER;
	//
	this->m_ID = this->m_NextID++;
}

CRigidBody::CRigidBody( const CRigidBody &rhs )	// Copy constructor
{
	this->name = rhs.name;
	this->location = rhs.location;
	this->orientation = rhs.orientation;
	this->linearVelocity = rhs.linearVelocity; 
	this->angularVelocity = rhs.angularVelocity;
	this->linearAcceleration = rhs.linearAcceleration;
	this->angularAcceleration = rhs.angularAcceleration;
	this->m_IntegrationMode = rhs.m_IntegrationMode;
	this->m_vecForces = rhs.m_vecForces;
	this->m_vecImpulses = rhs.m_vecImpulses;
	this->m_ResultantForce = rhs.m_ResultantForce;
	this->m_mass = rhs.m_mass;
	this->m_inverseMass = rhs.m_inverseMass;
	this->m_ID = rhs.m_ID;
}

CRigidBody& CRigidBody::operator=(const CRigidBody &rhs)
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &rhs )	return *this;
	// Else...copy values
	this->name = rhs.name;
	this->location = rhs.location;
	this->orientation = rhs.orientation;
	this->linearVelocity = rhs.linearVelocity; 
	this->angularVelocity = rhs.angularVelocity;
	this->linearAcceleration = rhs.linearAcceleration;
	this->angularAcceleration = rhs.angularAcceleration;
	this->m_IntegrationMode = rhs.m_IntegrationMode;
	this->m_vecForces = rhs.m_vecForces;
	this->m_vecImpulses = rhs.m_vecImpulses;
	this->m_ResultantForce = rhs.m_ResultantForce;
	this->m_mass = rhs.m_mass;
	this->m_inverseMass = rhs.m_inverseMass;
	this->m_ID = rhs.m_ID;

	return *this;
}

CRigidBody::~CRigidBody() {}

//static 
unsigned int CRigidBody::m_NextID = 1;		// NOTE: There is no "0" game object - start at 1 (0 means there's a problem)

unsigned int CRigidBody::GetID(void)		// ID is created when object is created
{
	return this->m_ID;
}
// 
void CRigidBody::Update( float dt )
{
	// Determine the resultant force...
	this->m_ResultantForce.Clear();

	// Add all the constant forces...
	for ( std::vector< CVector >::iterator itForce = this->m_vecForces.begin(); 
		  itForce != this->m_vecForces.end(); itForce++ )
	{
		this->m_ResultantForce += (*itForce);
	}
	// Now the impulses...
	for ( std::vector< CVector >::iterator itImpulse = this->m_vecImpulses.begin();
		  itImpulse != this->m_vecImpulses.end(); itImpulse++ )
	{
		this->m_ResultantForce += (*itImpulse);
	}
	// Clear the impluse vector (they are only applied once)
	this->m_vecImpulses.clear();

	// INTEGRATE...

	// If there are no forces or no mass, skip update...
	if (bIsVectorZero( this->m_ResultantForce ) || ( bIsFloatZero(this->m_mass) ) )
	{
		return;
	}

	// **************************************************************************
	// ** At this point, there is some force (i.e. not zero force or zero mass **
	// **************************************************************************

	// Calculate acceleration...
	this->linearAcceleration = this->m_ResultantForce / this->m_mass;

	// Perform integration...
	if ( this->m_IntegrationMode == EULER )
	{
		this->location += ( this->linearVelocity * dt );
		this->linearVelocity += ( this->linearAcceleration * dt );
	}
	else if ( this->m_IntegrationMode == RK4 )
	{
	    //mTranslate += mVelocity*dt;
	    //mVelocity += accel*dt;
		CVector xk1 = this->linearVelocity * dt;			// Delta location
		CVector vk1 = this->linearAcceleration * dt;		// DeltaV
	
		CVector midVelocity = this->linearVelocity + ( vk1 * 0.5f );	// V + ( DeltaV/2 )
		CVector xk2 = this->linearVelocity * dt;									
		CVector vk2 = this->linearAcceleration * dt;	/*( dt * 0.5f );*/

		midVelocity = this->linearVelocity + ( vk2 * 0.5f);		// V + (( DeltaV/2 ) / 2 )
		CVector xk3 = midVelocity * dt;
		CVector vk3 = this->linearAcceleration * dt;	/*( dt * 0.5f );*/

		midVelocity = this->linearVelocity + vk3;
		CVector xk4 = midVelocity * dt;
		CVector vk4 = this->linearAcceleration * dt;

		this->location += ( xk1 + (xk2 * 2.0f) + (xk3 * 2.0f) + xk4 ) / 6.0f;
		this->linearVelocity += ( vk1 + (vk2 * 2.0f) + (vk3 * 2.0f) + vk4 ) / 6.0f;
	}
	//
	
	this->linearVelocity *= calculateDamping( this->linearVelocity );

	//this->linearVelocity = this->linearVelocity * 0.99f;
}

void CRigidBody::SetMass( float newMass )
{
	this->m_mass = newMass;
	this->m_inverseMass = 1.0f / this->m_mass;
}

float CRigidBody::GetMass( void )
{
	return this->m_mass;
}

void CRigidBody::SetIntegrationMode( enumIntegrationMode newIntegrationMode )
{
	this->m_IntegrationMode = newIntegrationMode;
}

void CRigidBody::ApplyForce( CVector vecForce )
{
	this->ClearForces();
	this->m_vecForces.push_back( vecForce );
}

void CRigidBody::AddForce( CVector vecForce )
{
	this->m_vecForces.push_back( vecForce );
}

void CRigidBody::AddImpulse( CVector vecForce )
{
	this->m_vecImpulses.push_back( vecForce );
}

void CRigidBody::ClearForces( void )
{
	this->m_vecForces.clear();
}

