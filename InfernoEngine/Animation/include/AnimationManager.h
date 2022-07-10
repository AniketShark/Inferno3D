#ifndef AnimationManager_H_
#define AnimationManager_H_

#include "SkeletalAnimation.h"
#include <string>
#include <map>

extern std::map<std::string,SkeletalAnimation*> animationLibrary;

SkeletalAnimation* GetAnimationByName(std::string animatioName);

#endif