#include "CBVHFile.h"
#include "enumBVHErrorCodes.h"
#include <fstream>
#include <iostream>
#include <algorithm>

//// Local to this file only...
//// Attempts to copy frame data to the corresponding channel. If found (and copied), returns true
//bool CopyFrameInfoToChannelFILELOCAL( int channelNumber, std::vector< float > vecFrameData,
//								      std::vector< CBVHChannel >::iterator itStartChan, 
//									  std::vector< CBVHChannel >::iterator itEndChan );


CBVHFile::CBVHFile()
{
	// This maps the channel information from the hierarchy to the motion portion
	// The value is the column in the motion portion...
	this->currentMotionOffset = 0;
	this->mElapsedTime = 0;
	this->mCurrentFrameNo = 0;
}

CBVHFile::~CBVHFile()
{
	if(m_Biped)
		delete m_Biped;
}

bool CBVHFile::LoadBVHFile( std::wstring fileName, int &errorCode )
{
	errorCode = BVHErrorCodes::OK;

	// There are two parts to the file, the HIERARCHY portion and the MOTION portion
	enum enumReadState
	{
		UNKNOWN,
		READING_HIERARCHY_PORTION,
		READING_MOTION_PORTION
	} currentReadState = UNKNOWN;

	// Open file
	std::wifstream theBVHFile( fileName.c_str() );
	if ( !theBVHFile.is_open() )
	{
		errorCode = BVHErrorCodes::CANT_FIND_OR_OPEN_FILE;
		return false;
	}
	// File is open
	// First line should be "HIERARCHY"
	try
	{
		std::wstring tempVal;		
		theBVHFile >> tempVal;
		if ( tempVal != L"HIERARCHY" )	{	throw BVHErrorCodes::EXPECTED_HIERARCHY_BUT_GOT_UNKNOWN; }
		// 
		currentReadState = READING_HIERARCHY_PORTION;

		std::vector< std::wstring > vecBVHItems;	// Holds all the items in the hiearchy portion
		// Read until we hit the word "MOTION"
		while (true)
		{
			theBVHFile >> tempVal;
			if ( tempVal == L"MOTION" )	
				break;
			vecBVHItems.push_back( tempVal );
		}
		// 
		// At this point, vecBVHItems contains all the strings of the HIEARCHY portion of the file, 
		//	from the "ROOT" to the ending "}" bracket.

		// 

		std::vector< std::wstring >::iterator startBracket = vecBVHItems.begin();	// place at start of vector.
		std::vector< std::wstring >::iterator endBracket = vecBVHItems.end();	// place at end of vector.

		// Next should be "ROOT"
		if ( (*startBracket) != L"ROOT" )	{ throw BVHErrorCodes::EXPECTED_ROOT_BUT_GOT_UNKNOWN;	}
			
		// Next is name of root node
		startBracket++;	
		this->rootJoint.name = (*startBracket);	
		this->rootJoint.isRoot = true;

		// Next item is the starting bracket for this section
		startBracket++;
		// Position end bracket to one before the end of the vector
		endBracket--;
		// 
		// Have node process itself...
		if ( ! (this->rootJoint.ParseContainedBVHText( startBracket, endBracket, 
													   this->currentMotionOffset, false, errorCode ) ) )
		{
			throw ( errorCode );
		}
		
		// ***********************************************
		// Now process the MOTION portion of the file...
		currentReadState = READING_MOTION_PORTION;

		// We switch back to reading directly from the file stream...
		// First item should be "Frames:" 
		theBVHFile >> tempVal;
		if ( tempVal != L"Frames:" )	{ throw( BVHErrorCodes::EXPECTED_FRAMES_BUT_GOT_SOMETHING_ELSE ); }
		theBVHFile >> this->numberOfFrames;

		// Now 'Frame'
		theBVHFile >> tempVal;
		if ( tempVal != L"Frame" )	{ throw( BVHErrorCodes::EXPECTED_FRAME_BUT_GOT_SOMETHING_ELSE ); }
		// Now 'Time:'
		theBVHFile >> tempVal;
		if ( tempVal != L"Time:" )	{ throw( BVHErrorCodes::EXPECTED_TIME_BUT_GOT_SOMETHING_ELSE ); }
		theBVHFile >> this->frameTime;
		
		// Now we go through all the channel information, placing the capture data into the 
		//	corresponding frame.
		//
		// Here's how this works:
		// 
		// Each joint has a vector of channels in it. Each channel contains 'absChannelOffset' which 
		//	is the index of the channel in this portion of the file.
		// Each channel also has a vector of floats, which is the actual frame data, using the 
		//	index of the float vector as the frame number. 
		//
		// As we read the motion capture datum, we find the corresponding channel in the joint
		//	(using the 'absChannelOffset' number) and place the data into the vector.
		//
		
		// This holds all the frame information (each frame is a vector of floats)
		//std::vector< std::vector< float > > vecChannels;	// Contained vector is the frame for each channel

		for ( int curChannel = 0; curChannel != this->currentMotionOffset; curChannel++ )
		{ 
			std::vector< float > tempVector;
			this->vecAllChannelData.push_back( tempVector );
		}
		// At this point, there are the correct number of channels, each with NO frames in them

		for ( int curFrame = 0; curFrame != this->numberOfFrames; curFrame++ )
		{
			std::vector< std::vector< float > >::size_type curChannelIndex;
			for ( curChannelIndex = 0; curChannelIndex != this->currentMotionOffset; curChannelIndex++ )
			{
				float curData;
				theBVHFile >> curData;
				// Add a frame for this channel
				this->vecAllChannelData.at(curChannelIndex).push_back( curData );
			}
		}
		// At this point, all the MOTION data has been placed into a vector of vectors,
		//	where the 'outer' vector contains the channel information and the 'inner'
		//	vectors contain the frame data (per channel).
		
		// Now place this channel data into the Joints... (where it's most useful)
		for ( int curChannelIndex = 0; curChannelIndex != this->currentMotionOffset; curChannelIndex++ )
		{
			// First try the root joint...
			CBVHChannel tempChannelForCopy;
			bool DataCopied = 
				tempChannelForCopy.CopyFrameInfoToChannel( curChannelIndex, this->vecAllChannelData.at(curChannelIndex ),
												           this->rootJoint.vecChannels.begin(), this->rootJoint.vecChannels.end() );
			if ( !DataCopied )
			{	// Try recursive check to find joint... 
				DataCopied = this->m_CopyFrameInfoToJoint( curChannelIndex, this->vecAllChannelData.at( curChannelIndex), 
														   this->rootJoint.vecChildJoints.begin(), this->rootJoint.vecChildJoints.end() );
			}
			// Double check that it copied OK... (it should have been or something is very wrong)
			if ( !DataCopied )	
			{ 
				throw( BVHErrorCodes::CAN_NOT_INDEX_THE_CHANNEL_DATA ); 
			}			
		}

		int USE_ME_FOR_BREAKPOINTS = 0;
	}
	catch ( BVHErrorCodes::enumBVHErrorCodes theError )
	{
		errorCode = theError;		return false;
	}
	
	return true;
}


