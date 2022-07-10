#include "GameObject.h"
#include <math.h>
//#include <typeinfo.h>
#include "Scene.h"
#include "SharedInfo.h"
#include "Console.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameObject"/> class.
/// </summary>
GameObject::GameObject()
{
	HRESULT hr = CoCreateGuid(&mObjectID);
	renderer = NULL;
	transform.position = Vector3::Zero;
	transform.worldEulerRotation = Vector3::Zero;
	transform.localEulerRotation = Vector3::Zero;
	transform.SetLocalRotationQuaternion(Vector3::Zero);
	transform.SetWorldRotationQuaternion(Vector3::Zero);
	transform.scale = Vector3::One;
	Scene::RegisterGameObject(this);
	int exp = 0;
	int health = 0;
	int level = 0;
	
};

/// <summary>
/// Initializes a new instance of the <see cref="GameObject"/> class.
/// </summary>
/// <param name="plyFileName">Name of the ply file.</param>
GameObject::GameObject(std::string objectName)
{
	HRESULT hr = CoCreateGuid(&mObjectID);
	mName = objectName;
}
  
/// <summary>
/// Finalizes an instance of the <see cref="GameObject"/> class.
/// </summary>
GameObject::~GameObject()
{

	std::map<ComponentType,IBehaviour*>::iterator componentIt;
	for (componentIt = m_ComponentMap.begin();componentIt != m_ComponentMap.end();componentIt++)
	{
		if(componentIt->second)
		{
//			delete componentIt->second;
		}
	}
	m_ComponentMap.clear();
};

/// <summary>
/// Initializes a new instance of the <see cref="GameObject"/> class.
/// </summary>
/// <param name="">The .</param>
GameObject::GameObject(const GameObject& otherGameObject)
{
	this->transform.position = otherGameObject.transform.position;
	this->transform.worldEulerRotation = otherGameObject.transform.worldEulerRotation;
	this->transform.scale = otherGameObject.transform.scale;
}

/// <summary>
/// Operator=s the specified go.
/// </summary>
/// <param name="go">The go.</param>
void GameObject::operator=(GameObject* other)
{
	transform = other->transform;

	std::map<ComponentType, IBehaviour*> componentMap = other->GetComponentMap();
	std::map<ComponentType, IBehaviour*>::iterator componentIt;

	for (componentIt= componentMap.begin(); componentIt != componentMap.end();componentIt++)
	{
		if(componentIt->first != Script)
		{
			
			AddComponent(componentIt->first);
		}
	}
}

/// <summary>
/// Operator==s the specified go.
/// </summary>
/// <param name="go">The go.</param>
/// <returns>bool.</returns>
bool GameObject::operator== (GameObject go)
{
	if((bool)IsEqualGUID(this->mObjectID,go.mObjectID))
	{
		return true;
	}
	return false;
}

/// <summary>
/// Operator=s the specified go.
/// </summary>
/// <param name="go">The go.</param>
void GameObject::operator= (GameObject go)
{
	this->transform.position = go.transform.position;
	this->transform.worldEulerRotation = go.transform.worldEulerRotation;
	this->transform.scale = go.transform.scale;
}

// Adds component to  game object 
/// <summary>
/// Adds the component.
/// </summary>
/// <param name="component">The component.</param>

void GameObject::AddComponent(ComponentType componentType)
{
	switch(componentType)
	{
	case AnimationComp:
		//animation = new Animation();
		//m_InterlinkComponents(animation);
		//m_ComponentMap[componentType] = animation;
		break;
	case RenderComp:
		renderer = new Renderer();
		renderer->mThisGO->transform = &transform;
		m_InterlinkComponents(renderer);
		m_ComponentMap[componentType] = renderer;
		break;
	case CameraComp:
		camera = new Camera();
		camera->mThisGO->transform = &transform;
		m_InterlinkComponents(camera);
		m_ComponentMap[componentType] = camera;
		break;
	case ClothComp:
		//cloth = new(std::nothrow) Cloth();
		//cloth->transform = &transform;
		//m_InterlinkComponents(cloth);
		//m_ComponentMap[componentType] = cloth;
		break;
	case LightComp:
		light = new(std::nothrow) Light();
		light->mThisGO->transform = &transform;
		m_InterlinkComponents(light);
		m_ComponentMap[componentType] = light;
		break;
	case ColliderComp:
		//collider = new(std::nothrow) Collider();
		//collider->transform = &transform;
		/*m_InterlinkComponents(collider);
		m_ComponentMap[componentType] = collider;*/
		break;
	}
}

/// <summary>
/// Attaches the script.
/// </summary>
/// <param name="">The .</param>
//template <typename T>
//void  GameObject::AttachScript()
//{
//	T* newScriptObject = new T;
//	IBehaviour* newCastedPointer = NULL;
//	try
//	{
//		newCastedPointer = dynamic_cast<IBehaviour*>(newScriptObject);
//		newCastedPointer->transform =  &transform;
//		m_InterlinkComponents(newCastedPointer);
//		m_ComponentMap[Script] = newCastedPointer;
//	}
//	catch(exception &e)
//	{
//		MessageBox(NULL,L"ERROR: Could not create the depth-stencil buffer.",L"Error",MB_OK);
//	}
//	
//}

