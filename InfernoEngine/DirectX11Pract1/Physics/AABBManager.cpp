#include "AABBManager.h"


AABBManager::AABBManager(){}
AABBManager::~AABBManager(){}

bool AABBManager::InitializeGrid(Vector3 worldMin,Vector3 worldMax,float AABBSize)
{
	//this->mWorldMax.x = worldMax.x;
	//this->mWorldMin.x = worldMin.x;
	//this->mWorldMin.y = worldMin.y;
	//this->mWorldMax.y = worldMax.y;
	//this->mWorldMax.z = worldMax.z;
	//this->mWorldMin.z = worldMin.z;

	//this->mAABBSize = AABBSize;
	//// 1. determine how many AABBs we need on each axis
	//float worldDeltaX =  worldMax.x - worldMin.x;		// 5-(-5) = 10
	//int numberOfAABBsOnXAxis = worldDeltaX / AABBSize;	// 10 boxes on x

	//float worldDeltaY = worldMax.y - worldMin.y;		// 5-(-5) = 10
	//int numberOfAABBsOnYAxis = worldDeltaY / AABBSize;	// 10 boxes on y

	//float worldDeltaZ = worldMax.z - worldMin.z;
	//int numberOfAABBsOnZAxis = worldDeltaZ/AABBSize;


	//// This will create ALL the possible AABBs, but we might 
	////	want to create a "sparse" grid if we don't have enough memory...

	//int indexX = 0;
	//float currentXMin = worldMin.x;
	//for ( ; indexX != numberOfAABBsOnXAxis; indexX++ )
	//{
	//	int indexY = 0;
	//	float currentYMin = worldMin.y;
	//	for ( ; indexY != numberOfAABBsOnYAxis; indexY++ )
	//	{
	//		int indexZ = 0;
	//		float currentZMin = worldMin.z;
	//		for ( ; indexZ != numberOfAABBsOnZAxis; indexZ++ )
	//		{
	//			// Create an AABB and add it to the vector...
	//			AABB tempAABB;

	//			// Load up the max and min x & y of this AABB
	//			tempAABB.mMin.x = currentXMin;			
	//			tempAABB.mMin.y = currentYMin;
	//			tempAABB.mMin.z = currentZMin;
	//			tempAABB.mMax.x = tempAABB.mMin.x + AABBSize;
	//			tempAABB.mMax.y = tempAABB.mMin.y + AABBSize;
	//			tempAABB.mMax.z = tempAABB.mMin.z + AABBSize;

	//			//// Test if there are any triangles in this area
	//			//for ( std::vector< Triangle >::iterator itTri = triangleList.begin();itTri != triangleList.end(); itTri++ )
	//			//{
	//			//	// Get each point on the triangle and see if they are inside the 
	//			//	//	AABB.... Question: Is there any possible problem with this?

	//			//	Triangle testTriange = *itTri;		// Dereference the triangle

	//			//	if ( tempAABB.IsPointInside( testTriange.mPointA))
	//			//	{
	//			//		tempAABB.AddTriangle(testTriange);
	//			//	}
	//			//	if ( tempAABB.IsPointInside( testTriange.mPointB ))
	//			//	{
	//			//		tempAABB.AddTriangle(testTriange);
	//			//	}
	//			//	if ( tempAABB.IsPointInside( testTriange.mPointC))
	//			//	{
	//			//		tempAABB.AddTriangle(testTriange);
	//			//	}
	//			//}
	//			//// If the AABB doesn't have any triangles, do we need to add it?
	//			//// If we don't have ALL the AABBs, or we don't have any 
	//			////	empty ones, they we have a "sparse" data structure.
	//			//if ( !tempAABB.mTriangleList.empty() )
	//			//{
	//			//	//this->vecAABBs.push_back( tempAABB );
	//			int indexAABB = this->CalculateAABBIndex(tempAABB.mMin);
	//			//	// Add this to the map

	//			this->mAABBGrid[ indexAABB ].mMax = tempAABB.mMax;
	//			this->mAABBGrid[ indexAABB ].mMin = tempAABB.mMin;

	//			//} 
	//			currentZMin += AABBSize;
	//		}
	//		currentYMin += AABBSize;
	//	}
	//	currentXMin += AABBSize;
	//}

	return true;
}