//bool CopyFrameInfoToChannelFILELOCAL( int channelNumber, std::vector< float > vecFrameData,
//								      std::vector< CBVHChannel >::iterator itStartChan, 
//									  std::vector< CBVHChannel >::iterator itEndChan )
//{
//	std::vector< CBVHChannel >::iterator itChannel;	
//	for ( itChannel = itStartChan; itChannel != itEndChan; itChannel++ )
//	{	// Is this the channel?
//		if ( itChannel->absChannelOffset == channelNumber )
//		{	// Yup. Copy the frame data...
//			std::wcout << L"copying frame data for channel: " << channelNumber << " " << itChannel->jointName << ":" << itChannel->name << std::endl;
//			itChannel->frameData.clear();
//			std::vector< float >::iterator itFD;
//			for ( itFD = vecFrameData.begin(); itFD != vecFrameData.end(); itFD++ )
//			{
//				itChannel->frameData.push_back( *itFD );
//			}
//			return true;
//		}//if ( itChannel->absChannelOffset == channelNumber )
//	}//for ( itChannel = itStartChan; itChannel != itEndChan; itChannel++ )
//	// If we are here, we didn't find the appropriate channel...
//	return false;
//}




bool CBVHFile::m_CopyFrameInfoToJoint( int channelNumber, std::vector< float > vecFrameData,
									   std::vector< CBVHJoint >::iterator itJointBegin, 
										   std::vector< CBVHJoint >::iterator itJointEnd )
{
	bool bDataCopied = false;
	std::vector< CBVHJoint >::iterator itJoint;
	for ( itJoint = itJointBegin; itJoint != itJointEnd; itJoint++ )
	{
		CBVHChannel tempChannelForCopy;
		bDataCopied = tempChannelForCopy.CopyFrameInfoToChannel( 
							channelNumber, vecFrameData, 
							itJoint->vecChannels.begin(), itJoint->vecChannels.end() );

		if ( !bDataCopied )
		{	// Try the contained joints... (recursive)
			bDataCopied = this->m_CopyFrameInfoToJoint( channelNumber, vecFrameData, 
														itJoint->vecChildJoints.begin(), itJoint->vecChildJoints.end() );
		}
		if ( bDataCopied )
		{	// No need to keep searching
			break;
		}
	}
	// If here, then the correct channel was not found in this list
	return bDataCopied;
}

