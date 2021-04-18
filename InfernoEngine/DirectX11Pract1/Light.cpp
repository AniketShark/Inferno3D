#include "Light.h"
#include "Scene.h"
#include "Console.h"
#include "Helper.h"
#include "SharedInfo.h"

Light::Light()
{
	mThisGO = new SharedInfo;
}
Light::~Light()
{
	delete mThisGO;
	delete mLightInfo;
}

void Light::Initialize()
{
	mLightInfo = new LightInfo;
	
	mLightInfo->attenuation = DirectX::XMFLOAT4(0.001f, 0.001f, 0.001f,1);
	mLightInfo->vLightDirection = DirectX::XMFLOAT4(2,1,1,1);
	mLightInfo->material.mDiffuse = DirectX::XMFLOAT4(1,0,0,1);
	mLightInfo->material.mAmbient = DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1);
	mLightInfo->power = 1000;
	mLightInfo->range = 1000;
	mLightInfo->typeOfLight = 0;
	mLightInfo->padding = 1;

	Scene::RegisterLight(mLightInfo);
}
void Light::Update()
{
	if(mLightInfo != NULL)
		mLightInfo->vPosition = Helper::ConvertVector3ToXMFLOAT4(transform->position);
}
void Light::Destroy()
{

}
IBehaviour* Light::Clone()
{
	Light* lt = new Light;
	(*lt->mLightInfo) = (*this->mLightInfo);
	return lt;	
}

ComponentType Light::GetType()
{
	return LightComp;
}

void Light::SetLightColor(Vector4 color)
{
	mLightInfo->material.mDiffuse.x = color.x;
	mLightInfo->material.mDiffuse.y = color.y;
	mLightInfo->material.mDiffuse.z = color.z;
	mLightInfo->material.mDiffuse.w = color.y;
}


 //<LevelConfig>


	// <Lights>
	// <I>
	// <Position> 0 1 0 </Position>
	// <LightDirection> 2 1 1 </LightDirection>
	// <Type> 0 </Type>
	// <Attenuation> 0.001 0.001 0.001 </Attenuation>
	// <Power> 10 </Power>
	// <Range> 1000 </Range>
	// <Padding> 1 </Padding> 
	// <Material>
	// <Diffuse> 0.5 0.5 0.5 1 </Diffuse>
	// <Ambient> 0.2 0.2 0.2 1 </Ambient>
	// </Material>
	// </I>

	// </Lights>

	// </LevelConfig>
