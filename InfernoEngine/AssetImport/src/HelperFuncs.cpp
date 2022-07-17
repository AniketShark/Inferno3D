#include "HelperFuncs.h"

/////////////////////////////////////////////////
// Written by Daniel Brewer
// djbrewer@fanshawec.ca
// Use this code at your own risk. It is indented only as a learning aid.

FBXConstantBuffer          g_cb;

DirectX::XMMATRIX BonePose::GetTransform() const
{
    DirectX::XMMATRIX m = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
        DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
        DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
    return(m);
}