// Returns a particular frame from a particular channel
float CBVHFile::getFrameInfo( int channelNumber, int frameNumber )
{
	if ( channelNumber > static_cast<int>( this->vecAllChannelData.size() ) )
	{
		return 0;
	}
	if ( frameNumber > this->numberOfFrames )
	{
		return 0;
	}
	return this->vecAllChannelData.at( channelNumber ).at( frameNumber );
}

std::wstring CBVHFile::TranslateErrorCode( int errorCode )
{
	switch ( errorCode )
	{
	case BVHErrorCodes::OK:
		return L"OK. No error.";
		break;
	case BVHErrorCodes::CANT_FIND_OR_OPEN_FILE:
		return L"Can't find or open file.";
		break;
	case BVHErrorCodes::EXPECTED_HIERARCHY_BUT_GOT_UNKNOWN:
		return L"Expected 'HIERARCHY' node, but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_ROOT_BUT_GOT_UNKNOWN:
		return L"Expected 'ROOT' node, but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET:
		return L"Expected open curly bracket '{' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_CLOSED_CURLY_BRACKET:
		return L"Expected closed curly bracket '}' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_OFFSET:
		return L"Expected 'OFFSET' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_CHANNELS:
		return L"Expected 'CHANNELS' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_JOINT_OR_EndSite:
		return L"Expected 'JOINT' or 'End Site' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_End_Site:
		return L"Expected 'End Site' but got something else.";
		break;
	//
	case BVHErrorCodes::EXPECTED_FRAMES_BUT_GOT_SOMETHING_ELSE:
		return L"Expected 'Frames:' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_FRAME_BUT_GOT_SOMETHING_ELSE:
		return L"Expected 'Frame' but got something else.";
		break;
	case BVHErrorCodes::EXPECTED_TIME_BUT_GOT_SOMETHING_ELSE:
		return L"Expected 'Time' but got something else.";
		break;
	//
	case BVHErrorCodes::CAN_NOT_INDEX_THE_CHANNEL_DATA:
		return L"Fatal error: Can not index the channel data.";
		break;
	//
	case BVHErrorCodes::GENERAL_OR_UNKNOWN_ERROR:
		return L"General or unknown error.";
		break;
	}
	return L"General or unknown error.";
}



