
#include "Material.h"


Material::Material()
{
	this->mAmbient =  DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1);
	this->mDiffuse =  DirectX::XMFLOAT4(1,1,1,1);
	this->mSpecular = DirectX::XMFLOAT4(1,1,1,1);
	this->mMeshColor= DirectX::XMFLOAT4(1,1,1,1);
}
Material::Material(const Material& otherMaterial)
{
	this->mAmbient = otherMaterial.mAmbient;
	this->mDiffuse = otherMaterial.mDiffuse;
	this->mSpecular = otherMaterial.mSpecular;
	this->mMeshColor = otherMaterial.mMeshColor;

}
Material::~Material()
{

}