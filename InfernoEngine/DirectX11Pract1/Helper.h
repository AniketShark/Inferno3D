

#ifndef Helper_H_  
#define Helper_H_

#include <windows.h>
#include<xnamath.h>
#include "Vector.h"
#include <stdlib.h>
#include <sstream>
#define PI 3.14159f

class Helper
{
public:
	static bool AreEqual(float, float);
	static float DegToRad(float AngleInDegrees);
	static XMFLOAT3 Direction(XMFLOAT3 origin, XMFLOAT3 taregt);
	static XMVECTOR GMathFV(XMFLOAT3& val);
	// Taken directly from Frank Luna's DirectX 11 book....
	static XMMATRIX InverseTranspose(CXMMATRIX M);
	static XMFLOAT3 GMathVF(XMVECTOR& vec);
	static float RandomBetween0n1();
	static float RandomRange(float min,float max);
	static std::string UNICODEtoASCII( std::wstring UNICODEstring );
	static std::wstring ASCIItoUNICODE( std::string ASCIIstring );

	// Vector3 conversion helpers
	static Vector3 ConvertXMFLOAT3TOVector3(XMFLOAT3 xmVal);
	static Vector3 ConvertXMFLOAT4TOVector3(XMFLOAT4 xmVal);
	static XMFLOAT3 ConvertVector3ToXMFLOAT3(Vector3 xmVal);
	static XMFLOAT4 ConvertVector3ToXMFLOAT4(Vector3 xmVal);
	//Vector4 methods

	//static Vector3 ConvertXMFLOAT3TOVector3(XMFLOAT3 xmVal);
	//static Vector3 ConvertXMFLOAT4TOVector3(XMFLOAT4 xmVal);
	static XMFLOAT3 ConvertVector4ToXMFLOAT3(Vector4 xmVal);
	static XMFLOAT4 ConvertVector4ToXMFLOAT4(Vector4 xmVal);

	static std::string Helper::NumberToString( int Number );
	static int  Helper::StringToNumber (std::string &Text );
	static void trim2(std::string& str);

	static std::string SplitFilename (const std::string& str);
	static std::string GetExtension (const std::string& str);

};

class MathHelper
{
public:

 
protected:
private:
};

 
#endif