void CBVHFile::UpdateAnimation()
{

	if(this->mAnimationType == AnimationType::LOOP)
	{
		if(mElapsedTime >= frameTime)
		{
			UpdateJointsData(mCurrentFrameNo,this->rootJoint);
			mCurrentFrameNo += 1;
			
			if(mCurrentFrameNo >= numberOfFrames)
			{
				this->mIsAnimationDone = true;
				mCurrentFrameNo = 0;
			}
			mElapsedTime = 0;
		}
	}

	if(this->mAnimationType == AnimationType::LOOPBACKWARD)
	{
		if(mElapsedTime >= frameTime)
		{
			UpdateJointsData(mCurrentFrameNo,this->rootJoint);
			mCurrentFrameNo -= 1;
			if(mCurrentFrameNo <= 0)
			{
				mCurrentFrameNo = numberOfFrames - 1;
				mIsAnimationDone = true;
			}
			mElapsedTime = 0;
		}
		
	}

	if(this->mAnimationType == AnimationType::ONCE)
	{
		if(mElapsedTime >= frameTime && !this->mIsAnimationDone)
		{
			UpdateJointsData(mCurrentFrameNo,this->rootJoint);
			mCurrentFrameNo += 1;
			if(mCurrentFrameNo <= 0)
			{
				mCurrentFrameNo = 0;
				this->mIsAnimationDone = true;
			}
			mElapsedTime = 0;
		}
	}

	if(this->mAnimationType == AnimationType::LOOP || this->mAnimationType == AnimationType::LOOPBACKWARD || !this->mIsAnimationDone)
	{
		if(this->mAnimationType == AnimationType::LOOP || this->mAnimationType == AnimationType::LOOPBACKWARD)
		{
			if(!m_Biped->mSwitchAnimationAfterCurrent)  
				ResetAnimationState();
		}

		mElapsedTime += mGlobalDeltaTime;
	} 
}


void CBVHFile::UpdateJointsData(int frameNo,CBVHJoint& parentJoint)
{
	Vector3 pos = Vector3::Zero;
	Vector3 rotation = Vector3::Zero;

	if(parentJoint.isRoot)
	{
		pos.x = parentJoint.vecChannels[0].frameData[frameNo];
		pos.y = parentJoint.vecChannels[1].frameData[frameNo];
		pos.z = parentJoint.vecChannels[2].frameData[frameNo];

		//pos = Vector3::Zero;

		rotation.x = parentJoint.vecChannels[3].frameData[frameNo];
		rotation.y = parentJoint.vecChannels[4].frameData[frameNo];
		rotation.z = parentJoint.vecChannels[5].frameData[frameNo];

		m_Biped->CopyDataToPhysicalJoints(parentJoint.name,pos,rotation);
	}
	else
	{
		if(parentJoint.name != L"")
		{
			rotation.x = parentJoint.vecChannels[0].frameData[frameNo];
			rotation.y = parentJoint.vecChannels[1].frameData[frameNo];
			rotation.z = parentJoint.vecChannels[2].frameData[frameNo];
		}

		pos.x = parentJoint.offset.x;
		pos.y = parentJoint.offset.y;
		pos.z = parentJoint.offset.z;

		m_Biped->CopyDataToPhysicalJoints(parentJoint.name,pos,rotation);
	}
	
	if(!parentJoint.vecChildJoints.empty())
	{
		for (std::vector<CBVHJoint>::iterator jointIt = parentJoint.vecChildJoints.begin(); jointIt != parentJoint.vecChildJoints.end();jointIt++)
		{
			UpdateJointsData(frameNo,(*jointIt));
		}
	}
}

void CBVHFile::SetBipedReference(Biped* mainBiped)
{
	m_Biped = mainBiped;
}


void CBVHFile::ResetAnimationState()
{
	mIsAnimationDone = false;
}

void CBVHFile::SetAnimationType(AnimationType animationType)
{
	if(animationType == AnimationType::LOOPBACKWARD)
		mCurrentFrameNo = numberOfFrames - 1;

	mAnimationType = animationType;
}


