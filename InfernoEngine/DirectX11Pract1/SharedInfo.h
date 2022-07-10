////////////////////////////////////////////////////////////////////////// 
// Inferno Engine Project
// Created by Aniket Kayande
// Copyright(c) 2013 - 2014
//////////////////////////////////////////////////////////////////////////
#ifndef SharedInfo_H_
#define SharedInfo_H_


#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"

class SharedInfo
{
public:
	Transform* transform;
	Renderer* renderer;
	Camera* camera;
	Light* light;
	SharedInfo()
	{
		transform = NULL;
		renderer = NULL;
		camera = NULL;
		light = NULL;
	}
	~SharedInfo()
	{

	}

};


#endif
