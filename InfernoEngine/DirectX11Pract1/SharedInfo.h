////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////
#ifndef SharedInfo_H_
#define SharedInfo_H_


#include "Transform.h"
#include "Animation.h"
#include "Renderer.h"
#include "Camera.h"
#include "Cloth.h"
#include "Light.h"
#include "Collider.h"

class SharedInfo
{
public:
	Transform* transform;
	Renderer* renderer;
	Animation* animation;
	Camera* camera;
	Cloth* cloth;
	Light* light;
	Collider* collider;
	SharedInfo()
	{
		transform = NULL;
		renderer = NULL;
		animation = NULL;
		camera = NULL;
		cloth = NULL;
		light = NULL;
		collider = NULL;
	}
	~SharedInfo()
	{

	}

};


#endif
