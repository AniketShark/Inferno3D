#ifndef AI_H_
#define AI_H_


#include "PathNode.h"
#include "PowerUp.h"
#include "Weapon.h"
#include "Health.h"
#include "Armor.h"
#include <stdio.h>
#include <deque>
#include "Animation.h"

extern PathNodes sPathNodes;
extern PowerUps sPowerUps;

extern PathNode* PlayerClosestNode;

bool FindPowerUp(PathNodes& path, PowerUp::PowerUpType mType, PathNode *start);
bool CheckIfThere(PathNodes& path,PathNode* node);
void CreatePath(PathNodes& path,PathNode* endNode);
int LookUpForPosition(Vector3 position);
inline void LinkNodes(PathNode *n1, PathNode *n2);

class AI
{
public:
	AI();
	AI(const AI&);
	~AI();
	PathNodes mCurrentPath;
	PathNode* mClosestNodeToPlayer;
	PathNode* mPreviousClosestToPlayer;
	PathNode* mCurrentFollowPathNode;
	void UpdateState();

	void Chase(PathNode* path);
	void Evade();
};


#endif