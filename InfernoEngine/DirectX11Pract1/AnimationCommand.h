

#ifndef AnimationCommand_HG_
#define  AnimationCommand_HG_

#include <string>
#include "Vector.h"

class AnimationCommand
{
public:
	AnimationCommand();
	~AnimationCommand();

	 std::string m_command;
	 Vector3 m_source;
	 Vector3 m_destination;
	 Vector3 m_targetRotation;
	 Vector3 m_targetScale;
	 std::string m_accelerationType;
	 int m_accelerationSteps;
	 float time;
};

#endif