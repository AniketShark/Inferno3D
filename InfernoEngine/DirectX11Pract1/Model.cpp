
#include "Model.h"

Model::Model()
{
	transform.position = Vector3::Zero;
	transform.worldEulerRotation = Vector3::Zero;
	transform.scale = Vector3::One;
	mIsSkybox = false;
	mIsTransperent = false;
	mIsLightingOnly = false;
	mIsTextureOnly= false;
	mIsTwoTextureOnly= false;
	mIsTextureAndLightingOnly= false;
	mIsTwoTextureAndLightingOnly= false;
	mIsNormalMapped = false;
	
}
Model::~Model()
{
	
}

Model::Model(const Model& otherModel)
{
	this->mBounds = otherModel.mBounds;
	this->mPlyLoader = otherModel.mPlyLoader;
	this->modelInfo = otherModel.modelInfo;
	this->mMaterial = otherModel.mMaterial;
	this->mChildObjectsList = otherModel.mChildObjectsList;

	this->mName = otherModel.mName;
	this->transform.position = otherModel.transform.position;
	this->transform.worldEulerRotation = otherModel.transform.worldEulerRotation;
	this->transform.localEulerRotation = otherModel.transform.localEulerRotation;
	this->transform.scale = otherModel.transform.scale;
	for (int index = 0; index != otherModel.mTextureArray.size();index++)
	{
		this->mTextureArray.push_back(otherModel.mTextureArray[index]);
	}
} 

void Model::operator= (const Model& otherModel)
{
	this->mBounds = otherModel.mBounds;
	this->mPlyLoader = otherModel.mPlyLoader;
	this->modelInfo = otherModel.modelInfo;
	this->mMaterial = otherModel.mMaterial;
	this->mChildObjectsList = otherModel.mChildObjectsList;

	this->mName = otherModel.mName;
	this->transform.position = otherModel.transform.position;
	this->transform.worldEulerRotation = otherModel.transform.worldEulerRotation;
	this->transform.localEulerRotation = otherModel.transform.localEulerRotation;
	this->transform.scale = otherModel.transform.scale;

	for (int index = 0; index != otherModel.mTextureArray.size();index++)
	{
		this->mTextureArray.push_back(otherModel.mTextureArray[index]);
	}
}

Model::Model(std::string modelName)
{
	HRESULT hr = CoCreateGuid(&mObjectID);

	if(!mPlyLoader.LoadPlyFile(modelName))
	{
		MessageBox(NULL,L"The ply file loading failed",L"File Reading Error",MB_OK);
	}
	else 
	{
		MessageBox(NULL,L"The ply file loading failed",L"File Reading Error",MB_OK);
	}
}

void Model::CalculateModelBounds()
{
	this->mBounds.mMin = this->mPlyLoader.m_Min;
	this->mBounds.mMax = this->mPlyLoader.m_Max;

	Vector3 center;
	center.x = (this->mBounds.mMax.x - this->mBounds.mMin.x)/2; 
	center.y = (this->mBounds.mMax.y - this->mBounds.mMin.y)/2; 
	center.z = (this->mBounds.mMax.z - this->mBounds.mMin.z)/2; 

	this->mBounds.mExtents.x = (this->mBounds.mMax.x - this->mBounds.mMin.x)/2;
	this->mBounds.mExtents.y = (this->mBounds.mMax.x - this->mBounds.mMin.x)/2;
	this->mBounds.mExtents.z = (this->mBounds.mMax.x - this->mBounds.mMin.x)/2;

	mBounds.mCenter = center;
}

void Model::UpdateChildsTransforms()
{

}

void Model::ReadBVHFile(std::wstring bvhFileName,AnimationType animationType)
{
	int errorCode;
	std::string animationName = std::string(bvhFileName.begin(),bvhFileName.end()); 
	bvhFileName = L"Animations/" + std::wstring(bvhFileName.begin(),bvhFileName.end()); 
	mBvhFileList[animationName] = new CBVHFile;
	mBvhFileList[animationName]->LoadBVHFile(bvhFileName,errorCode);
	mBvhFileList[animationName]->mAnimationType = animationType;
	mBvhFileList[animationName]->m_FileName = animationName;
	m_LastAnimationFile = animationName;
}

bool Model::CreateBiped(Model* parentModel)
{
	mBiped = new Biped;
	mBiped->LoadJointMeshMap(L"Animations/JointMeshConfig.txt");
	mBiped->RepresentModel(mBvhFileList[m_LastAnimationFile]->rootJoint,parentModel);
	for (std::map<std::string,CBVHFile*>::iterator bvhIt = mBvhFileList.begin();bvhIt != mBvhFileList.end();bvhIt++)
	{
		bvhIt->second->SetBipedReference(mBiped);
	}
	
	return 0;
}

void Model::SwitchAnimationImmidiately(std::string animationName)
{
	mBiped->mSwitchAnimationImmidiate = true;
	m_SwitchToAnimationName = animationName;
}
void Model::SwitchAnimationAfterCurrent(std::string animationName)
{
	mBiped->mSwitchAnimationAfterCurrent = true;
	m_SwitchToAnimationName = animationName;
}


