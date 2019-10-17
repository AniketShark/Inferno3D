#include "SkinningInterface.h"
#include "GameObjectManager.h"

SkinningData::SkinningData()
{
	
}
SkinningData::~SkinningData()
{

}


void SkinningData::CreateSkinnedMeshFromFile(std::wstring fileName)
{
	std::wifstream file(fileName);

	if(file.is_open())
	{
		std::wstring jointName;
		int originalIndex, parentIndex;
		XMFLOAT4 position,rotation;
		while (file>>jointName)
		{
			Joint tempJoint;

			file>>originalIndex;

			file>>position.x;
			file>>position.y;
			file>>position.z;
			position.w = 1.0f;

			file>>rotation.x;
			file>>rotation.y;
			file>>rotation.z;
			rotation.w = 1.0f;

			file>>parentIndex;

			tempJoint.name = jointName;
			tempJoint.parent = parentIndex;
			tempJoint.position = position;
			tempJoint.rotation = rotation;

			m_ListOfJoints.push_back(tempJoint);
		}
	}
}

void CreateSkinnedMeshFromJoints(Joint* joints,int noOfJoints)
{

}
