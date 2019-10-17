#include "HelperFuncs.h"

/////////////////////////////////////////////////
// Written by Daniel Brewer
// djbrewer@fanshawec.ca
// Use this code at your own risk. It is indented only as a learning aid.

FBXConstantBuffer          g_cb;

XMMATRIX BonePose::GetTransform() const
{
    XMMATRIX m = XMMatrixScaling(scale.x, scale.y, scale.z) *
                 XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
                 XMMatrixTranslation(translation.x, translation.y, translation.z);
    return(m);
}

