#ifndef PATH_NODE_H
#define PATH_NODE_H

#include "Vector.h"

#include <vector>
#include <algorithm>

class PathNode;
typedef std::vector<PathNode*> PathNodes;

class PowerUp;
typedef std::vector<PowerUp*> PowerUps;

class PathNode
{
public:
    PathNode(std::string name, Vector3 position) :
        mPosition(position)
    {
        mName  = name;//= new char [strlen(name)];
        //strcpy(mName, name);
    }
    
    ~PathNode()
    {
    }

    void AddLink(PathNode *pathNode)
    {
        mLinks.push_back(pathNode);
    }
    
    void RemoveLink(PathNode *pathNode)
    {
        PathNodes::iterator i = std::find(mLinks.begin(), mLinks.end(), pathNode);
        mLinks.erase(i);
    }

    void AddPowerUp(PowerUp *powerUp)
    {
        mPowerUps.push_back(powerUp);
    }
    
    void RemovePowerUp(PowerUp *powerUp)
    {
        PowerUps::iterator i = std::find(mPowerUps.begin(), mPowerUps.end(), powerUp);
        mPowerUps.erase(i);
    }

    std::string GetName() const
    {
        return(mName);
    }

    const PathNodes& GetLinks() const
    {
        return(mLinks);
    }

    const PowerUps& GetPowerUps() const
    {
        return(mPowerUps); 
    }

	Vector3 GetPosition()
	{
		return mPosition;
	}

	bool visited;

	PathNode* mPrevVisitedNode;
	int CostSoFar;

	
protected:
   Vector3      mPosition;
   std::string       mName;

    PathNodes   mLinks;
    PowerUps    mPowerUps;

	
	
};

#endif // PATH_NODE_H
