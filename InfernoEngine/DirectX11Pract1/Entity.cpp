#include "Entity.h"

/////////////////////////////////////////////////
// Written by Daniel Brewer
// djbrewer@fanshawec.ca
// Use this code at your own risk. It is indented only as a learning aid.

Entity::Entity() :
    mPosition(0.0f, 0.0f, 0.0f),
    mRotation(0.0f, 0.0f, 0.0f, 0.0f)
{
    XMVECTOR r = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
    XMStoreFloat4(&mRotation, r);
}

Entity::~Entity()
{
}

HRESULT Entity::Initialize()
{
    return(S_OK);
}

void Entity::Update(float delta)
{
}

void Entity::Draw()
{
}

XMMATRIX Entity::GetTransform() const
{
    return(XMMatrixRotationQuaternion(XMLoadFloat4(&mRotation)) * XMMatrixTranslationFromVector(XMLoadFloat3(&mPosition)));
    return(XMMatrixIdentity());
}

XMFLOAT3 Entity::GetPosition() const
{
    return(mPosition);
}

void Entity::SetPosition(XMFLOAT3 p)
{
    mPosition = p;
}

XMFLOAT4 Entity::GetRotation() const
{
    return(mRotation);
}

void Entity::SetRotation(XMFLOAT4 r)
{
    mRotation = r;
}
