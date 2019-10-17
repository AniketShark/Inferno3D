// ***********************************************************************
// Assembly         : InfernoEngine
// Author           : Aniket Kayande
// Created          : 02-08-2014
//
// Last Modified By : Aniket Kayande
// Last Modified On : 02-14-2014
// ***********************************************************************
// <copyright file="GameObjectManager.cpp" company="Inferno Studio">
//     Copyright (c) Inferno Studio. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#include <map>
#include <stdlib.h>
#include "GameObjectManager.h"
#include "Helper.h"
#include "PlyFileHandler.h"
#include <process.h>
#include <exception>
#include "Console.h"


struct FileLoadInfo 
{
	FILETYPE type;
	std::string path;
};

std::vector<FileLoadInfo*> assetsPathList;

//////////////////////////////////////////////////////////////////////////
//  Threading Code
//////////////////////////////////////////////////////////////////////////

//Variables
CRITICAL_SECTION assetLoadingCriticalSection;
CRITICAL_SECTION fbxCriticalSection;
HANDLE  threadHandles;
DWORD noOfThreads;
bool threaded = false;

void SaveMeshToList(Mesh* mesh)
{
	if(threaded)
	{
		EnterCriticalSection(&assetLoadingCriticalSection);
		if(mesh->mIsInGlobalVB)
		{
			g_TotalNoOfVertices += mesh->mNoOfVertices;
			g_TotalNoOfTriangles += (int) (mesh->mNumIndices / 3);
			mStaticMeshList[mesh->mModelName] = mesh;
		}
		else
		{
			mDynamicMeshList[mesh->mModelName] = mesh;
		}
		LeaveCriticalSection(&assetLoadingCriticalSection);
	}
	else
	{
		if(mesh->mIsInGlobalVB)
		{
			g_TotalNoOfVertices += mesh->mNoOfVertices;
			g_TotalNoOfTriangles += (int) (mesh->mNumIndices / 3);
			mStaticMeshList[mesh->mModelName] = mesh;
		}
		else
		{
			mDynamicMeshList[mesh->mModelName] = mesh;
		}
	}
}

//Asset Loading Threads
DWORD WINAPI PlyLoadingThread(PVOID param)
{
	FileLoadInfo* path = static_cast<FileLoadInfo*>(param);
	std::string pathstring = (path->path);
	LoadPlyFileToMesh(pathstring,true);
	//Sleep(100);
	return 0;
}

