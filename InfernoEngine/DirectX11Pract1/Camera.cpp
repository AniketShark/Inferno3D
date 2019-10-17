
#include "Camera.h"
#include "Helper.h"
#include "global.h"
#include "SharedInfo.h"

Camera::Camera()
{
	Eye = XMFLOAT3(0,10,0);
	View = XMFLOAT3(0,0,0);
	Up = XMFLOAT3(0,1,0);

	mThisGO = new SharedInfo;
	mLookAtTransform = NULL;
}
Camera::~Camera()
{
	if(mThisGO)
		delete mThisGO;
}


//Sets the Camera to given Position
void Camera::SetCamera(float posX,float posY,float posZ)
{
	Eye.x = posX;
	Eye.y = posY;
	Eye.z = posZ;
}
//Sets the look at point
void Camera::LookAtPoint(float posX,float posY,float posZ)
{
	View.x = posX;
	View.y = posY;
	View.z = posZ;
}
// Moves the camera by deltaX,deltaY,deltaZ
void Camera::MoveCamera(float deltaX,float deltaY,float deltaZ)
{
	Eye.x += deltaX;
	Eye.y += deltaY;
	Eye.z += deltaZ;

	View.x += deltaX;
	View.y += deltaY;
	View.z += deltaZ;
}

void Camera::MoveCamera(Vector3 direction)
{
	Eye.x += direction.x;
	Eye.y += direction.y;
	Eye.z += direction.z;

	View.x += direction.x;
	View.y += direction.y;
	View.z += direction.z;
}

void Camera::CameraPitchYawRoll(float pitch, float yaw, float roll)
{
	
}

void Camera::MoveAroundPoint(Vector3 point,float distance,float moveSpeed)
{
	
}
void Camera::SetLookAtTransform(Transform* transform)
{
	mLookAtTransform = transform;
}

void Camera::Initialize()
{
	if(mThisGO->transform != NULL)
		mThisGO->transform->position = Vector3::Forward*-10;
}
void Camera::Update()
{
	if(mLookAtTransform != NULL)
		View = Helper::ConvertVector3ToXMFLOAT3(mLookAtTransform->position);

	Eye = Helper::ConvertVector3ToXMFLOAT3(mThisGO->transform->position);

	g_pGlobalConstantBuffer.mEyePosition.x = Eye.x;
	g_pGlobalConstantBuffer.mEyePosition.y = Eye.y;
	g_pGlobalConstantBuffer.mEyePosition.z = Eye.z;
	g_pGlobalConstantBuffer.mEyePosition.w = 1.0f;

	g_View = XMMatrixLookAtLH(Helper::GMathFV(Eye) , Helper::GMathFV(View),  Helper::GMathFV(Up));

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, 1024 / (FLOAT)768, 1.0f, 100000.0f );
	
}
void Camera::Destroy()
{

}
ComponentType Camera::GetType()
{
	return CameraComp;
}

IBehaviour* Camera::Clone()
{
	Camera* instance = new Camera;
	instance->mThisGO = mThisGO;
	return instance;
}

