#ifndef HEALTH_H
#define HEALTH_H

#include "PowerUp.h"

class Health : public PowerUp
{
public:
    Health(const char* name, Vertex position) :
        PowerUp(name, position)
    {
        mType = HEALTH;
    }

	float GetHealingTime()
	{
		return healingTime;
	}
	float SetHealingTime(float time)
	{
		healingTime = time;
	}
    
protected:
    float healing;

	//Added properties
	float healingTime;
};

#endif // HEALTH_H

