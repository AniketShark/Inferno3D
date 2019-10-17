

#ifndef Model_H_
#define Model_H_

#include "Animation.h"
#include "Rigidbody.h"
#include "Material.h"
#include "Texture.h"
#include "PlyUtility.h"
#include "AABB.h"
#include "AIManager.h"
#include "CBVHFile.h"
#include "Biped.h"
#include "GameObject.h"


#define MAX_NO_OF_TEXTURES 3

class  Model : public GameObject
{
public:
	Model();
	virtual ~Model();
	Model(const Model&);
	Model(std::string modelName);

	bool mIsSkybox;
	bool mIsTransperent;
	bool mIsLightingOnly;
	bool mIsTextureOnly;
	bool mIsTwoTextureOnly;
	bool mIsTextureAndLightingOnly;
	bool mIsTwoTextureAndLightingOnly;
	bool mIsNormalMapped;

	std::vector<Model*> mChildObjectsList;
	Material mMaterial;
	AABB mBounds;
	std::vector<Texture> mTextureArray;
	PlyUtility mPlyLoader;
	RenderInfo modelInfo;
	
	std::map<std::string,CBVHFile*> mBvhFileList;
	Biped* mBiped;

	void operator= (const Model&);

	void CalculateModelBounds();
	void UpdateChildsTransforms();
	void ReadBVHFile(std::wstring bvhFileName,AnimationType animationType);
	bool CreateBiped(Model* parentModel);
	void SwitchAnimationImmidiately(std::string animationName);
	void SwitchAnimationAfterCurrent(std::string animationName);
	void CalculateTangentBinormal(Vertex vertex1, Vertex vertex2, Vertex vertex3,XMFLOAT4& tangent, XMFLOAT4& binormal);

private:
	std::string m_LastAnimationFile;
	std::string m_SwitchToAnimationName;
	bool m_SwitchAnimationImmidiate;
	bool m_SwitchAnimationAfterCurrent;
};

#endif