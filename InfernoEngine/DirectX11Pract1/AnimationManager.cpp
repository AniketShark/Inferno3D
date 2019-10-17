
#include "AnimationManager.h"


std::map<std::string,SkeletalAnimation*> animationLibrary;


SkeletalAnimation* GetAnimationByName(std::string animatioName)
{
	std::map<std::string,SkeletalAnimation*>::iterator animIt;
	animIt = animationLibrary.find(animatioName);
	if(animIt != animationLibrary.end())
	{
		return animIt->second;
	}

	return NULL;
}


