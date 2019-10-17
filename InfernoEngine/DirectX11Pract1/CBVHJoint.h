#ifndef _CBVHJoint_HG_
#define _CBVHJoint_HG_

#include "CBVHPoint3f.h"
#include "CBVHChannel.h"
#include <vector>

class CBVHJoint
{
public:
	CBVHJoint();
	~CBVHJoint();
	
	//
	bool isRoot;
	bool isEndSite;		// Indicates that it's an 'End Site' instead of a 'JOINT'
						// 'End Site' nodes only have OFFSETs not child nodes...
	std::wstring name;		// NOTE: End Side nodes don't have names.
	CBVHPoint3f offset;
	std::vector< CBVHChannel > vecChannels;		// Includes name and absolute offset in file
	std::vector< CBVHJoint > vecChildJoints;
	
	// Parsing stuff
	std::vector< std::wstring >::iterator startBracket;	// Used for parsing the data
	std::vector< std::wstring >::iterator endBracket;	// Used for parsing the data
	std::wstring innerText;
	int nestLevel;									
	// Passes the starting and ending brackets, and parses (and processes) all the contained data...
	bool ParseContainedBVHText( std::vector< std::wstring >::iterator startBracket,
								std::vector< std::wstring >::iterator endBracket,
								unsigned int &curMotionOffset, 	// Maps to columns in MOTION portion of file
								bool bIsEndSiteNode,			// Set to true when reading an "End Site" node (with only OFFSET data)
								int &errorCode ); 
private:
	float stof( std::wstring theString );
	int   stoi( std::wstring theString );
};

#endif