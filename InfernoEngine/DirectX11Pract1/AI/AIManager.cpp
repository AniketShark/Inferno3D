#include "AIManager.h"
#include <algorithm>

bool CheckIfThere(PathNodes& path,PathNode* node);
void CreatePath(PathNodes& path,PathNode* endNode);


PathNodes sPathNodes;
PowerUps sPowerUps;
PathNode* PlayerClosestNode = 0;


bool FindPowerUp(PathNodes& path, PowerUp::PowerUpType mType, PathNode *start)
{
	for (int i = 0; i != sPathNodes.size();i++)
	{
		sPathNodes[i]->visited = false;
		sPathNodes[i]->mPrevVisitedNode = NULL;
		sPathNodes[i]->CostSoFar = -1;
	}

	std::deque<PathNode*> pathNodes;

	pathNodes.push_front(start);
	start->visited = true;
	start->CostSoFar = 0;


	while(pathNodes.size() != 0)
	{
		PathNode* checkNode =  pathNodes.front();

		PowerUps powerUps = checkNode->GetPowerUps();
		for (int jb = 0; jb!= powerUps.size();jb++)
		{
			if(powerUps[jb]->GetPowerUpType() == mType)
			{
				CreatePath(path,checkNode);
				return true;
			}
		}

		pathNodes.pop_front();
		if(checkNode->GetLinks().size() != 0)
		{
			PathNodes links = checkNode->GetLinks();
			for(int it = 0; it != links.size();it++)
			{
				PowerUps powerUps = links[it]->GetPowerUps();
				for (int j = 0; j!= powerUps.size();j++)
				{
					if(powerUps[j]->GetPowerUpType() == mType)
					{
						links[it]->mPrevVisitedNode = checkNode;
						links[it]->CostSoFar = links[it]->mPrevVisitedNode->CostSoFar + 1;
						CreatePath(path,links[it]);
						return true;
					}
				}
				if(!links[it]->visited)
				{
					pathNodes.push_front(links[it]);
					links[it]->mPrevVisitedNode = checkNode;
					links[it]->CostSoFar = links[it]->mPrevVisitedNode->CostSoFar + 1;
					links[it]->visited = true;
				}
			}
		}
	}

	return(false); // No path found.
}

bool CheckIfThere(PathNodes& path,PathNode* node)
{
	for(int index = 0; index != path.size();index++)
	{
		if(path[index]->GetName() == node->GetName())
		{
			return true;
		}
	}
	return false;
}

//Traces the path in the reverse order from end node to start node
//and finds Shortest path
void CreatePath(PathNodes& path,PathNode* endNode)
{
	int lowestCost = 10000;
	int lowestCostIndex = 0;
	PathNode* tempNode = endNode;
	while(tempNode != NULL)
	{
		if(tempNode->CostSoFar == 0)
		{
			path.push_back(tempNode);
			return;
		}

		path.push_back(tempNode);
		PathNodes links =  tempNode->GetLinks();
		int lowestCost = tempNode->CostSoFar;
		for (int index = 0; index != links.size();index++)
		{
			if(links[index]->CostSoFar < lowestCost && links[index]->visited)
			{
				lowestCost = links[index]->CostSoFar; 
				lowestCostIndex = index;
			}
		}
		tempNode = links[lowestCostIndex];
	}
	//std::reverse(path.begin(), path.end());
}

// Find Destination Node
// For this example, all links are symmetric.
inline void LinkNodes(PathNode *n1, PathNode *n2)
{
	n1->AddLink(n2);
	n2->AddLink(n1);
}

int LookUpForPosition(Vector3 position)
{
	for(int lookUpIndex = 0; lookUpIndex != sPathNodes.size();lookUpIndex++)
	{
		if(Vector3::IsEqual(position,sPathNodes[lookUpIndex]->GetPosition()))
		{
			return lookUpIndex;
		}
	}
	return -1;
}

