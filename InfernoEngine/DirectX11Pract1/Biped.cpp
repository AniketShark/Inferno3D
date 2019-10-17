#include "Biped.h"
#include "GameObjectManager.h"

Biped::Biped()
{
	m_JointTypeListByName[L"hip"] = BVHJOINT_hip;
	m_JointTypeListByName[L"abdomen"] = BVHJOINT_abdomen;
	m_JointTypeListByName[L"chest"] = BVHJOINT_chest;
	m_JointTypeListByName[L"neck"] = BVHJOINT_neck;
	m_JointTypeListByName[L"head"] = BVHJOINT_head;

	m_JointTypeListByName[L"lShldr"] = BVHJOINT_lShldr;
	m_JointTypeListByName[L"lForeArm"] = BVHJOINT_lForeArm;
	m_JointTypeListByName[L"lHand"] = BVHJOINT_lHand;
	m_JointTypeListByName[L"lThigh"] =BVHJOINT_lThigh;
	m_JointTypeListByName[L"lShin"] = BVHJOINT_lShin;
	m_JointTypeListByName[L"lFoot"] = BVHJOINT_lFoot;

	m_JointTypeListByName[L"rCollar"] = BVHJOINT_rCollar;
	m_JointTypeListByName[L"rShldr"] = BVHJOINT_rShldr;
	m_JointTypeListByName[L"rForeArm"] = BVHJOINT_rForeArm;
	m_JointTypeListByName[L"rHand"] = BVHJOINT_rHand;
	m_JointTypeListByName[L"rThigh"] =BVHJOINT_rThigh;
	m_JointTypeListByName[L"rShin"] = BVHJOINT_rShin;
	m_JointTypeListByName[L"rFoot"] = BVHJOINT_rFoot;

	mSwitchAnimationAfterCurrent = false;
	mSwitchAnimationImmidiate = false;
}
Biped::~Biped()
{

}

void Biped::RepresentModel(CBVHJoint& joint,Model* parent)
{
	/*Vector3 pos = Vector3(joint.offset.x,joint.offset.y,joint.offset.z);

	std::string meshName = GetMeshNameByJoint(joint.name);

	if(meshName == "")
		return;
	Model* cube  =  CreateObjectByModelName(meshName,pos,Vector3::Zero,Vector3::One);

	cube->Tag = std::string(joint.name.begin(),joint.name.end());

	m_JointModelList[GetJointNameTypeFromString(joint.name)] = cube;

	parent->mChildObjectsList.push_back(cube);

	if(!joint.vecChildJoints.empty())
	{
		for (std::vector<CBVHJoint>::iterator jointIt = joint.vecChildJoints.begin();jointIt != joint.vecChildJoints.end();jointIt++)
		{
			RepresentModel((*jointIt),cube);
		}
	}*/
}

Biped::JointNames Biped::GetJointNameTypeFromString(std::wstring jointName)
{
	return m_JointTypeListByName[jointName];
}


void Biped::CopyDataToPhysicalJoints(std::wstring jointName,Vector3 position,Vector3 rotation)
{
	Model* joint = m_JointModelList[GetJointNameTypeFromString(jointName)];
	joint->transform.localEulerRotation = rotation;
	joint->transform.SetLocalRotationQuaternion(rotation);
	joint->transform.position = position;
}

void Biped::LoadJointMeshMap(std::wstring filePath)
{
	std::ifstream file(filePath);

	if(!file.is_open())
	{
		MessageBox(NULL,L"Error Reading JointToMesh Map",L"Error",MB_OK);
		return;
	}

	std::string tempString;

	while(tempString != "End")
	{
		file>>tempString;

		if(tempString == "BVHJOINT")
		{
		    std::string jointName;
			std::string meshName;
			file>>jointName;
			file>>meshName;
			std::wstring str = std::wstring(jointName.begin(),jointName.end());
			m_Joint_MeshToLoad_Map[str] = meshName;
		}
	}

	file.close();
}

std::string Biped::GetMeshNameByJoint(std::wstring jointName)
{
	//return m_Joint_MeshToLoad_Map[jointName];


		if(jointName == L"hip")
		{
			return "Hip.ply";
		}		
		if(jointName == L"abdomen")
		{
			return "Abdomen.ply";
		}			
		if(jointName == L"chest")
		{
			return "Chest.ply";
		}	
		if(jointName == L"neck")
		{
			return "Neck.ply";
		}		
		if(jointName == L"head")
		{
			return "Head.ply";
		}
		if(jointName == L"lCollar")
		{
			return "UVSphere.ply";
		}
		if(jointName == L"lShldr")
		{
			return "LeftSholder.ply";
		}
		if(jointName == L"lForeArm")
		{
			return "LeftArm.ply";
		}
		if(jointName == L"lHand")
		{
			return "LeftHand.ply";
		}
		if(jointName == L"rCollar")
		{
			return "UVSphere.ply";
		}
		if(jointName == L"rShldr")
		{
			return "RightSholder.ply";
		}
		if(jointName == L"rForeArm")
		{
			return "RightArm.ply";
		}
		if(jointName == L"rHand")
		{
			return "RightHand.ply";
		}
		if(jointName == L"lThigh")
		{
			return "Thigh.ply";
		}
		if(jointName == L"lShin")
		{
			return "Shin.ply";
		}
		if(jointName == L"lFoot")
		{
			return "Foot.ply";
		}
		if(jointName == L"rThigh")
		{
			return "Thigh.ply";
		}
		if(jointName == L"rShin")
		{
			return "Shin.ply";
		}
		
		if(jointName == L"rFoot")
		{
			return "Foot.ply";
		}

		if(jointName == L"")
		{
			return "UVSphere.ply";
		}
	
	return "UVSphere.ply";
}