DWORD WINAPI FBXLoadingThread(PVOID param)
{
	FileLoadInfo* path = static_cast<FileLoadInfo*>(param);
	std::string pathstring = (path->path);
	//EnterCriticalSection(&fbxCriticalSection);
	SaveMeshToList(LoadMeshFromFbx(pathstring.c_str())); 
	//LeaveCriticalSection(&fbxCriticalSection);
	//Sleep(100);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

std::string MODELS_LOCATION = ""; 

std::string ANIMATIONS_LOCATION = "";

std::string LEVELS_LOCATION = "";

//mesh list for ply files
std::map<std::string,Mesh*> mStaticMeshList;

std::map<std::string,Mesh*> mDynamicMeshList;


/// <summary>
/// Loads the models from configuration file.
/// </summary>
/// <param name="configFile">The configuration file.</param>
/// <returns>bool.</returns>
bool LoadModelsFromConfigFileThreaded(std::string configFile)
{
	std::ifstream file(configFile);

	if(!file.is_open())
	{
		MessageBox(NULL,L"The Config File is not found",L"File Reading Error",MB_OK);
		return false;
	}

	std::string tempString;

	while(tempString != "ModelLocation")
	{
		file>>tempString;
	}

	file>>MODELS_LOCATION;

	while(tempString != "</ConfigEnd>")
	{
		file>>tempString;
		std::string modelName = "";

		if(tempString == "<ModelInfo>")
		{
			file>>tempString;
			file>>tempString;

			modelName = tempString;
			
			std::string extension = Helper::GetExtension(modelName);

			if(extension == "ply")
			{
				if(modelName != "")
				{
					if(extension == "ply")
					{
						//mStaticMeshList[modelName] = LoadPlyFileToMesh((MODELS_LOCATION + modelName),true);
						FileLoadInfo* newPath = new FileLoadInfo;
						newPath->type = PLY;
						newPath->path = (MODELS_LOCATION + modelName);
						assetsPathList.push_back(newPath);
					}
				}
			}

			if(extension == "fbx")
			{
				FileLoadInfo* newPath = new FileLoadInfo;
				newPath->type = FBX;
				newPath->path = (MODELS_LOCATION + modelName);
				assetsPathList.push_back(newPath);
				//mStaticMeshList[modelName] = LoadMeshFromFbx(path.c_str());
				//mStaticMeshList[modelName]->mModelName = modelName;
			}
		}
	}

	InitializeCriticalSection(&assetLoadingCriticalSection);

	InitializeCriticalSection(&fbxCriticalSection);

	int noOfThreads = assetsPathList.size();

	HANDLE threadHandleArray[10000];

	DWORD threadsCount = static_cast<DWORD>(noOfThreads); 
	LPDWORD mThreads[10000];
	int index = 0;
	for (int i  = 0; i !=  assetsPathList.size(); i++)
	{
		if(assetsPathList[i]->type == PLY)
		{
			//std::string path = fileIt->second;
			threadHandleArray[index] = CreateThread(NULL,0,PlyLoadingThread,assetsPathList[i],0,(DWORD*)&mThreads[index]);
			index++;
		}
		else if(assetsPathList[i]->type == FBX)
		{
			//std::string path = fileIt->second;
			threadHandleArray[index] = CreateThread(NULL,0,FBXLoadingThread,assetsPathList[i],0,(DWORD*)&mThreads[index]);
			index++;
		}
	}
	
	WaitForMultipleObjects(threadsCount,threadHandleArray,true,INFINITE);

	// Clean-up
	for ( int iLoop =  0; iLoop < noOfThreads; iLoop++)
		CloseHandle( threadHandleArray[iLoop]);

	DeleteCriticalSection(&fbxCriticalSection);
	DeleteCriticalSection(&assetLoadingCriticalSection);

	CreateGlobalVertexBufferAndIndexBuffer();
	
	//file.close();
	return true;
}

bool LoadModelsFromConfigFileNormal(std::string configFile)
{
	std::ifstream file(configFile);

	if(!file.is_open())
	{
		MessageBox(NULL,L"The Config File is not found",L"File Reading Error",MB_OK);
		return false;
	}

	std::string tempString;

	while(tempString != "ModelLocation")
	{
		file>>tempString;
	}

	file>>MODELS_LOCATION;

	while(tempString != "</ConfigEnd>")
	{
		file>>tempString;
		std::string modelName = "";

		if(tempString == "<ModelInfo>")
		{
			file>>tempString;
			file>>tempString;

			modelName = tempString;

			std::string extension = Helper::GetExtension(modelName);

			if(extension == "ply")
			{
				if(modelName != "")
				{
					if(extension == "ply")
					{
						SaveMeshToList(LoadPlyFileToMesh((MODELS_LOCATION + modelName),true));
						//FileLoadInfo* newPath = new FileLoadInfo;
						//newPath->type = PLY;
						//newPath->path = (MODELS_LOCATION + modelName);
						//assetsPathList.push_back(newPath);
					}
				}
			}

			if(extension == "fbx")
			{
				//FileLoadInfo* newPath = new FileLoadInfo;
				//newPath->type = FBX;
				//newPath->path = (MODELS_LOCATION + modelName);
				std::string path = (MODELS_LOCATION + modelName);
				SaveMeshToList(LoadMeshFromFbx(path.c_str()));
				//assetsPathList.push_back(newPath);
				//mStaticMeshList[modelName] = LoadMeshFromFbx(path.c_str());
				//mStaticMeshList[modelName]->mModelName = modelName;
			}
		}
	}

	//InitializeCriticalSection(&assetLoadingCriticalSection);

	//InitializeCriticalSection(&fbxCriticalSection);

	//int noOfThreads = assetsPathList.size();

	//HANDLE threadHandleArray[10000];

	//DWORD threadsCount = static_cast<DWORD>(noOfThreads); 
	//LPDWORD mThreads[10000];
	//int index = 0;
	//for (int i  = 0; i !=  assetsPathList.size(); i++)
	//{
	//	if(assetsPathList[i]->type == PLY)
	//	{
	//		//std::string path = fileIt->second;
	//		threadHandleArray[index] = CreateThread(NULL,0,PlyLoadingThread,assetsPathList[i],0,(DWORD*)&mThreads[index]);
	//		index++;
	//	}
	//	else if(assetsPathList[i]->type == FBX)
	//	{
	//		//std::string path = fileIt->second;
	//		threadHandleArray[index] = CreateThread(NULL,0,FBXLoadingThread,assetsPathList[i],0,(DWORD*)&mThreads[index]);
	//		index++;
	//	}
	//}

	//WaitForMultipleObjects(threadsCount,threadHandleArray,true,INFINITE);

	//DeleteCriticalSection(&fbxCriticalSection);
	//DeleteCriticalSection(&assetLoadingCriticalSection);

	CreateGlobalVertexBufferAndIndexBuffer();
	return true;
}

///////////////////////////////////////////////////////
//  Threaded Code
///////////////////////////////////////////////////////
//InitializeCriticalSection(&myCriticalSection);
//
//int noOfAnimals = 100;
//HANDLE mThreadHandlesArray[100];
//DWORD noOfThreads = static_cast<DWORD>(noOfAnimals);
//
//for( int iLoop = 0; iLoop < noOfAnimals; iLoop++)
//{
//	//THIS GetMapNodeAtIndex(int) METHOD IS ONLY USED FOR THREAD EXAMPLE it is not used elsewhere
//	Elephant* node = elephantMyMap.GetMapNodeAtIndex(iLoop); 
//	if(node != NULL)
//	{
//		mThreadHandlesArray[iLoop] = CreateThread(NULL,	
//			0,
//			&EatAgeThread,
//			node,   //PASSING Elephant pointer as PARAMETER
//			0,
//			(DWORD*) &node->mThread);
//	}
//
//}
//DWORD retVal;
//retVal = WaitForMultipleObjects(noOfThreads,mThreadHandlesArray,true,INFINITE);




/// <summary>
/// Loads the data in static vb and ib.
/// </summary>
/// <param name="mesh">The mesh.</param>
/// <param name="vertexArray">The vertex array.</param>
/// <param name="indexArray">The index array.</param>
/// <returns>bool.</returns>
bool LoadDataInStaticVBAndIB(Mesh* mesh,Vertex* vertexArray,WORD* indexArray)
{
//	if(threaded)
//		EnterCriticalSection(&assetLoadingCriticalSection);
	
	mesh->mRenderInfo.mVertexBufferStartIndex = g_LastVertexBufferIndex;
	mesh->mRenderInfo.mIndexBufferStartIndex = g_LastIndexBufferIndex;

	for(int index = 0; index != mesh->mNoOfVertices;index++)
	{
		try
		{
			XMFLOAT4 pos = mesh->mVertices[index].m_Position;
			XMFLOAT4 normal = mesh->mVertices[index].m_Normal;
			XMFLOAT2 texCoord = mesh->mVertices[index].m_TexCoord;
			XMFLOAT4 binormal = mesh->mVertices[index].m_Binormal;
			XMFLOAT4 tangent = mesh->mVertices[index].m_Tangent;

			int indexOffset = mesh->mRenderInfo.mVertexBufferStartIndex + index;
			vertexArray[indexOffset].m_Position.x = pos.x;
			vertexArray[indexOffset].m_Position.y = pos.y;
			vertexArray[indexOffset].m_Position.z = pos.z;
			vertexArray[indexOffset].m_Position.w = 1.0f;

			vertexArray[indexOffset].m_Normal.x = normal.x;
			vertexArray[indexOffset].m_Normal.y = normal.y;
			vertexArray[indexOffset].m_Normal.z = normal.z;
			vertexArray[indexOffset].m_Normal.w = 1.0f;

			vertexArray[indexOffset].m_TexCoord.x = texCoord.x;
			vertexArray[indexOffset].m_TexCoord.y = texCoord.y;

			vertexArray[indexOffset].m_Binormal = binormal;
			vertexArray[indexOffset].m_Tangent = tangent;

			vertexArray[indexOffset].m_Color = XMFLOAT4(1,1,1,1);
		}
		catch (std::exception& e)
		{
			ConsolePrint(e.what());
		}
	}

	for(int index = 0; index != mesh->mNumIndices;index++)
	{
		int indexBufferIndex = g_LastIndexBufferIndex;
		indexArray[g_LastIndexBufferIndex + index] = mesh->mIndices[index];
	}

	// Update the "last" vertex index location...
	g_LastVertexBufferIndex += mesh->mNoOfVertices;
	// Round up to next 16 float offset (just in case): 16 floats = 16 * 4 bytes = 64 bytes
	int multipleOf64Bytes = ( g_LastVertexBufferIndex / 64 ) + 1; 
	g_LastVertexBufferIndex = multipleOf64Bytes * 64; 


	// Update the "last" vertex index location...
	g_LastIndexBufferIndex += mesh->mNumIndices;
	// Round up to next 16 byte offset (just in case)
	multipleOf64Bytes = ( g_LastIndexBufferIndex / 64 ) + 1;
	g_LastIndexBufferIndex = multipleOf64Bytes * 64; 

//	if(threaded)
//		LeaveCriticalSection(&assetLoadingCriticalSection);

	return true;
}

/// <summary>
/// Creates the global vertex buffer and index buffer.
/// </summary>
/// <returns>bool.</returns>
bool CreateGlobalVertexBufferAndIndexBuffer()
{

	g_TotalNoOfVertices = g_TotalNoOfVertices + (g_TotalNoOfVertices/2);
	g_TotalNoOfTriangles = g_TotalNoOfTriangles + (g_TotalNoOfTriangles/2);


	// Create a vertex and index buffer big enough to hold ALL the models...
	Vertex* tempVertexArray = new Vertex[ g_TotalNoOfVertices ];
	memset( tempVertexArray, 0, g_TotalNoOfVertices * sizeof( Vertex ));

	WORD* tempIndexArray = new WORD[ g_TotalNoOfTriangles * 3 ];
	memset( tempIndexArray, 0, g_TotalNoOfTriangles * 3 * sizeof(WORD));

	std::map<std::string,Mesh*>::iterator meshIt;

	for(meshIt = mStaticMeshList.begin(); meshIt != mStaticMeshList.end();meshIt++)
	{
		LoadDataInStaticVBAndIB(meshIt->second,tempVertexArray,tempIndexArray);
	}
	if(mStaticMeshList.size() > 0)
		if(!CreateConstantBuffers(tempVertexArray,tempIndexArray))
			return false;

	//DeleteCriticalSection(&fbxCriticalSection);
	//DeleteCriticalSection(&assetLoadingCriticalSection);
	
	return true;

}

/// <summary>
/// Creates the constant buffers.
/// </summary>
/// <param name="vertexArray">The vertex array.</param>
/// <param name="indexArray">The index array.</param>
/// <returns>bool.</returns>
bool CreateConstantBuffers(Vertex* vertexArray,WORD* indexArray)
{
	HRESULT hr;

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( Vertex ) * g_TotalNoOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertexArray;

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Unable to create vertex buffer",L"ERROR",MB_OK);
		return false;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * g_TotalNoOfTriangles * 3;       
	//bd.ByteWidth = sizeof( DWORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = indexArray;
	hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Unable to create index buffer",L"ERROR",MB_OK);
		return false;
	}

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Unable to create constant buffer",L"ERROR",MB_OK);
		return false;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ChangingBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0; 
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pChangingBuffer);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Unable to create changing buffer",L"ERROR",MB_OK);
		return false;
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AnimationMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0; 
	hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pBoneMatrixBuffer);
	if( FAILED( hr ) )
	{
		MessageBox(NULL,L"ERROR: Unable to create changing buffer",L"ERROR",MB_OK);
		return false;
	}

	return true;
}

