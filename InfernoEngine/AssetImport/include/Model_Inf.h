

#ifndef Model_H_
#define Model_H_

#include "Material.h"
#include "Texture.h"
#include "PlyUtility.h"


#define MAX_NO_OF_TEXTURES 3

class  Model_Inf
{
public:
	Model_Inf();
	virtual ~Model_Inf();
	Model_Inf(const Model_Inf&);
	Model_Inf(std::string modelName);

	bool mIsSkybox;
	bool mIsTransperent;
	bool mIsLightingOnly;
	bool mIsTextureOnly;
	bool mIsTwoTextureOnly;
	bool mIsTextureAndLightingOnly;
	bool mIsTwoTextureAndLightingOnly;
	bool mIsNormalMapped;

	std::vector<Model_Inf*> mChildObjectsList;
	Material mMaterial;
	std::vector<Texture> mTextureArray;
	PlyUtility mPlyLoader;
	RenderInfo modelInfo;
	
	void operator= (const Model_Inf&);

	void CalculateModelBounds();
	void UpdateChildsTransforms();
	bool CreateBiped(Model_Inf* parentModel);
	void SwitchAnimationImmidiately(std::string animationName);
	void SwitchAnimationAfterCurrent(std::string animationName);
	void CalculateTangentBinormal(Vertex vertex1, Vertex vertex2, Vertex vertex3, DirectX::XMFLOAT4& tangent, DirectX::XMFLOAT4& binormal);

private:
	std::string m_LastAnimationFile;
	std::string m_SwitchToAnimationName;
	bool m_SwitchAnimationImmidiate;
	bool m_SwitchAnimationAfterCurrent;
};

#endif