#include "Physics.h"
//#include "Shapes.h"
#include <math.h>

const float g_GRAVITY = 9.81f;
const float g_SLOWDOWN = 1.0f;	//******************************************

float absFloat( float value )
{
	if ( value < 0 ) 
		return -value;
	return value;
}

// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
void BarycentricCoordinates( const CPoint &a, const CPoint &b, const CPoint &c, 
							 const CPoint &p, float &u, float &v, float &w)
{
	// These call the static functions within CVector...
	CVector v0 = CVector::Subtract( CVector( b.x, b.y, 0 ), CVector( a.x, a.y, 0 ) );
	CVector v1 = CVector::Subtract( CVector( c.x, c.y, 0 ), CVector( a.x, a.y, 0 ) );
	CVector v2 = CVector::Subtract( CVector( p.x, p.y, 0 ), CVector( a.x, a.y, 0 ) );
	float d00 = CVector::DotProduct(v0, v0);
    float d01 = CVector::DotProduct(v0, v1);
    float d11 = CVector::DotProduct(v1, v1);
    float d20 = CVector::DotProduct(v2, v0);
    float d21 = CVector::DotProduct(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
} 

// Test if point p is contained in triangle (a, b, c)
// a, b, c --> triangle coordinates
// p --> point to test
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
bool TestPointTriangle( const CPoint &p, const CPoint &a, const CPoint &b, const CPoint &c)
{
    float u, v, w;
    BarycentricCoordinates( a, b, c, p, u, v, w );
	if ( ( v >= 0.0f ) && ( w >= 0.0f ) && ( (v + w) <= 1.0f ) )
	{
		return true;
	}
	return false;
}

//// Returns true if a) not same ball, b) are close, c) are moving towards each other
//bool testBallBallCollision( std::vector<CBall>::iterator itB1, std::vector<CBall>::iterator itB2 ) 
//{	// See if this is the 'same' ball
//	if ( itB1 != itB2 )
//	{	
//		CVector B1Loc = CVector( itB1->location.x, itB1->location.y, 0.0f );
//		CVector B2Loc = CVector( itB2->location.x, itB2->location.y, 0.0f );
//
//		float ballDistance = CVector::Distance( B1Loc, B2Loc );
//		float sumRadii = itB1->radius + itB2->radius;
//		// Are they close (distance <= radii )?		
//		if ( ballDistance <= sumRadii )
//		{	// They are in contact 
//
//			//std::cout << "B1Loc: "; B1Loc.DebugPrint();
//			//std::cout << "B2Loc: "; B2Loc.DebugPrint();
//			//std::cout << "\t Distance = " << ballDistance << std::endl;
//
//
//			// Are they moving towards each other?
//			CVector netVelocity = itB1->velocity - itB2->velocity;
//			CVector displacement = B1Loc - B2Loc;
//			//
//			float netVelocityDot = netVelocity.Dot( displacement );
//			//std::cout << "\t dot(netVelocity) = " << netVelocityDot << std::endl;
//			if ( netVelocityDot < 0 )
//			{
//				return true;
//			}
//			// Else, they are moving away...
//		} // if ( ballDistance <= g_CONTACT_THRESH )
//	} // if ( itBallOut != itBallIn )
//	return false;
//}

//// NOTE: This assumes that these two balls have collided...
//// Modified from http://www.gamasutra.com/view/feature/3015/pool_hall_lessons_fast_accurate_.php?print=1
//void handleBallBallCollision( std::vector<CBall>::iterator itB1, std::vector<CBall>::iterator itB2 ) 
//{	// See if this is the 'same' ball
//	if ( itB1 != itB2 )
//	{
//		CVector B1Loc = CVector( itB1->location.x, itB1->location.y, 0.0f );
//		CVector B2Loc = CVector( itB2->location.x, itB2->location.y, 0.0f );
//
//		CVector n = B2Loc - B1Loc;
//		n.Normalize();
//
//		float a1 = itB1->velocity.Dot( n );
//		float a2 = itB2->velocity.Dot( n );
//
//		float optimizedP = a1 - a2;
//
//		CVector v1new = itB1->velocity - ( n * optimizedP );
//		CVector v2new = itB2->velocity + ( n * optimizedP );
//
//		itB1->velocity = v1new;
//		itB2->velocity = v2new;
//
//	}//if ( itB1 != itB2 )
//}	

// Tests what the closest point to a triangle is. Used for the Sphere-Triangle intersection test
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
CPoint ClosestPointToTriangle( const CPoint &point, 
							   const CPoint &triA, const CPoint &triB, const CPoint &triC )
{
	CVector tA( triA.x, triA.y, 0.0f );
	CVector tB( triB.x, triB.y, 0.0f );
	CVector tC( triC.x, triC.y, 0.0f );
	CVector p( point.x, point.y, 0.0f );

	CVector ab = tB - tA;
	CVector ac = tC - tA;
	CVector ap = p - tA;

	float d1 = CVector::DotProduct( ab, ap );
	float d2 = CVector::DotProduct( ac, ap );

	// Test for barycentric coordinates (1, 0, 0)
	if ( (d1 <= 0.f) && (d2 <= 0.0f) ) return CPoint( tA.x, tA.y );

	// Check if p is vertex region outside B
	CVector bp = p - tB;
	float d3 = CVector::DotProduct( ab, bp );
	float d4 = CVector::DotProduct( ac, bp );

	// Test for barycentric coordinates (0, 1, 0)
	if ( (d3 >= 0.0f) && (d4 <= d3) )	return CPoint( tB.x, tB.y );

	// Check if P is edge region of AB. If so, return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if ( vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f )
	{
		float v = d1 / (d1 - d3);
		CVector result = tA + ( ab * v );	// barycentric coordinates (1-v,v, 0)
		return CPoint( result.x, result.y );
	}

	// Check if P in vertex region outside C
	CVector cp = p - tC;
	float d5 = CVector::DotProduct( ab, cp );
	float d6 = CVector::DotProduct( ac, cp );
	if ( (d6 >= 0.0f) && (d5 <= d6) ) return CPoint( tC.x, tC.y );	// barycentri coordinates (0,0,1)

	// Check if P is edge region of AC. If so, return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if ( (vb <= 0.0f) && (d2 >= 0.0f) && (d6 <= 0.0f) )
	{
		float w = d2 / (d2 - d6);
		CVector result = tA + ( ac * w );	// barycentric coordinates (1-w,0,w)
		return CPoint( result.x, result.y );
	}

	// Check if P in edge region of BC. If so, return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if ( va <= 0.0f && (d4-d3) >= 0.0f && (d5-d6) >= 0.0f )
	{
		float w = (d4-d3) / ( (d4-d3) + (d5-d6) );
		CVector result = tB + ( ( tC - tB) * w );	// barycentric coordinates (0,1-w,w)
		return CPoint( result.x, result.y );
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / ( va + vb + vc );
	float v = vb * denom;
	float w = vc * denom;
	// = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
	CVector result = tA + ( ab * v ) + ( ac * w );	
	return CPoint( result.x, result.y );
}

//// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
//bool TestSphereTriangle( const CBall &s, 
//					    const CPoint &triA, const CPoint &triB, const CPoint &triC, 
//						CPoint &pClosest)
//{
//	// Find point P on triangle ABC closest to sphere centre
//	pClosest = ClosestPointToTriangle( s.location, triA, triB, triC );
//
//	// Sphere and triangle intersect if the (squared) distance from sphere
//	//	centre to point p is less than the (squared) sphere radius...
//	CVector v = CVector( pClosest.x, pClosest.y, 0.0f) - CVector( s.location.x, s.location.y, 0.0f );
//	if ( CVector::DotProduct( v, v ) <= ( s.radius * s.radius ) )
//	{
//		return true;
//	}
//	return false;
//}

// Given a point, returns the closest point on a line segment
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
void ClosestPointPointSegment( const CPoint &pTest, 
							   const CPoint &lineA, const CPoint &lineB, 
							   float &t, CPoint &pClosest )
{
	CVector c( pTest.x, pTest.y, 0.0f );
	CVector a( lineA.x, lineA.y, 0.0f );
	CVector b( lineB.x, lineB.y, 0.0f );

	CVector ab = b - a;
	// Project c onto ab, computing parameterized d(t) = a + t*(b - a)
	t = CVector::DotProduct( (c-a), ab ) / CVector::DotProduct( ab, ab );
	// If outside the segment, clamp t to closest endpoint
	if ( t < 0.0f )		t = 0.0f;
	if ( t > 1.0f )		t = 1.0f;
	// Compute projected position from the clamped t
	CVector d;
	d = a + ( ab * t );
	pClosest.x = d.x;	pClosest.y = d.y;
}


