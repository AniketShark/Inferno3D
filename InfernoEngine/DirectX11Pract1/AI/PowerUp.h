#ifndef POWER_UP_H
#define POWER_UP_H

#include "Vertex.h"
#include <string.h>

// Item Types
enum ItemType
{
	INFERIOR,
	SUPERIOR,
	MAGIC,
	RARE
};

class PowerUp
{
public:
    PowerUp(std::string name, Vertex position)
    {
        mPosition = position;
        mName =  name; // new char [strlen(name)];
        //strcpy(mName, name);
    }

    ~PowerUp()
    {
        //delete mName;
    }

    enum PowerUpType
    {
        WEAPON,
        ARMOUR, 
        HEALTH
    };

    PowerUpType GetPowerUpType() const
    {
        return(mType);
    };

    const Vertex& GetPosition() const
    {
        return(mPosition);
    };

	ItemType	mItemType;

protected:
    Vertex      mPosition;
    PowerUpType mType;
	
    std::string       mName;
};


#endif // POWER_UP_H
