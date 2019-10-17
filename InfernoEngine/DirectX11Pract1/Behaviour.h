#ifndef Behaviour_H_
#define Behaviour_H_


class Behaviour
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual void CleanUp() = 0;
};

#endif