#ifndef AABBManager_H_
#define AABBManager_H_


#include "Model.h"
#include "Vector.h"
#include "Shape.h"
#include "Model.h"
#include "AABB.h"
#include <vector>
#include <map>

class AABBManager
{

public:
	AABBManager();
	virtual ~AABBManager();
	float mAABBSize;
	std::map<int,AABB> mAABBGrid;
	Vector3 mWorldMin;
	Vector3 mWorldMax;

	bool InitializeGrid(Vector3 worldMin,Vector3 worldMax, float AABBSize);
	std::vector<Triangle> FindTrianglesInThisAABB(Vector3 point);
	int GetIndexOfAABBContainingThisPoint(Vector3 point);
	int CalculateAABBIndex(Vector3 objectPosition);
	void UpdateGrid(std::vector<Triangle> trianglesList);
};

int CalculateAABBIndex(Vector3 objectPosition,float aabbSize,Vector3 worldMin);
bool InitializeGrid(std::map<int,AABB>& aabbGrid,std::vector<Triangle> triangleList,Vector3 worldMin,Vector3 worldMax);
void CreateBoundingBoxHierarchy(int noOfLevels,AABB& parentAABB,Model& model);

#endif