std::vector<Triangle> AABBManager::FindTrianglesInThisAABB(Vector3 point)
{

	std::vector<Triangle> vecTriangles;

	int indexOfAABB = CalculateAABBIndex(point);

	std::map<int,AABB>::iterator mapIt = this->mAABBGrid.find(indexOfAABB);

	if(mapIt != mAABBGrid.end())
	{
		vecTriangles = mapIt->second.mTriangleList;
	}

	return vecTriangles;
}

int AABBManager::CalculateAABBIndex(Vector3 objectPosition)
{
	// Say give it x = 3, y = 4... 
	//000004003
	// Since the numbers are negative, offset them by the world min

	float xOffset = objectPosition.x/mAABBSize + abs(mWorldMin.x);
	float yOffset = objectPosition.y/mAABBSize + abs(mWorldMin.y);
	float zOffset = objectPosition.z/mAABBSize + abs(mWorldMin.z);

	int xAxisValue = static_cast<int>( xOffset );
	int yAxisValue = static_cast<int>( yOffset );
	int zAxisValue = static_cast<int>( zOffset);

	// Shift the y value over by three digits...
	yAxisValue *= 1000;
	zAxisValue *= 10000;
	int finalIndex = xAxisValue + yAxisValue + zAxisValue;

	return finalIndex;

}

int AABBManager::GetIndexOfAABBContainingThisPoint(Vector3 point)
{
	int indexOfAABB = CalculateAABBIndex(point);
	return indexOfAABB;
}

void AABBManager::UpdateGrid(std::vector<Triangle> trianglesList)
{
	this->mAABBGrid.clear();
	//InitializeGrid(trianglesList,this->mWorldMin,this->mWorldMax,this->mAABBSize);
}

