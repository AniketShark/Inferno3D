#include "AABBManager.h"
#include <math.h>

int g_globalIterationCounter = 0;

AABB::AABB()
{
	mParentIterationCount = 0;
	mMyLevel = 0;
}

AABB::~AABB()
{
	
}

AABB::AABB(const AABB& other)
{
	this->mMax = other.mMax;
	this->mMin= other.mMin;
	this->mCenter = other.mCenter;
	this->mExtents = other.mExtents;
	this->radius = other.radius;
	this->mTypeOfAABB = other.mTypeOfAABB;
	this->mNoOfLevels = other.mNoOfLevels;
	this->mTriangleList = other.mTriangleList;
	this->mChildAABBs = other.mChildAABBs;
	this->mParentIterationCount = other.mParentIterationCount;
	this->mMyLevel = other.mMyLevel;
}

void AABB::operator= (const AABB& other)
{
	this->mMax = other.mMax;
	this->mMin= other.mMin;
	this->mCenter = other.mCenter;
	this->mExtents = other.mExtents;
	this->radius = other.radius;
	this->mTypeOfAABB = other.mTypeOfAABB;
	this->mNoOfLevels = other.mNoOfLevels;
	this->mTriangleList = other.mTriangleList;
	this->mChildAABBs = other.mChildAABBs;
	this->mParentIterationCount = other.mParentIterationCount;
	this->mMyLevel = other.mMyLevel;

}
void AABB::Update()
{

}

void AABB::CalculateAABB()
{

}
void AABB::RecalculateBounds()
{
	Vector3 center;

	center.x = (this->mMax.x - this->mMin.x)/2; 
	center.y = (this->mMax.y - this->mMin.y)/2; 
	center.z = (this->mMax.z - this->mMin.z)/2; 

	this->mExtents.x = (this->mMax.x - this->mMin.x)/2;
	this->mExtents.y = (this->mMax.x - this->mMin.x)/2;
	this->mExtents.z = (this->mMax.x - this->mMin.x)/2;

	this->mCenter = center;
}

bool AABB::IsPointInside(Vector3 point)
{
	// If this is inside BOTH x AND y ranges, then it's inside.
	// NOTE: you'll have to decide about < and <= 
	if ( (point.x >= this->mMin.x) && (point.x < this->mMax.x))	
	{
		if ((point.y >= this->mMin.y) && (point.y < this->mMax.y) )
		{
			if ((point.z >= this->mMin.z) && (point.z < this->mMax.z))
				return true;
		}
	}
	return false;
}

void AABB::AddTriangle(Triangle triangle)
{
	this->mTriangleList.push_back(triangle);
}