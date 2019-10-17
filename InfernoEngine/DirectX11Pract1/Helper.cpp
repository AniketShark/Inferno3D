#include "Helper.h"
#include <math.h>




bool Helper::AreEqual(float a , float b)
{
	float EPSILON = 0.2f;
	if (abs( a - b ) <= EPSILON )
	{
		return true;
	}
	return false;
}

float Helper::DegToRad(float angleInDegrees)
{
	float angleRadian;
	angleRadian = angleInDegrees*(PI/180);
	return angleRadian;
}

XMFLOAT3 Helper::Direction(XMFLOAT3 origin, XMFLOAT3 target)
{
	XMFLOAT3 resultVec;

	resultVec.x = target.x - origin.x;
	resultVec.y = target.y - origin.y;
	resultVec.z = target.z - origin.z;

	return resultVec;

}

XMVECTOR Helper::GMathFV(XMFLOAT3& val)
{
	return XMLoadFloat3(&val);	
}

XMFLOAT3 Helper::GMathVF(XMVECTOR& vec)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, vec);
	return val;
}

float Helper::RandomBetween0n1()
{
	return rand() / double(RAND_MAX);
}

float Helper::RandomRange(float min,float max)
{
	float a = (RandomBetween0n1()*max + min);
	return min(a,max);
}


XMMATRIX Helper::InverseTranspose(CXMMATRIX M)
{
	// Inverse-transpose is just applied to normals.  So zero out 
	// translation row so that it doesn't get into our inverse-transpose
	// calculation--we don't want the inverse-transpose of the translation.
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

std::string Helper::UNICODEtoASCII( std::wstring UNICODEstring )
{
	std::stringstream ssReturnASCII;
	for ( std::wstring::iterator itChar = UNICODEstring.begin(); itChar != UNICODEstring.end(); itChar++ )
	{
		char theChar = static_cast<char>( *itChar );
		ssReturnASCII << theChar;
	}
	return ssReturnASCII.str();
}


std::wstring Helper::ASCIItoUNICODE( std::string ASCIIstring )
{
	std::wstringstream ssReturnUNICODE;
	for ( std::string::iterator itChar = ASCIIstring.begin(); itChar != ASCIIstring.end(); itChar++ )
	{
		wchar_t theChar = static_cast<wchar_t>( *itChar );
		ssReturnUNICODE << theChar;
	}
	return ssReturnUNICODE.str();
}

// Vector3 convention helpers
Vector3 Helper::ConvertXMFLOAT3TOVector3(XMFLOAT3 xmVal)
{
	Vector3 tempVec;

	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;
	
	return tempVec;
}
XMFLOAT3 Helper::ConvertVector3ToXMFLOAT3(Vector3 xmVal)
{
	XMFLOAT3 tempVec;

	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;

	return tempVec;
}
XMFLOAT4 Helper::ConvertVector3ToXMFLOAT4(Vector3 xmVal)
{
	XMFLOAT4 tempVec;

	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;
	tempVec.w = 1.0f;

	return tempVec;
}
Vector3 Helper::ConvertXMFLOAT4TOVector3(XMFLOAT4 xmVal)
{
	XMFLOAT3 tempVec;
	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;

	return ConvertXMFLOAT3TOVector3(tempVec);
}

// Vector4 conversion helpers
XMFLOAT3 Helper::ConvertVector4ToXMFLOAT3(Vector4 xmVal)
{
	XMFLOAT3 tempVec;

	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;

	return tempVec;
}
XMFLOAT4 Helper::ConvertVector4ToXMFLOAT4(Vector4 xmVal)
{
	XMFLOAT4 tempVec;

	tempVec.x = xmVal.x;
	tempVec.y = xmVal.y;
	tempVec.z = xmVal.z;
	tempVec.w = xmVal.w;

	return tempVec;
}

std::string Helper::NumberToString( int Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

int Helper::StringToNumber (std::string &Text )
{
	std::istringstream ss(Text);
	int result;
	return ss >> result ? result : 0;
}


void Helper::trim2(std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != std::string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}

std::string Helper::SplitFilename (const std::string& str)
{
	//std::cout << "Splitting: " << str << '\n';
	unsigned found = str.find_last_of("/\\");
	//std::cout << " path: " << str.substr(0,found) << '\n';
	//std::cout << " file: " << str.substr(found+1) << '\n';
	return str.substr(found+1);
}

std::string Helper::GetExtension (const std::string& str)
{
	//std::cout << "Splitting: " << str << '\n';
	unsigned found = str.find_last_of(".");
	//std::cout << " path: " << str.substr(0,found) << '\n';
	//std::cout << " file: " << str.substr(found+1) << '\n';
	return str.substr(found+1);
}