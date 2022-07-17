#ifndef Light_H_
#define Light_H_

#include "IBehavior.h"
#include "global.h"
#include "Material.h"
#include "Vector.h"

struct LightInfo
{
	DirectX::XMFLOAT4 vPosition;
	DirectX::XMFLOAT4 vLightDirection;
	DirectX::XMFLOAT4 attenuation;
	float power;
	float range;
	float typeOfLight;
	float padding;
	Material material;
};

struct ConstantBuffer
{

	DirectX::XMMATRIX  mWorld;
	DirectX::XMMATRIX  mView;
	DirectX::XMMATRIX  mProjection;
	DirectX::XMMATRIX  mInverseTransposedWorld;
	DirectX::XMFLOAT4  mEyePosition;
	UINT	  mNoOfLights;
	UINT      mNoOfBones;
	UINT      mGpuSkinnig;
	UINT      mPadding;
	LightInfo mlights[10];
};


class Light : public IBehaviour
{
public:
	Light();
	~Light();
	LightInfo* mLightInfo;

	//Initializes Object
	virtual void Initialize();
	//Updates Component every frame
	virtual void Update();
	//Destroys and releses all the resources when called 
	virtual void Destroy();
	//Clones the behaviour
	virtual IBehaviour* Clone();
	virtual ComponentType GetType();

	void SetLightColor(Vector4 color);
};


#endif