#ifndef PlyFileHandler_H_
#define PlyFileHandler_H_

#include "PlyUtility.h"
#include "Mesh.h"
#include <string>

extern int MAX_VERTICES_PER_BUFFER;

Mesh* LoadPlyFileToMesh(std::string fileName,bool loadInGlobalStaticBuffer);

#endif 