#ifndef _CBVHFile_HG_
#define _CBVHFile_HG_

#include <iostream>

#include "CBVHJoint.h"
#include "CHRTimer.h"
#include "Biped.h"

//class Biped;
class Model;

enum AnimationType
{
	LOOP,
	LOOPBACKWARD,
	ONCE,
	PINGPONG
};
class CBVHFile
{
public:
	CBVHFile();
	virtual ~CBVHFile();

	bool LoadBVHFile( std::wstring fileName, int &errorCode );
	// this is my code
	std::string m_FileName;
	std::wstring TranslateErrorCode( int errorCode );
	CBVHJoint rootJoint;
	//
	int numberOfFrames;
	float frameTime;
	float mElapsedTime;
	bool mIsAnimationDone;
	

	AnimationType mAnimationType;
	int mCurrentFrameNo;
	float getCaptureTime( void );		// frameTime * numberOfFrames
	float getFrameInfo( std::wstring jointName, int frameNumber );	// Returns a particular frame of a particular joint
	float getFrameInfo( int channelNumber, int frameNumber );		// Returns a particular frame from a particular channel
	bool CreateBiped(Model*);
	void SetBipedReference(Biped*);
	void UpdateAnimation();
	void ResetAnimationState();
	void SetAnimationType(AnimationType animationType);
	

private:
	
	// The raw channel data, each containing the frame data (vector index = frame number)
	std::vector< std::vector< float > > vecAllChannelData;	// Contained vector is the frame for each channel
	// Used during parsing. Finds the channel number in the joints, then copies the data to it...
	bool m_CopyFrameInfoToJoint( int channelNumber, std::vector< float > vecFrameData,
								 std::vector< CBVHJoint >::iterator itJointBegin, 
								 std::vector< CBVHJoint >::iterator itJointEnd );

	// This maps the channel information from the hierarchy to the motion portion
	// The value is the column in the motion portion... is used when parsing the data
	unsigned int currentMotionOffset;		// Indicates the number of channels (one more than the index)
	//bool m_FindJoint( int channelNumber, CBVHChannel &channel );		// Finds channel (in joint) by channel number
	
	Biped* m_Biped;
	
	void UpdateJointsData(int frameNo,CBVHJoint&);
};

#endif