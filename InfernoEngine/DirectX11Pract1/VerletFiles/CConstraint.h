#ifndef _CConstraint_HG_
#define _CConstraint_HG_

class CConstraint
{
public:
	CConstraint();
	CConstraint( const CConstraint &rhs );	// Copy constructor
	CConstraint& operator=(const CConstraint &rhs);

	int particleA, particleB;
	float restlength;
};

#endif