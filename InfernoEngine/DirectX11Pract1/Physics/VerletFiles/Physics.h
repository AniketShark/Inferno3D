#ifndef _Physics_HG_
#define _Physics_HG_

#include "BasicTypes.h"
//#include "Shapes.h"
#include "Global.h"
#include <vector>

// Used in the TestPointTriangle and other functions...
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
void BarycentricCoordinates( const CPoint &a, const CPoint &b, const CPoint &c, 
							 const CPoint &p, float &u, float &v, float &w);

// Test if point p is contained in triangle (a, b, c)
// a, b, c --> triangle coordinates
// p --> point to test
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
bool TestPointTriangle( const CPoint &p, const CPoint &a, const CPoint &b, const CPoint &c);

// Tests what the closest point to a triangle is. Used for the Sphere-Triangle intersection test
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
CPoint ClosestPointToTriangle( const CPoint &point, 
							   const CPoint &triA, const CPoint &triB, const CPoint &triC );

float absFloat( float value );

extern const float g_GRAVITY;// = 9.81;
extern const float g_SLOWDOWN;//	0.90 (slow down per frame)



// Returns true if a) not same ball, b) are close, c) are moving towards each other
//bool testBallBallCollision( std::vector<CBall>::iterator itB1, std::vector<CBall>::iterator itB2 );

// NOTE: This assumes that these two balls have collided...
//void handleBallBallCollision( std::vector<CBall>::iterator itB1, std::vector<CBall>::iterator itB2 );

// Tests what the closest point to a triangle is. Used for the Sphere-Triangle intersection test
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
CPoint ClosestPointToTriangle( const CPoint &point, 
							   const CPoint &triA, const CPoint &triB, const CPoint &triC );

//bool TestSphereTriangle( const CBall &s, 
//					    const CPoint &triA, const CPoint &triB, const CPoint &triC, 
//						CPoint &pClosest);

// Given a point, returns the closest point on a line segment
// Adapted from from "Real Time Collision Detection" by Christer Ericson, Morgan Kaufmann Publishers, ISBN: 1-55860-732-3
void ClosestPointPointSegment( const CPoint &pTest, 
							   const CPoint &lineA, const CPoint &lineB, 
							   float &t, CPoint &pClosest );
#endif