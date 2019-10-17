#ifndef FBX_HANDLER_H
#define FBX_HANDLER_H

#include "HelperFuncs.h"

class Mesh;
class SkeletalAnimation;

void InitializeFbxSdk();
void CleanUpFbxSdk();

void PrintFbxFile(const char* szFileName);
Mesh* LoadMeshFromFbx(const char* szFileName);
SkeletalAnimation* LoadAnimationFromFbx(const char* szFileName);


#endif