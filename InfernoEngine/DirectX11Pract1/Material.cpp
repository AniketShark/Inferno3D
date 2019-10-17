
#include "Material.h"


Material::Material()
{
	this->mAmbient = XMFLOAT4(0.5f,0.5f,0.5f,1);
	this->mDiffuse = XMFLOAT4(1,1,1,1);
	this->mSpecular = XMFLOAT4(1,1,1,1);
	this->mMeshColor = XMFLOAT4(1,1,1,1);
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