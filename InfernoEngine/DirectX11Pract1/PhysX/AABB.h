#ifndef AABB_H_
#define AABB_H_


#include "Vector.h"
#include "Shape.h"
#include <map>


extern int g_globalIterationCounter;
class AABB
{
public:
	AABB();
	AABB(const AABB&);
	~AABB();
	enum AABBType
	{
		STATIC,
		DYNAMIC
	};
	int mParentIterationCount;
	int mMyLevel;
	Vector3 mCenter;
	Vector3 mExtents;
	float radius;
	Vector3 mMin;
	Vector3 mMax;
	AABBType mTypeOfAABB;
	int mNoOfLevels;
	std::vector<Triangle> mTriangleList;
	std::map<int,AABB> mChildAABBs;

	void Update();
	void CalculateAABB();
	void RecalculateBounds();
	void operator= (const AABB&);
	bool IsPointInside(Vector3 point);
	void AddTriangle(Triangle triangle);
};



#endif