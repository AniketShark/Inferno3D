#ifndef WorldCell_H_
#define WorldCell_H_

#include <windows.h>
#include <vector>
#include "GameObject.h"
class WorldCell
{
public:
	WorldCell();
	~WorldCell();

	HANDLE mThreadHandle;
	DWORD mThreadCount;
	LPDWORD mThreadId;

	int mHashKey;

	std::vector<GameObject*> mCollidersInCell;

	void UpdateColliderList();
	
	DWORD WINAPI CellUpdate(PVOID param);
};

#endif