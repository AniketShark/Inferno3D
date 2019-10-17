#ifndef Light_H_
#define Light_H_

#include "IBehavior.h"
#include "global.h"

class Light : public IBehaviour
{
public:
	Light();
	~Light();
	LightInfo* mLightInfo;

	//Initializes Object
	virtual void Initialize();
	//Updates Component every frame
	virtual void Update();
	//Destroys and releses all the resources when called 
	virtual void Destroy();
	//Clones the behaviour
	virtual IBehaviour* Clone();
	virtual ComponentType GetType();


	void SetLightColor(Vector4 color);
	
	
};


#endif