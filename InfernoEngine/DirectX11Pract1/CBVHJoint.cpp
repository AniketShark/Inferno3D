#include "CBVHJoint.h"
#include "enumBVHErrorCodes.h"

#include <iostream>
#include <sstream>		// for the string to float method

CBVHJoint::CBVHJoint()
{
	this->isRoot = false;
	this->isEndSite = false;
	this->name = L"";
}

CBVHJoint::~CBVHJoint()
{
}

float CBVHJoint::stof( std::wstring theString )
{
	std::wstringstream ss;
	ss << theString;
	float returnVal;
	ss >> returnVal;
	return returnVal;
}

int CBVHJoint::stoi( std::wstring theString )
{
	std::wstringstream ss;
	ss << theString;
	int returnVal;
	ss >> returnVal;
	return returnVal;
}


bool CBVHJoint::ParseContainedBVHText( std::vector< std::wstring >::iterator startBracket,
									   std::vector< std::wstring >::iterator endBracket,
									   unsigned int &curMotionOffset, 						// Maps to columns in MOTION portion of file
									   bool bIsEndSiteNode,
									   int &errorCode ) 
{

	this->startBracket = startBracket;
	this->endBracket = endBracket;
	this->endBracket++;

	// Copy inner text (between brackets)
	std::wstringstream ss;
	while ( this->startBracket != this->endBracket )
	{
		ss << (*this->startBracket) << " ";	(this->startBracket)++;
	}
	this->startBracket = startBracket;
	this->endBracket = endBracket;
	this->innerText = ss.str();

	//std::cout << "----------------------------------------------------------------" << std::endl;
	//std::cout << "processing: " << this->name << " node" << std::endl;
	//std::cout << this->innerText << std::endl << std::endl;

	try 
	{
		// Double-check to see that the start and end brackets are correct
		if ( (*startBracket) != L"{" )	{ throw( BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET ); }
		if ( (*endBracket) != L"}" )		{ throw( BVHErrorCodes::EXPECTED_CLOSED_CURLY_BRACKET ); }

		std::vector< std::wstring >::iterator curTopItem = startBracket;
		std::vector< std::wstring >::iterator curBottomItem = endBracket;

		// OFFSET x y z
		curTopItem++;
		if ( (*curTopItem) != L"OFFSET" )	{ throw( BVHErrorCodes::EXPECTED_OFFSET ); }
		// Next three items are the location offset (x, y, z)
		curTopItem++;	this->offset.x = this->stof( *curTopItem );
		curTopItem++;	this->offset.y = this->stof( *curTopItem );
		curTopItem++;	this->offset.z = this->stof( *curTopItem );
		//
		// If this is an "End Site" node, then we are done parsing...
		if ( bIsEndSiteNode )
		{
			return true;
		}
		//
		// Next is CHANNEL
		curTopItem++;
		if ( (*curTopItem) != L"CHANNELS" )	{ throw( BVHErrorCodes::EXPECTED_CHANNELS ); }
		// Get number of channels
		int numChannels;
		curTopItem++;	numChannels = this->stoi( *curTopItem );
		// Read the channels
		for ( int curChannel = 0; curChannel != numChannels; curChannel++ )
		{
			CBVHChannel tempChannel;
			tempChannel.absChannelOffset = curMotionOffset;
			curMotionOffset++;			// Move to next channel
			curTopItem++;	tempChannel.name = ( *curTopItem );		tempChannel.jointName = this->name;
			this->vecChannels.push_back( tempChannel );
		}
		// Move to next item...
		curTopItem++;
		// Check to see that the start != end (it shouldn't...but you never know)
		if ( curTopItem == curBottomItem )
		{	// We must be done - really, there's likely a problem... but assume we've read everything.
			return true;
		}
		
		// Next we scan through the rest of the file, looking for the words "JOINT" or "End Site"
		//	These mark the locations of the child nodes
		// 
		std::vector< std::wstring >::iterator itCurItem = curTopItem;
		while ( itCurItem != curBottomItem )
		{
			// Find a "JOINT" or "End"?
			if ( ( (*itCurItem) == L"JOINT" ) || ( (*itCurItem) == L"End" ) )
			{	// Yes, so remember this
				CBVHJoint tempJoint;
				// Move to the first bracket...
				if ( (*itCurItem) == L"JOINT" )

				{	// Next item is name of joint
					itCurItem++;
					tempJoint.name = (*itCurItem);					
					// Next is the the open bracket
					itCurItem++;
					if ( (*itCurItem) != L"{" )	{ throw( BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET ); }
				}
				else	// Assume it's an "End"
				{	// Next should be "Site"
					itCurItem++;
					if ( (*itCurItem) != L"Site" )	{ throw( BVHErrorCodes::EXPECTED_End_Site ); }
					// And next should be bracket
					itCurItem++;
					if ( (*itCurItem) != L"{" )	{ throw( BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET ); }
				}
				// 
				tempJoint.startBracket = itCurItem;
				// Now we have to find the MATCHING closed bracket, which may not be the curBottomItem
				// Scan through the rest of the text, looking for the correct number of closing brackets
				int numBrackets = 0;
				//itCurItem++;	// Move to next thing
				bool bKeepSearching = true;
				while ( (itCurItem != curBottomItem) && bKeepSearching )
				{
					if ( (*itCurItem) == L"{" )	
					{
						numBrackets++;
					}
					if ( (*itCurItem) == L"}" )
					{	// Found a bracket, but is it the matching one?
						numBrackets--;
						if ( numBrackets == 0 )
						{	// Yes
							bKeepSearching = false;
						}
					}
					if ( bKeepSearching )	{ itCurItem++; }
				}
				// itCurItem is now pointing to the matching closing bracket...
				tempJoint.endBracket = itCurItem;
				// Save it
				this->vecChildJoints.push_back( tempJoint );
			}//while ( itCurItem != curBottomItem )

			itCurItem++;
		}
		// Did we find anything? (we should or there is something wrong...)
		if ( this->vecChildJoints.empty() )		{ throw( BVHErrorCodes::EXPECTED_JOINT_OR_EndSite ); }

		//
		// At this point, the vector of child nodes all contain nodes with the starting and ending bracket locations
		//	set to the correct location in the vector of characters
		// So now we process them...
		std::vector< CBVHJoint >::iterator itChildNodes; 
		for ( itChildNodes = this->vecChildJoints.begin(); itChildNodes != this->vecChildJoints.end(); itChildNodes++ )
		{
			itChildNodes->ParseContainedBVHText( itChildNodes->startBracket, itChildNodes->endBracket, 
												 curMotionOffset, itChildNodes->isEndSite, errorCode );
		}

		int USE_ME_FOR_BREAKPOINTS = 0;
	}
	catch ( BVHErrorCodes::enumBVHErrorCodes theErrorCode )
	{
		errorCode = theErrorCode;
		return false;
	}

	return true;
}


