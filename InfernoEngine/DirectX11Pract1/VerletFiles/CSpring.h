#ifndef _CSpring_HG_
#define _CSpring_HG_

#include "BasicTypes.h"		// CVector
#include <string>

class CSpring
{
public:
	CSpring();
	CSpring( CVector start, CVector end, float HookesConstant );	// Also calls CalculateRestingLengthFromCurrent()
	CSpring( const CSpring &rhs );	// Copy constructor
	CSpring& operator=(const CSpring &rhs);

	unsigned int GetID(void);		// ID is created when object is created
	std::string name;

	float HookesConstant;
	float restingLength;
	CVector start;
	CVector end;

	void CalculateRestingLengthFromCurrent( void );
	float GetCurrentLength( void );			// Returns scalar length Distance(end - start)
	float GetStretchDelta( void );			// Current length - original length
	CVector CalculateCurrentForce( void );	// Returns a force based on delta length vs resting lenght
private:
	unsigned int m_ID;
	static unsigned int m_NextID;
};

#endif