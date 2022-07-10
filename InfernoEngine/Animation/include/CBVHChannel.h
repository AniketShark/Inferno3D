#ifndef _CBVHChannel_HG_
#define _CBVHChannel_HG_

#include <string> 
#include <vector>

class CBVHChannel
{
public:
	CBVHChannel();
	virtual ~CBVHChannel();
	std::wstring jointName;			// Name of hosting joint 
	std::wstring name;				// Name of channel
	unsigned int absChannelOffset;	// Offset in the "MOTION" portion of the main file
	std::vector< float > frameData;	// Contains the motion data for this channel, indexed by frame
	//
	bool CopyFrameInfoToChannel( int channelNumber, std::vector< float > vecFrameData,
								 std::vector< CBVHChannel >::iterator itStartChan, 
								 std::vector< CBVHChannel >::iterator itEndChan );
};

#endif