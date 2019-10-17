#ifndef Biped_H_
#define Biped_H_
#include <map>
#include "CBVHJoint.h"
#include "Vector.h"
#include <fstream>
class Model;
class Biped
{
public:
	Biped();
	~Biped();
	
	enum JointNames
	{
		BVHJOINT_head,		BVHJOINT_neck,		
		BVHJOINT_lCollar,	BVHJOINT_rCollar,	
		BVHJOINT_lShldr,	BVHJOINT_rShldr,	
		BVHJOINT_lForeArm,	BVHJOINT_rForeArm,
		BVHJOINT_lHand,		BVHJOINT_rHand,	
		BVHJOINT_chest,		
		BVHJOINT_abdomen,	
		BVHJOINT_hip,	
		BVHJOINT_lThigh,	BVHJOINT_rThigh,	
		BVHJOINT_lShin,		BVHJOINT_rShin,
		BVHJOINT_lFoot,		BVHJOINT_rFoot,
		BVHJOINT_UNKNOWN_OR_ERROR
	};

	enum enumJointAxis	{	BVHAXIS_X,	BVHAXIS_Y,	BVHAXIS_Z  };

	std::map<JointNames,Model*> m_JointModelList;

	void RepresentModel(CBVHJoint& joint,Model* parent);

	JointNames GetJointNameTypeFromString(std::wstring jointName);

	void CopyDataToPhysicalJoints(std::wstring jointName,Vector3 position,Vector3 rotation);
	
	void LoadJointMeshMap(std::wstring filePath);

	std::string GetMeshNameByJoint(std::wstring joinName);

	bool mSwitchAnimationImmidiate;
	bool mSwitchAnimationAfterCurrent;

private : 
	std::map<std::wstring,JointNames> m_JointTypeListByName;
	std::map<std::wstring,std::string> m_Joint_MeshToLoad_Map;
	
};



#endif