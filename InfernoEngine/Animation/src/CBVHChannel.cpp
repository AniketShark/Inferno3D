#include "CBVHChannel.h"

CBVHChannel::CBVHChannel()
{
	this->name = L"";
	this->absChannelOffset = UINT_MAX;
}


CBVHChannel::~CBVHChannel()
{
}

bool CBVHChannel::CopyFrameInfoToChannel( int channelNumber, std::vector< float > vecFrameData,
										  std::vector< CBVHChannel >::iterator itStartChan, 
										  std::vector< CBVHChannel >::iterator itEndChan )
{
	std::vector< CBVHChannel >::iterator itChannel;	
	for ( itChannel = itStartChan; itChannel != itEndChan; itChannel++ )
	{	// Is this the channel?
		if ( itChannel->absChannelOffset == channelNumber )
		{	// Yup. Copy the frame data...
			//std::wcout << L"copying frame data for channel: " << channelNumber << " " << itChannel->jointName << ":" << itChannel->name << std::endl;
			itChannel->frameData.clear();
			std::vector< float >::iterator itFD;
			for ( itFD = vecFrameData.begin(); itFD != vecFrameData.end(); itFD++ )
			{
				itChannel->frameData.push_back( *itFD );
			}
			return true;
		}//if ( itChannel->absChannelOffset == channelNumber )
	}//for ( itChannel = itStartChan; itChannel != itEndChan; itChannel++ )
	// If we are here, we didn't find the appropriate channel...
	return false;
}

