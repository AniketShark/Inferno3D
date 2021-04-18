
#ifndef Material_H_
#define Material_H_

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
//#include <xnamath.h>
#include <sstream>

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	DirectX::XMFLOAT4 mDiffuse;
	DirectX::XMFLOAT4 mAmbient;
	DirectX::XMFLOAT4 mSpecular;
	DirectX::XMFLOAT4 mMeshColor;
};



#endif