/// <summary>
/// Gets the component.
/// </summary>
/// <param name="component">The component.</param>

//template <typename T>
//T* GameObject::GetComponent()
//{
//	std::map<ComponentType,IBehaviour*>::iterator componentIt = m_ComponentMap.find(component);
//	
//	for (componentIt= componentMap.begin(); componentIt != componentMap.end();componentIt++)
//	{
//		if(typeid(T).name() == typeid(*(componentIt->second)).name())
//		{
//			return dynamic_cast<T*>(componentIt->second);
//		}
//	}
//
//	/*if(componentIt != m_ComponentMap.end())
//	{
//	return dynamic_cast<T*>(componentIt->second);
//	}*/
//
//	return NULL;
//}

/// <summary>
/// Gets the component map.
/// </summary>
/// <returns>std.map{_Kty, _Ty, _Pr, _Alloc}.</returns>


std::map<ComponentType,IBehaviour*> GameObject::GetComponentMap()
{
	return m_ComponentMap;
}

//////////////////////////////////////////////////////////////////////////
//   Private Methods
//////////////////////////////////////////////////////////////////////////

void GameObject::m_InterlinkComponents(IBehaviour* behaviour)
{
	std::map<ComponentType,IBehaviour*>::iterator componentIt;
	for(componentIt = m_ComponentMap.begin();componentIt != m_ComponentMap.end();componentIt++)
	{
		
		ConsolePrint(mName);
		if(componentIt->first == AnimationComp)
		{
			//behaviour->mThisGO->animation = animation;
		}
		if(componentIt->first == RenderComp)
		{
			behaviour->mThisGO->renderer = renderer;
		}
		if(componentIt->first == CameraComp)
		{
			behaviour->mThisGO->camera = camera;
		}
		if(componentIt->first == ClothComp)
		{
			//behaviour->mThisGO->cloth = cloth;
		}
		if(componentIt->first == LightComp)
		{
			behaviour->mThisGO->light = light;
		}
	}

	//behaviour->mThisGO->transform = &transform;
	behaviour->mGameObjectPointer = this;
}

void GameObject::AddClonedComponent(ComponentType componentType,IBehaviour* component)
{
	m_ComponentMap[componentType] = component;

	switch(componentType)
	{
	case RenderComp:
		renderer = dynamic_cast<Renderer*>(component);
		break;
	case CameraComp:
		camera =  dynamic_cast<Camera*> (component);
		break;
	case AnimationComp:
		//animation =  dynamic_cast<Animation*>(component);
		break;
	}
}

//void GameObject::OnCollisionEnter(Collision collision)
//{
//	ConsolePrint(collision.mCollidedTo->mName);
//}

/// <summary>
/// Clones this instance.
/// </summary>
/// <returns>GameObject *.</returns>
GameObject* GameObject::Clone()
{
	GameObject* instance = new GameObject;
	instance->transform = transform;
	std::map<ComponentType,IBehaviour*>::iterator componentIt;
	for (componentIt= m_ComponentMap.begin(); componentIt != m_ComponentMap.end();componentIt++)
	{
		instance->AddComponent(componentIt->first);
		//if(componentIt->first == Script)
			//instance->AttachScript(componentIt->second->Clone());
	}
	if(instance->renderer != NULL)
	{
		instance->renderer->LoadMesh(renderer->mMesh->mModelName);
		int noOfTexture = renderer->mMesh->mNumTextures;
		if(noOfTexture > 0)
		{
			for (int index = 0; index < noOfTexture;index++)
			{
				instance->renderer->mMesh->mTextureArray[index] = renderer->mMesh->mTextureArray[index];
			}
		}
	}

	return instance;
}