/// <summary>
/// Loads the data in vb and ib.
/// </summary>
/// <param name="inputModel">The input model.</param>
/// <param name="vertexArray">The vertex array.</param>
/// <param name="indexArray">The index array.</param>
/// <returns>bool.</returns>
bool LoadDataInVBAndIB(Model &inputModel,Vertex* vertexArray,DWORD* indexArray)
{

	inputModel.modelInfo.mVertexBufferStartIndex = g_LastVertexBufferIndex;
	inputModel.modelInfo.mIndexBufferStartIndex = g_LastIndexBufferIndex;


	for(int index = 0; index != inputModel.modelInfo.mNoOfVertices;index++)
	{

		XMFLOAT4 pos = inputModel.mPlyLoader.GetVertexAtIndex(index).m_Position;
		XMFLOAT4 normal = inputModel.mPlyLoader.GetVertexAtIndex(index).m_Normal;
		XMFLOAT2 texCoord = inputModel.mPlyLoader.GetVertexAtIndex(index).m_TexCoord;
		XMFLOAT4 binormal = inputModel.mPlyLoader.GetVertexAtIndex(index).m_Binormal;
		XMFLOAT4 tangent = inputModel.mPlyLoader.GetVertexAtIndex(index).m_Tangent;

		int indexOffset = inputModel.modelInfo.mVertexBufferStartIndex + index;
		vertexArray[indexOffset].m_Position.x = pos.x;
		vertexArray[indexOffset].m_Position.y = pos.y;
		vertexArray[indexOffset].m_Position.z = pos.z;
		vertexArray[indexOffset].m_Position.w = 1.0f;

		vertexArray[indexOffset].m_Normal.x = normal.x;
		vertexArray[indexOffset].m_Normal.y = normal.y;
		vertexArray[indexOffset].m_Normal.z = normal.z;
		vertexArray[indexOffset].m_Normal.w = 1.0f;

		vertexArray[indexOffset].m_TexCoord.x = texCoord.x;
		vertexArray[indexOffset].m_TexCoord.y = texCoord.y;

		vertexArray[indexOffset].m_Binormal = binormal;
		vertexArray[indexOffset].m_Tangent = tangent;


		vertexArray[indexOffset].m_Color = inputModel.mMaterial.mDiffuse;

	}

	for(int index = 0; index != inputModel.modelInfo.mNoOfTris;index++)
	{
		// Calculate the location in the index buffer (stride is 3, not 1)
		int indexBufferIndex = ( index * 3 ) + g_LastIndexBufferIndex;
		indexArray[indexBufferIndex+0] = inputModel.mPlyLoader.GetElementAtIndex(index).index_v1;
		indexArray[indexBufferIndex+1] = inputModel.mPlyLoader.GetElementAtIndex(index).index_v2;
		indexArray[indexBufferIndex+2] = inputModel.mPlyLoader.GetElementAtIndex(index).index_v3;
	}

	// Update the "last" vertex index location...
	g_LastVertexBufferIndex += inputModel.mPlyLoader.GetNumberOfVertices();
	// Round up to next 16 float offset (just in case): 16 floats = 16 * 4 bytes = 64 bytes
	int multipleOf64Bytes = ( g_LastVertexBufferIndex / 64 ) + 1; 
	g_LastVertexBufferIndex = multipleOf64Bytes * 64; 


	// Update the "last" vertex index location...
	g_LastIndexBufferIndex += inputModel.mPlyLoader.GetNumberOfElements() * 3;
	// Round up to next 16 byte offset (just in case)
	multipleOf64Bytes = ( g_LastIndexBufferIndex / 64 ) + 1;
	g_LastIndexBufferIndex = multipleOf64Bytes * 64; 

	return true;
}

/// <summary>
/// Creates the object.
/// </summary>
/// <param name="objectName">Name of the object.</param>
void CreateObject(std::string objectName)
{

}

/// <summary>
/// Gets the name of the mesh by.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <returns>Mesh *.</returns>
Mesh* GetMeshByName(std::string fileName)
{
	std::map<std::string,Mesh*>::iterator meshIt = mStaticMeshList.find(fileName);

	if(meshIt != mStaticMeshList.end())
	{
		return meshIt->second;
	}
	else 
	{
		std::map<std::string,Mesh*>::iterator dynamicMeshIt = mDynamicMeshList.find(fileName);
		if(dynamicMeshIt != mDynamicMeshList.end())
		{
			return dynamicMeshIt->second;
		}
	}

	return NULL;

}


