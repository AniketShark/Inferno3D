#ifndef PlyFileHandler_H_
#define PlyFileHandler_H_

#include "PlyUtility.h"
#include "Mesh.h"
#include <string>

extern int MAX_VERTICES_PER_BUFFER;

Mesh* LoadPlyFileToMesh(std::string fileName, ID3D11Device* device, ID3D11DeviceContext* immediateContext,bool loadInGlobalStaticBuffer);

#endif 