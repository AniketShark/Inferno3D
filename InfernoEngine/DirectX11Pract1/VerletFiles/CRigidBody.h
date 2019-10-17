#ifndef _CRigidBody_HG_
#define _CRigidBody_HG_

#include "BasicTypes.h"		// CVector
#include <vector>
#include <string>

enum enumIntegrationMode
{
	EULER,
	RK4
};

class CRigidBody
{
public:
	CRigidBody();
	virtual ~CRigidBody();
	CRigidBody( const CRigidBody &rhs );	// Copy constructor
	CRigidBody& operator=(const CRigidBody &rhs);

	unsigned int GetID(void);		// ID is created when object is created
	std::string name;
	// 
	void ApplyForce( CVector vecForce );
	void AddForce( CVector vecForce );
	void AddImpulse( CVector vecForce );
	void ClearForces( void );
	// 
	void Update( float dt );
	void SetIntegrationMode( enumIntegrationMode newIntegrationMode );
	//
	CVector location;		// current position
	CVector orientation;	// angle
	// velocities
	CVector linearVelocity;
	CVector angularVelocity;
	// acceleration
	CVector linearAcceleration;
	CVector angularAcceleration;
	// Momentum
	//CVector linearMomentum;
	//CVector angularMementum;
	//void ApplyTorque( CVector vecTorque );		// Around all three...
	//void ApplyTorqueXAxis( float torque );		//	...or just one
	//void ApplyTorqueYAxis( float torque );
	//void ApplyTorqueZAxis( float torque );
	// mass
	void SetMass( float newMass );
	float GetMass( void );
	//
private:
	enumIntegrationMode m_IntegrationMode;
	std::vector< CVector > m_vecForces;
	std::vector< CVector > m_vecImpulses;
	CVector m_ResultantForce;

	float m_mass;
	float m_inverseMass;

	unsigned int m_ID;
	static unsigned int m_NextID;
};

#endif