#ifndef DISPLAY_MESH_H
#define DISPLAY_MESH_H

#include "Entity.h"
#include "HelperFuncs.h"

class Mesh;
class SkeletalAnimation;

class DisplayMesh : public Entity
{
public:
    DisplayMesh();
    virtual ~DisplayMesh();

	void SetMesh(Mesh* pMesh);
    virtual HRESULT Initialize();
    virtual void Update(float delta);
    virtual void Draw();

    enum DisplayMode {
        DM_MESH_ONLY,
        DM_SKEL_ONLY,
        DM_MESH_AND_SKEL,
        DM_MAX
    };

    void SetDisplayMode(DisplayMode dm);
    DisplayMode GetDisplayMode() const;

    void SetDrawBindPose(bool bindPose) { mDrawBindPose = bindPose; }
    bool GetDrawBindPose() const { return(mDrawBindPose); }

    void SetSingleBoneSkinning(bool single) { mSingleBoneSkinning = single; }
    bool GetSingleBoneSkinning() const { return(mSingleBoneSkinning); }

    void SetFrameInterpolate(bool interpolate) { mFrameInterpolate = interpolate; }
    bool GetFrameInterpolate() const { return(mFrameInterpolate); }

    void SetDebugRotation(float rot) { mDebugRotation = rot; }
    float GetDebugRotation() const { return(mDebugRotation); }

    void PlayAnimation(SkeletalAnimation* anim);
    
    char* GetPlayingAnimation() const;
    float GetCurrentAnimTime() const;
    float GetCurrentAnimFrame() const;

    void SetAnimRate(float rate) { mAnimRate = rate; }
    float GetAnimRate() const { return(mAnimRate); }

protected:
	Mesh*                   mMesh; //not owned by this class
    SkeletalAnimation*      mAnimation; //not owned by this class
    unsigned char*          mChannelMap;

    DisplayMode             mDisplayMode;

    SimpleVertex*           mCurrentVerts;
    JointPose*              mCurrentBones;
    _XMFLOAT4X4*            mCurrentGlobalPose;
    SimpleVertex*           mBoneVerts;
    WORD*                   mBoneIndices;

    bool                    mDrawBindPose;
    bool                    mSingleBoneSkinning;
    bool                    mFrameInterpolate;
    float                   mDebugRotation;

    float                   mAnimTime;
    float                   mAnimRate;
    float                   mCurrentFrame;

    //DX stuff
    ID3D11VertexShader*     mVertexShader;
    ID3D11PixelShader*      mPixelShader;
    ID3D11InputLayout*      mVertexLayout;
    ID3D11Buffer*           mVertexBuffer;
    ID3D11Buffer*           mIndexBuffer;
    ID3D11Buffer*           mConstantBuffer;
    ID3D11Buffer*           mSkelVertexBuffer;
    ID3D11Buffer*           mSkelIndexBuffer;
};

#endif