bool FindPlayer(PathNodes& path, PathNode* end, PathNode *start)
{

	for (int i = 0; i != sPathNodes.size();i++)
	{
		sPathNodes[i]->visited = false;
		sPathNodes[i]->mPrevVisitedNode = NULL;
		sPathNodes[i]->CostSoFar = -1;
	}

	std::deque<PathNode*> pathNodes;

	pathNodes.push_front(start);
	start->visited = true;
	start->CostSoFar = 0;


	while(pathNodes.size() != 0)
	{
		PathNode* checkNode =  pathNodes.front();

		//PowerUps powerUps = checkNode->GetPowerUps();
		//for (int jb = 0; jb!= powerUps.size();jb++)
		//{
		if(Vector3::IsEqual(checkNode->GetPosition(),end->GetPosition()))
		{
			CreatePath(path,checkNode);
			return true;
		}
		//}

		pathNodes.pop_front();
		if(checkNode->GetLinks().size() != 0)
		{
			PathNodes links = checkNode->GetLinks();
			for(int it = 0; it != links.size();it++)
			{
				if(links[it]->GetName() == end->GetName())
				{
					links[it]->mPrevVisitedNode = checkNode;
					links[it]->CostSoFar = links[it]->mPrevVisitedNode->CostSoFar + 1;
					links[it]->visited = true;
					CreatePath(path,links[it]);
					return true;
				}
				if(!links[it]->visited)
				{
					pathNodes.push_front(links[it]);
					links[it]->mPrevVisitedNode = checkNode;
					links[it]->CostSoFar = links[it]->mPrevVisitedNode->CostSoFar + 1;
					links[it]->visited = true;
				}
				
			}
		}
	}

	return(false); // No path found.
}

// AI Class Methods 

AI::AI()
{
	mClosestNodeToPlayer = NULL;
	mPreviousClosestToPlayer = NULL;
	mGameobjectPointer = NULL;
	mCurrentFollowPathNode = NULL;
}
AI::AI(const AI& other)
{
	this->mClosestNodeToPlayer = other.mClosestNodeToPlayer;
	this->mGameobjectPointer = other.mGameobjectPointer;
}
AI::~AI()
{

}

void AI::UpdateState()
{
	float minDistance = 10000;
	int minIndex = -1;
	for (int index = 0; index != sPathNodes.size();index++)
	{
		float dist  = Vector3::Distance(mGameobjectPointer->transform.position,sPathNodes[index]->GetPosition());
		if(dist < minDistance)
		{
			minDistance = dist;
			minIndex = index;
		}
	}
	if(minIndex != -1)
		this->mClosestNodeToPlayer  = sPathNodes[minIndex];


	if(this->mGameobjectPointer->mIsAI)
	{
	//	if(this->mPreviousClosestToPlayer != NULL)
		/*	if(!Vector3::IsEqual(PlayerClosestNode->GetPosition(),mPreviousClosestToPlayer->GetPosition()))
			{*/
				FindPlayer(this->mCurrentPath,PlayerClosestNode,this->mClosestNodeToPlayer);
				this->mCurrentFollowPathNode = (*this->mCurrentPath.end());
			//}

			Chase(mCurrentFollowPathNode);
		//	mPreviousClosestToPlayer = PlayerClosestNode;
	}
	else
		PlayerClosestNode = this->mClosestNodeToPlayer;

}

void AI::Chase(PathNode* followNode)
{
	if(Vector3::IsEqual(followNode->GetPosition(),this->mGameobjectPointer->transform.position))
	{
		this->mCurrentPath.erase(this->mCurrentPath.end());
		this->mCurrentFollowPathNode = (*this->mCurrentPath.end());
		Vector3 direction  = (this->mCurrentFollowPathNode->GetPosition() -  this->mGameobjectPointer->transform.position).GetNormalized();
		//this->mGameobjectPointer->ManualMove(direction,10);
	}
}
void AI::Evade()
{

}

//#include <vector>
//#include <algorithm>
//
//int main() {
//	std::vector<int> a;
//	std::reverse(a.begin(), a.end());
//	return 0;
//