#include "BasicTypes.h"

#include <math.h>	// sqrt, etc.

GDPVertex::GDPVertex(float x, float y, float z,
					 float red, float green, float blue)
{
	this->x = x; this->y = y; this->z = z;
	this->red = red; this->green = green; this->blue = blue;
	return;
}

GDPVertex::~GDPVertex()
{
	return;
}


// Fully defined in header...
//CVector::CVector() : x(0.0f), y(0.0f), z(0.0f) {};

CVector::CVector( float x, float y, float z )
{
	this->x = x; this->y = y; this->z = z;
}

// Copy constructor
CVector::CVector( const CVector &v )
{
	this->x = v.x; this->y = v.y; this->z = v.z;
}

// Fully defined in header...
//CVector::~CVector() {};

CVector& CVector::operator=(const CVector &v)
{
	// Check for self-assignment *IMPORTANT*
	if ( this == &v )	return *this;
	// Else...copy values
	this->x = v.x; this->y = v.y; this->z = v.z;
	return *this;
}

CVector CVector::operator+(const CVector &v)
{
	return ( CVector( *this ) += v );
}

CVector CVector::operator-(const CVector &v)
{
	return ( CVector( *this ) -= v );
}

CVector& CVector::operator+=(const CVector &v)
{
	this->x += v.x; this->y += v.y; this->z += v.z;
	return *this;
}

CVector& CVector::operator-=(const CVector &v)
{
	this->x -= v.x; this->y -= v.y; this->z -= v.z;
	return *this;
}

CVector CVector::operator*(float scalar)
{
	CVector temp( *this );
	temp *= scalar;
	return temp;
}


// Times a scalar
CVector& CVector::operator*=(float scalar)
{
	this->x *= scalar; this->y *= scalar; this->z *= scalar;
	return *this;
}

// Divided by a scalar
CVector& CVector::operator/=(float scalar)
{
	this->x /= scalar; this->y /= scalar; this->z /= scalar;
	return *this;
}

CVector CVector::operator/(float scalar)
{
	CVector temp( *this );
	temp /= scalar;
	return temp;
}

	
CVector CVector::operator-()	// Negative on all directions
{
	return CVector( -x, -y, -z );
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void CVector::Subtract(const float &x0, const float &y0, const float &z0, 
                       const float &x1, const float &y1, const float &z1,
                       float &xSub, float &ySub, float &zSub)
{
	xSub = x0 - x1; ySub = y0 - y1; zSub = z0 - z1;
}

/*static*/ // Subtracts one vector from another and returns in Sub (by reference)
void CVector::Subtract( const CVector &v1, const CVector &v2, CVector &vSub )
{
	CVector::Subtract( v1.x, v1.y, v1.z, 
		               v2.x, v2.y, v2.z, 
					   vSub.x, vSub.y, vSub.z );
}

/*static*/ // Subtracts returns one vector from another 
CVector CVector::Subtract( const CVector &v1, const CVector &v2 )
{
	CVector vTemp;
	CVector::Subtract( v1, v2, vTemp );
	return vTemp;
}

/*static*/
CVector CVector::ScalarMultiply( const CVector &v, float s)
{
	CVector vTemp(v);
	vTemp *= s;
	return vTemp;
}

/*static*/
CVector CVector::ScalarDivide( const CVector &v, float s)
{
	CVector vTemp(v);
	vTemp /= s;
	return vTemp;
}

/*static*/
// Takes cross of vec0 (x0, y0, z0) and vec1 (x1, y1, z1) and 
//	returns cross (xCross, yCross, zCross)
void CVector::CrossProduct( const float &x0, const float &y0, const float &z0, 
                            const float &x1, const float &y1, const float &z1,
                            float &xCross, float &yCross, float &zCross)
{
    /* find cross-product between these vectors */
    xCross = y0 * z1 - z0 * y1;
    yCross = z0 * x1 - x0 * z1;
    zCross = x0 * y1 - y0 * x1;
}

/*static*/
void CVector::CrossProduct( const CVector v1, const CVector v2, CVector &vSub )
{
	CVector::CrossProduct( v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, vSub.x, vSub.y, vSub.z );
}

/*static*/
float CVector::DotProduct( const float &x1, const float &y1, const float &z1, 
                           const float &x2, const float &y2, const float &z2 )
{
	float dot = x1 * x2 + y1 * y2 + z1 * z2;
	return dot;
}

/*static*/
float CVector::DotProduct( const CVector &v1, const CVector &v2 )
{
	return DotProduct( v1.x, v1.y, v1.z, v2.x, v2.y, v2.z );
}

/*static*/	
// Set vector to unit size (length of 1.0)
void CVector::Normalize( float &x, float &y, float &z)
{
	// Convert to double so there is no loss...
	double x2 = static_cast<double>(x) * static_cast<double>(x);
	double y2 = static_cast<double>(y) * static_cast<double>(y);
	double z2 = static_cast<double>(z) * static_cast<double>(z);
	double length = x2 + y2 + z2;
	if ( length == 0 )
	{
		x = y = z = 0.0f;
	} 
	else
	{	// Multiply by reciprical as multiplies are faster than divides (usually)
		double reciprical = 1 / sqrt( length );
		x = static_cast<float>( static_cast<double>(x) * reciprical   );
		y = static_cast<float>( static_cast<double>(y) * reciprical  );
		z = static_cast<float>( static_cast<double>(z) * reciprical  );
	}
}

/*static*/	
void CVector::Normalize( CVector &v )
{
	CVector::Normalize( v.x, v.y, v.z );
}

void CVector::Normalize(void)
{
	CVector::Normalize( *this );
}

/*static*/
float CVector::Magnitude( const CVector &v )
{
	return static_cast<float>(sqrt( v.x*v.x + v.y*v.y + v.z*v.z ));
}

float CVector::Magnitude( void )
{
	return CVector::Magnitude( *this );
}
/*static*/
// Same as magnatude, but without the square root...
float CVector::MagnitudeSquared( const CVector &v )
{
	return static_cast<float>( v.x*v.x + v.y*v.y + v.z*v.z );

}

float CVector::MagnitudeSquared( void )
{
	return CVector::Magnitude( *this );
}

/*static*/
float CVector::Distance( const CVector &v1, const CVector &v2)
{
	double deltaX = static_cast<double>( v1.x - v2.x );
	double deltaY = static_cast<double>( v1.y - v2.y );
	double deltaZ = static_cast<double>( v1.z - v2.z );
	return static_cast<float>(sqrt( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ ));
}

/*static*/
// Same as distance, but without the square root...
float CVector::DistanceSquared( const CVector &v1, const CVector &v2)
{
	double deltaX = static_cast<double>( v1.x - v2.x );
	double deltaY = static_cast<double>( v1.y - v2.y );
	double deltaZ = static_cast<double>( v1.z - v2.z );
	return static_cast<float>( deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ );
}

void CVector::Reverse(void)
{
	this->x = -(this->x);
	this->y = -(this->y);
	this->z = -(this->z);
}

void CVector::Clear(void)
{
	this->x = this->y = this->z = 0.0f;
}
