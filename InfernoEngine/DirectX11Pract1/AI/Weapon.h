#ifndef WEAPON_H
#define WEAPON_H

#include "PowerUp.h"

class Weapon : public PowerUp
{
public:
    Weapon(const char* name, Vertex position) :
    PowerUp(name, position)
    {
        mType = WEAPON;
    }

	int GetDamage()
	{
		return mDamage;
	}

	int GetRequiredLevel()
	{
		return mRequiredLevel;
	}
	bool IsUnlocked()
	{
		return mUnlocked;
	}
	int GetAttackSpeed()
	{
		return mAttackSpeed;
	}
	ItemType GetItemType()
	{
		return mItemType; // declared in PowerUp class
	}
private:
	int mDamage;
	int mAttackSpeed;
	int mRequiredLevel;
	bool mUnlocked;
};





#endif // WEAPON_H

