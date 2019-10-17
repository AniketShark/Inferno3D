
#ifndef Material_H_
#define Material_H_

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <sstream>

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	XMFLOAT4 mDiffuse;
	XMFLOAT4 mAmbient;
	XMFLOAT4 mSpecular;
	XMFLOAT4 mMeshColor;
};



#endif

