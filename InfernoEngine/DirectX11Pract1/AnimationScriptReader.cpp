

#include "AnimationScriptReader.h"
#include "AnimationCommand.h"
#include <fstream>
#include <iostream>
#include <string>


AnimationScriptReader::AnimationScriptReader(){}
AnimationScriptReader::~AnimationScriptReader(){}

//bool AnimationScriptReader::LoadAnimationScriptFile(std::string fileName,Animation &Obj)
//{
//	std::ifstream animationScript(fileName);
//
//	if(!animationScript.is_open())
//	{
//		return false;
//	}
//	std::string temp;
//	while(temp != "</Animation>")
//	{
//		animationScript>>temp;
//		if(temp == "<Command>")
//		{
//			std::string commadName;
//			animationScript>>commadName;
//			if(commadName == "MoveTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//				animationScript>>moveToCommand.m_destination.x;
//				animationScript>>moveToCommand.m_destination.y;
//				animationScript>>moveToCommand.m_destination.z;
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//				if(temp == "easeOut" || temp == "easeIn")
//				{
//					moveToCommand.m_accelerationType = temp;
//					animationScript>>moveToCommand.m_accelerationSteps;
//				}
//				
//
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//			else if(commadName == "RotateTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//				animationScript>>moveToCommand.m_targetRotation.x; 
//				animationScript>>moveToCommand.m_targetRotation.y;
//				animationScript>>moveToCommand.m_targetRotation.z;
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//
//				if(temp == "easeOut" || temp == "easeIn")
//				{
//					moveToCommand.m_accelerationType = temp;
//					animationScript>>moveToCommand.m_accelerationSteps;
//				}
//
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//			else if(commadName == "ScaleTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//				animationScript>>moveToCommand.m_targetScale.x;
//				animationScript>>moveToCommand.m_targetScale.y;
//				animationScript>>moveToCommand.m_targetScale.z;
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//
//				if(temp == "easeOut" || temp == "easeIn")
//				{
//					moveToCommand.m_accelerationType = temp;
//					animationScript>>moveToCommand.m_accelerationSteps;
//				}
//
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//			if(commadName == "MoveAndRotateTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//
//				animationScript>>moveToCommand.m_destination.x;
//				animationScript>>moveToCommand.m_destination.y;
//				animationScript>>moveToCommand.m_destination.z;
//
//				animationScript>>moveToCommand.m_targetRotation.x;
//				animationScript>>moveToCommand.m_targetRotation.y;
//				animationScript>>moveToCommand.m_targetRotation.z;
//
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//			if(commadName == "MoveAndScaleTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//
//				animationScript>>moveToCommand.m_destination.x;
//				animationScript>>moveToCommand.m_destination.y;
//				animationScript>>moveToCommand.m_destination.z;
//
//				animationScript>>moveToCommand.m_targetScale.x;
//				animationScript>>moveToCommand.m_targetScale.y;
//				animationScript>>moveToCommand.m_targetScale.z;
//
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//			if(commadName == "RotateAndScaleTo")
//			{
//				std::string temp;
//				AnimationCommand moveToCommand;
//				moveToCommand.m_command = commadName;
//
//				animationScript>>moveToCommand.m_targetRotation.x;
//				animationScript>>moveToCommand.m_targetRotation.y;
//				animationScript>>moveToCommand.m_targetRotation.z;
//
//				animationScript>>moveToCommand.m_targetScale.x;
//				animationScript>>moveToCommand.m_targetScale.y;
//				animationScript>>moveToCommand.m_targetScale.z;
//
//				animationScript>>moveToCommand.time;
//				animationScript>>temp;
//				Obj.m_CommandList.push_back(moveToCommand);
//			}
//		}
//	}
//	return true;
//}

