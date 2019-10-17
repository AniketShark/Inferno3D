#include "SkeletalAnimation.h"

SkeletalAnimation::SkeletalAnimation() :
	mSkeleton(NULL),
    mNumBones(0),
    mKeys(NULL),
    mNumKeys(0),
    mDuration(0.0f)
{
    memset(mName, 0, 100);
}

SkeletalAnimation::~SkeletalAnimation()
{
	if(mSkeleton) delete [] mSkeleton;

    if(mKeys)
    {
        for(int i = 0; i < mNumKeys; ++i)
        {
            if(mKeys[i].mBones)
            {
                delete [] mKeys[i].mBones;
            }
        }
        delete [] mKeys;
    }
}