//
//// Recursively reads the BVH file.
//// First looks for "this" joints offset and channel info, then 
////	continues on to read any child joins (does this by calling method recursively).
//// * errorCode gets passed back if a problem
//// * curMotionOffset is used to map channels to the columns in the motion data
////
//bool CBVHJoint::ReadDataAndChildren( std::vector< std::string >::iterator &itCurBVHItem, 
//									 const std::vector< std::string >::iterator &endOfVector,
//									 int &errorCode, 
//									 unsigned int &curMotionOffset/*, unsigned int &nestLevel*/  )
//{
//	try 
//	{
//		// Read "{"
//		itCurBVHItem++;
//		if ( (*itCurBVHItem) != "{" )	{	throw ( BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET );	};
//		//
//		// Read "OFFSET"
//		itCurBVHItem++;
//		if ( (*itCurBVHItem) != "OFFSET" )	{	throw ( BVHErrorCodes::EXPECTED_OFFSET );	};
//
//		// Now offset information...
//		itCurBVHItem++;		this->offset.x = this->stof( *itCurBVHItem );
//		itCurBVHItem++;		this->offset.y = this->stof( *itCurBVHItem );
//		itCurBVHItem++;		this->offset.z = this->stof( *itCurBVHItem );
//
//		// Read "CHANNELS"
//		itCurBVHItem++;		
//		if ( (*itCurBVHItem) != "CHANNELS" ) {	throw BVHErrorCodes::EXPECTED_CHANNELS;	};
//
//		int numChannels;	
//		itCurBVHItem++;		numChannels = this->stoi( *itCurBVHItem );
//		for ( int chanCount = 0; chanCount != numChannels; chanCount++ )
//		{
//			CBVHChannel curChan;
//			itCurBVHItem++;		curChan.name = (*itCurBVHItem);
//			curChan.absMotionOffset = curMotionOffset;	// Set absolute offset...
//			this->vecChannels.push_back( curChan );		// ...add it to the vector of channels...
//			curMotionOffset++;							// ...and increment for next count
//		}
//
//		bool bReadMoreChildren = true;
//		bool bChildOKToSave = true;
//		do 
//		{
//			// Now we read the child joints...
//			CBVHJoint childJoint;	
//
//			// Now should be either "JOINT" (in which case we recursively read)
//			//	...or "End Site" in which case we return...
//			itCurBVHItem++;
//			// JOINT or End Site
//			if ( (*itCurBVHItem) == "JOINT" )
//			{	// Yup, so 
//				// ...get its name...
//				itCurBVHItem++;		childJoint.name = (*itCurBVHItem);
//				// ...and have it read itself...			
//				if ( !( childJoint.ReadDataAndChildren( itCurBVHItem, endOfVector, errorCode, curMotionOffset ) ) )
//				{	// Carp! Something is wrong...
//					throw ( errorCode );
//				}
//				// Child read, so add to list...
//				//this->vecChildJoints.push_back( childJoint );
//			}
//			else
//			{	// Was it "End"?
//				if ( (*itCurBVHItem) == "End" )
//				{	// Yup. Next one "Site"?
//					itCurBVHItem++;		
//					if ( (*itCurBVHItem) != "Site" )
//					{	// Nope. 
//						throw ( BVHErrorCodes::EXPECTED_JOINT_OR_EndSite );
//					}
//					else
//					{	// Found "End Site" so read offset
//						childJoint.isEndSite = true;
//						childJoint.name = "End Node";
//						// Read "{"
//						itCurBVHItem++;
//						if ( (*itCurBVHItem) != "{" )	{	throw ( BVHErrorCodes::EXPECTED_OPEN_CURLY_BRACKET );	};
//						// Should be "OFFSET"...
//						itCurBVHItem++;
//						if ( (*itCurBVHItem) != "OFFSET" )	{	throw ( BVHErrorCodes::EXPECTED_OFFSET );	};
//						itCurBVHItem++;		childJoint.offset.x = this->stof( *itCurBVHItem );
//						itCurBVHItem++;		childJoint.offset.y = this->stof( *itCurBVHItem );
//						itCurBVHItem++;		childJoint.offset.z = this->stof( *itCurBVHItem );
//						bReadMoreChildren = false;	// Stop reading... no more children...
//						// Read '}' (for 'End Site' NOTE current node...)
//						itCurBVHItem++;
//						if ( (*itCurBVHItem) != "}" )	{	throw ( BVHErrorCodes::EXPECTED_CLOSED_CURLY_BRACKET );	};
//					} // if ( tempVal != "Site" )
//				}
//				else
//				{	// Not "End" or "JOINT"
//					throw ( BVHErrorCodes::EXPECTED_JOINT_OR_EndSite );
//				}//if ( tempVal == "End" )
//			}//if ( tempVal == "JOINT" )
//			// 
//
//			if ( bChildOKToSave )
//			{
//				itCurBVHItem++;
//				if ( (*itCurBVHItem) != "}" )	{	throw ( BVHErrorCodes::EXPECTED_CLOSED_CURLY_BRACKET );	};
//
//				// At this point, we have read all the children and-or found "End Site"
//				std::cout << this->name << " is adding " << childJoint.name << std::endl;
//				this->vecChildJoints.push_back( childJoint );
//			}
//
//			// Check to see if there are more children or we are done.
//
//			// If the next word is "JOINT", then there are more children
//			// If the next word is "}" , then we are done
//			// (Double-check to see if we are at the end of the vector, too, so we don't go off the end)
//			if ( itCurBVHItem == endOfVector )
//			{	// We are done...
//					bReadMoreChildren = false;
//			}
//			else
//			{
//				itCurBVHItem++;
//				if ( (*itCurBVHItem) == "}" ) 
//				{	// We are done...
//					bReadMoreChildren = false;
//				}
//				// Put the last read back...for the next pass
//				itCurBVHItem--;
//			}// if ( itVurBVHItem == endOfVector )
//
//		} while ( bReadMoreChildren );	// Set when we found an 'End Site' child node
//		// All done...
//
//	}
//	catch ( BVHErrorCodes::enumBVHErrorCodes curErrorCode )
//	{
//		errorCode = curErrorCode;
//		return false;
//	}
//
//	return true;
//}


