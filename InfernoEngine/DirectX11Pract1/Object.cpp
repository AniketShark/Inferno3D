#include "Object.h"


CoreObject::CoreObject()
{
	mName = " ";
	HRESULT hr = CoCreateGuid(&mObjectID);
}

CoreObject::~CoreObject()
{
	
}