#ifndef ARMOUR_H
#define ARMOUR_H

#include "PowerUp.h"

#include <stddef.h>
#include <string.h>

class Armor : public PowerUp
{
public:
    Armor(const char* name, const Vertex& position) :
        PowerUp(name, position)
    {
        mType = ARMOUR;
        mClanTag = "";
    }

    ~Armor()
    {
        //delete mClanTag;
    }

    std::string GetClanTag() const
    {
        return(mClanTag);
    }

    void SetClanTag(std::string n)
    {
       // delete mClanTag;
        mClanTag = n;//new char[strlen(n)];
        //strcpy(mClanTag, n);
    }

	// Added properties
	int GetDefence()
	{
		return mDefence;
	}
	int SetDefence(int defenceVal)
	{
		mDefence = defenceVal;
	}
	int GetRequiredLevel()
	{
		return mRequiredLevel;
	}
	int SetRequiredLevel(int requiredLevel)
	{
		 mRequiredLevel = requiredLevel;
	}
	bool IsUnlocked()
	{
		return mUnlocked;
	}
	ItemType GetItemType()
	{
		return mItemType; // declared in PowerUp class
	}
protected:
    std::string mClanTag;

	//Added properties
	int mDefence;
	int mRequiredLevel;
	bool mUnlocked;
};

#endif // ARMOUR_H

