#ifndef Object_H_
#define Object_H_

//Author : Aniket Kayande
//23.01.14
// Object class is a base class for every Object that is present in the game
//

#include <windows.h>
#include <string>

class CoreObject
{
public:
	std::string mName;
	GUID mObjectID;
	CoreObject();
	~CoreObject();

};

#endif