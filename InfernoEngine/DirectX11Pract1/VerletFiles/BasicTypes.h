#ifndef _BasicTypes_HG_
#define _BasicTypes_HG_

#include <stdlib.h>		// RAND_MAX

// ********************* some other helpful functions....
// Templated version: use by putting the type in the angle brackets
// 
// So this:
//		GLdouble getRand(GLdouble lowRange, GLdouble highRange);
//
// Would get called like this (you add they type in the angle brackets):
//
//		float x = getRand<float>( 0.0f, 10.0f );
//
template <class T>
T getRand(T lowRange, T highRange)
{
	if ( lowRange > highRange )
	{
		T temp = lowRange;
		lowRange = highRange;
		highRange = temp;
	}
	T delta = highRange - lowRange;
	delta = static_cast<T>( ( static_cast<double>( rand() ) / static_cast<double>(RAND_MAX) ) * delta );
	return delta + lowRange;
}

class CPoint
{
public:
	CPoint() : x(0.0f), y(0.0f) {};
	CPoint(float x, float y) : x(x), y(x) {};
	virtual ~CPoint() {};
	float x;
	float y;
};

class CPoint3f
{
public:
	CPoint3f() : x(0.0f), y(0.0f), z(0.0f) {};
	virtual ~CPoint3f() {};
	float x, y, z;
};

class CVector
{
public:
	CVector() : x(0.0f), y(0.0f), z(0.0f) {};
	CVector( float x, float y, float z );
	CVector( const CVector &v );	// Copy constructor
	virtual ~CVector() {};
	float x; float y; float z;

	// These are used on vector types
	CVector& operator=(const CVector &v);
	CVector& operator+=(const CVector &v);
	CVector& operator-=(const CVector &v);
	CVector& operator*=(float scalar);	// Times a scalar
	CVector& operator/=(float scalar);	// Divided by a scalar

	CVector operator+(const CVector &v);
	CVector operator-(const CVector &v);
	CVector operator*(float scalar);	
	CVector operator/(float scalar);	// Divided by a scalar
	CVector operator-();	// Negative on all directions
	
	// Helpful functions
	void Normalize(void);
	float Magnitude(void);
	float MagnitudeSquared(void);
	void Reverse(void);
	void Clear(void);
	
	// Helpful static functions...
	static void Normalize( float &x, float &y, float &z);
	static void Normalize( CVector &v );
	static float Magnitude( const CVector &v );
	static float MagnitudeSquared( const CVector &v );
	static float Distance( const CVector &v1, const CVector &v2);
	static float DistanceSquared( const CVector &v1, const CVector &v2);

	static void Subtract( const float &x0, const float &y0, const float &z0, 
                          const float &x1, const float &y1, const float &z1,
                          float &xSub, float &ySub, float &zSub);
	static void Subtract( const CVector &v1, const CVector &v2, CVector &vSub );
	static CVector Subtract( const CVector &v1, const CVector &v2 );
	static CVector ScalarMultiply( const CVector &v, float s);
	static CVector ScalarDivide( const CVector &v, float s);

	static void CrossProduct( const float &x0, const float &y0, const float &z0, 
	                          const float &x1, const float &y1, const float &z1,
	                          float &xCross, float &yCross, float &zCross);
	static void CrossProduct( const CVector v1, const CVector v2, CVector &vSub );

	static float DotProduct( const float &x1, const float &y1, const float &z1, 
	                         const float &x2, const float &y2, const float &z2 );
	static float DotProduct( const CVector &v1, const CVector &v2 );
};

class GDPVertex
{
public:
	GDPVertex(float x, float y, float z,
              float red, float green, float blue);
	virtual ~GDPVertex();
	float x;	float y;	float z;
	float red; float green; float blue;
private:

};

#endif