#ifndef SKELETAL_ANIM_H
#define SKELETAL_ANIM_H

#include "HelperFuncs.h"

class SkeletalAnimation
{
public:
	SkeletalAnimation();
	virtual ~SkeletalAnimation();

    Bone*                mSkeleton;
    int                   mNumBones;

    struct Frame
    {
        BonePose*            mBones;
        float                 mTime;

        Frame() : mBones(NULL), mTime(-1.0f) {}
    };

    Frame*                mKeys;
    int                   mNumKeys;

    float                 mDuration;

    char                  mName[100];
};

#endif