//
//XMVECTOR GameObject:: GetLocalRotationQuaternion()
//{
//	return XMVectorSet(transform.localQuaternionRotation.x,transform.localQuaternionRotation.y,transform.localQuaternionRotation.z,transform.localQuaternionRotation.w);
//}
//
//XMVECTOR GameObject::GetWorldRotationQuaternion()
//{
//	return XMVectorSet(transform.worldQuaternionRotation.x,transform.worldQuaternionRotation.y,transform.worldQuaternionRotation.z,transform.worldQuaternionRotation.w);
//}
//
//void GameObject::transform.SetLocalRotationQuaternion(Vector3 eulerRotation)
//{
//	float x = Helper::DegToRad(eulerRotation.x);
//	float y = Helper::DegToRad(eulerRotation.y);
//	float z = Helper::DegToRad(eulerRotation.z);
//	XMStoreFloat4(&transform.localQuaternionRotation,XMQuaternionRotationRollPitchYaw(x,y,z));
//}
//
//void GameObject::transform.SetWorldRotationQuaternion(Vector3 eulerRotation)
//{
//	float x = Helper::DegToRad(eulerRotation.x);
//	float y = Helper::DegToRad(eulerRotation.y);
//	float z = Helper::DegToRad(eulerRotation.z);
//	XMStoreFloat4(&transform.worldQuaternionRotation,XMQuaternionRotationRollPitchYaw(x,y,z));
//}
//
//void GameObject::transfrom.SetEulerRotationFromQuaternion(Vector3& rotationVector,XMVECTOR theQuaternionRotation)
//{
//	XMVECTOR axis;
//	float angle;
//	XMFLOAT4 floatAxis;
//	XMQuaternionToAxisAngle(&axis,&angle,theQuaternionRotation);
//	XMStoreFloat4(&floatAxis,axis);
//	rotationVector = Helper::ConvertXMFLOAT4TOVector3(floatAxis) * angle;
//}
//
// void GameObject::SetLocalQuaternionRotationSpeed(Vector3 eulerRotationSpeed)
// {
//	 float x = Helper::DegToRad(eulerRotationSpeed.x);
//	 float y = Helper::DegToRad(eulerRotationSpeed.y);
//	 float z = Helper::DegToRad(eulerRotationSpeed.z);
//	 XMStoreFloat4(&transform.localRotationSpeedQuaternion,XMQuaternionRotationRollPitchYaw(x,y,z));
// }
//
// void GameObject::SetWorldQuaternionRotationSpeed(Vector3 eulerRotationSpeed)
// {
//	 float x = Helper::DegToRad(eulerRotationSpeed.x);
//	 float y = Helper::DegToRad(eulerRotationSpeed.y);
//	 float z = Helper::DegToRad(eulerRotationSpeed.z);
//	 XMStoreFloat4(&transform.worldRotationSpeedQuaternion,XMQuaternionRotationRollPitchYaw(x,y,z));
// }
//
//void GameObject::IncrementRotation( float timeDelta )
//{
//	m_IncrementLocalRotation(timeDelta);
//	m_IncremenWorldRotation(timeDelta);
//}
//
//void GameObject::m_IncrementLocalRotation(float timeDelta)
//{
//	// Determine how much of the rotation speed we have to go.
//	// In 1 second, we would move an angle of m_pqRotationSpeed.
//	// This is from the "origin" (or the "unit" quaternion)
//	XMVECTOR qIdentity = XMQuaternionIdentity();
//	XMVECTOR qRotationSpeed 
//		= XMVectorSet( this->transform.localRotationSpeedQuaternion.x, this->transform.localRotationSpeedQuaternion.y, 
//		this->transform.localRotationSpeedQuaternion.z, this->transform.localRotationSpeedQuaternion.w );
//	XMVECTOR qAdjust 
//		= XMQuaternionSlerp( qIdentity, qRotationSpeed, timeDelta );
//
//	// Now get the current angle... 
//	XMVECTOR qCurrentRotation 
//		= XMVectorSet( this->transform.localQuaternionRotation.x, this->transform.localQuaternionRotation.y, 
//		this->transform.localQuaternionRotation.z, this->transform.localQuaternionRotation.w );
//
//	// Do the actual transformation...
//	XMVECTOR qResult = XMQuaternionMultiply( qCurrentRotation, qAdjust );
//	qResult = XMQuaternionNormalize( qResult );
//
//	// Save it back
//	XMStoreFloat4( &(this->transform.localQuaternionRotation), qResult );
//	transfrom.SetEulerRotationFromQuaternion(this->transform.localEulerRotation,qResult);
//}
//
//void GameObject::m_IncremenWorldRotation(float timeDelta)
//{
//	// Determine how much of the rotation speed we have to go.
//	// In 1 second, we would move an angle of m_pqRotationSpeed.
//	// This is from the "origin" (or the "unit" quaternion)
//	XMVECTOR qIdentity = XMQuaternionIdentity();
//	XMVECTOR qRotationSpeed 
//		= XMVectorSet( this->transform.worldRotationSpeedQuaternion.x, this->transform.worldRotationSpeedQuaternion.y, 
//		this->transform.worldRotationSpeedQuaternion.z, this->transform.worldRotationSpeedQuaternion.w );
//	XMVECTOR qAdjust 
//		= XMQuaternionSlerp( qIdentity, qRotationSpeed, timeDelta );
//
//	// Now get the current angle... 
//	XMVECTOR qCurrentRotation 
//		= XMVectorSet( this->transform.worldQuaternionRotation.x, this->transform.worldQuaternionRotation.y, 
//		this->transform.worldQuaternionRotation.z, this->transform.worldQuaternionRotation.w );
//
//	// Do the actual transformation...
//	XMVECTOR qResult = XMQuaternionMultiply( qCurrentRotation, qAdjust );
//	qResult = XMQuaternionNormalize( qResult );
//
//	// Save it back
//	XMStoreFloat4( &(this->transform.worldQuaternionRotation), qResult );
//	transfrom.SetEulerRotationFromQuaternion(this->transform.worldEulerRotation,qResult);
//}