bool InitializeGrid(std::map<int,AABB>& aabbGrid,std::vector<Triangle> triangleList, Vector3 worldMin,Vector3 worldMax)
{

	//Vector3 mWorldMax = Vector3::Zero;
	//Vector3 mWorldMin= Vector3::Zero;

	//float mAABBSize  = 0;
	//mWorldMax.x = worldMax.x;
	//mWorldMin.x = worldMin.x;
	//mWorldMin.y = worldMin.y;
	//mWorldMax.y = worldMax.y;
	//mWorldMax.z = worldMax.z;
	//mWorldMin.z = worldMin.z;

	//mAABBSize = abs(mWorldMax.x - mWorldMin.x) * 0.50f;

	//// 1. determine how many AABBs we need on each axis
	//float worldDeltaX =  worldMax.x - worldMin.x;		// 5-(-5) = 10
	//int numberOfAABBsOnXAxis = worldDeltaX / mAABBSize;	// 10 boxes on x

	//float worldDeltaY = worldMax.y - worldMin.y;		// 5-(-5) = 10
	//int numberOfAABBsOnYAxis = worldDeltaY / mAABBSize;	// 10 boxes on y

	//float worldDeltaZ = worldMax.z - worldMin.z;
	//int numberOfAABBsOnZAxis = worldDeltaZ/mAABBSize;


	//// This will create ALL the possible AABBs, but we might 
	////	want to create a "sparse" grid if we don't have enough memory...

	//int indexX = 0;
	//float currentXMin = worldMin.x;
	//for ( ; indexX != numberOfAABBsOnXAxis; indexX++ )
	//{
	//	int indexY = 0;
	//	float currentYMin = worldMin.y;
	//	for ( ; indexY != numberOfAABBsOnYAxis; indexY++ )
	//	{
	//		int indexZ = 0;
	//		float currentZMin = worldMin.z;
	//		for ( ; indexZ != numberOfAABBsOnZAxis; indexZ++ )
	//		{
	//			// Create an AABB and add it to the vector...
	//			AABB tempAABB;

	//			// Load up the max and min x & y of this AABB
	//			tempAABB.mMin.x = currentXMin;			
	//			tempAABB.mMin.y = currentYMin;
	//			tempAABB.mMin.z = currentZMin;
	//			tempAABB.mMax.x = tempAABB.mMin.x + mAABBSize;
	//			tempAABB.mMax.y = tempAABB.mMin.y + mAABBSize;
	//			tempAABB.mMax.z = tempAABB.mMin.z + mAABBSize;

	//			tempAABB.RecalculateBounds();

	//			// Test if there are any triangles in this area
	//			for ( std::vector< Triangle >::iterator itTri = triangleList.begin();itTri != triangleList.end(); itTri++ )
	//			{
	//				// Get each point on the triangle and see if they are inside the 
	//				//	AABB.... Question: Is there any possible problem with this?

	//				Triangle testTriange = *itTri;		// Dereference the triangle

	//				if ( tempAABB.IsPointInside(testTriange.mPointA))
	//				{
	//					tempAABB.AddTriangle(testTriange);
	//				}
	//				if ( tempAABB.IsPointInside( testTriange.mPointB ))
	//				{
	//					tempAABB.AddTriangle(testTriange);
	//				}
	//				if ( tempAABB.IsPointInside( testTriange.mPointC))
	//				{
	//					tempAABB.AddTriangle(testTriange);
	//				}
	//			}
	//			// If the AABB doesn't have any triangles, do we need to add it?
	//			// If we don't have ALL the AABBs, or we don't have any 
	//			//	empty ones, they we have a "sparse" data structure.
	//			if ( !tempAABB.mTriangleList.empty() )
	//			{
	//				//this->vecAABBs.push_back( tempAABB );

	//			int indexAABB = CalculateAABBIndex(tempAABB.mMin,mAABBSize,mWorldMin);
	//			// Add this to the map

	//			aabbGrid[ indexAABB ].mMin = tempAABB.mMin;
	//			aabbGrid[ indexAABB ].mMax = tempAABB.mMax;
	//			aabbGrid[ indexAABB ].mTriangleList = tempAABB.mTriangleList;
	//			}

	//			currentZMin += mAABBSize;
	//		}
	//		currentYMin += mAABBSize;
	//	}
	//	currentXMin += mAABBSize;
	//}

	return true;
}

int CalculateAABBIndex(Vector3 objectPosition,float aabbSize,Vector3 worldMin)
{
	// Say give it x = 3, y = 4... 
	//000004003
	// Since the numbers are negative, offset them by the world min
	float xOffset = objectPosition.x/aabbSize + abs(worldMin.x);
	float yOffset = objectPosition.y/aabbSize  + abs(worldMin.y);
	float zOffset = objectPosition.z/aabbSize + abs(worldMin.z);

	int xAxisValue = static_cast<int>( xOffset );
	int yAxisValue = static_cast<int>( yOffset );
	int zAxisValue = static_cast<int>( zOffset);
	// Shift the y value over by three digits...
	yAxisValue *= 1000;
	zAxisValue *= 10000;
	int finalIndex = xAxisValue + yAxisValue + zAxisValue;

	return finalIndex;
}

void CreateBoundingBoxHierarchy(int noOfLevels,AABB& parentAABB,Model& model)
{
	if(parentAABB.mMyLevel >= noOfLevels)
		return;

	InitializeGrid(parentAABB.mChildAABBs,model.mPlyLoader.m_TriangleListWithPoints,parentAABB.mMin,parentAABB.mMax);

	std::map<int,AABB>::iterator mapIt; 
	for(mapIt = parentAABB.mChildAABBs.begin(); mapIt != parentAABB.mChildAABBs.end();mapIt++)
	{
		mapIt->second.mMyLevel = parentAABB.mMyLevel + 1;
		CreateBoundingBoxHierarchy(noOfLevels,mapIt->second,model);
	}
}

