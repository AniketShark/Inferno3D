
#include <process.h>
#include "PlyFileHandler.h"

int MAX_VERTICES_PER_BUFFER = 500;

bool LoadTexturesAndCreateResourceViews(Mesh* mesh, ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
	bool hr;
	int index = 0;
	for (int textureIt = 0; textureIt != mesh->mNumTextures; textureIt++)
	{
		Texture* textPoint = new Texture;
		std::string str = mesh->mTextureNameList[textureIt];
		hr = textPoint->Initialize(device, immediateContext, str);
		if (hr == false)
			return false;
		mesh->mTextureArray[textureIt] = textPoint;
	}
	return true;
}

std::string SplitFilename(const std::string& str)
{
	//std::cout << "Splitting: " << str << '\n';
	unsigned found = str.find_last_of("/\\");
	//std::cout << " path: " << str.substr(0,found) << '\n';
	//std::cout << " file: " << str.substr(found+1) << '\n';
	return str.substr(found + 1);
}

Mesh* LoadPlyFileToMesh(std::string fileName, ID3D11Device* device, ID3D11DeviceContext* immediateContext,bool loadInGlobalStaticBuffer)
{
	Mesh* mesh = new Mesh;
	mesh->mIsInGlobalVB = loadInGlobalStaticBuffer;
	mesh->mModelName = SplitFilename(fileName);
	mesh->mFileType = PLY;
	PlyUtility* plyLoaderObject = new PlyUtility();
	plyLoaderObject->LoadPlyFile(fileName);
	mesh->mNoOfVertices = plyLoaderObject->GetNumberOfVertices();
	mesh->mNumIndices = plyLoaderObject->GetNumberOfElements() * 3;
	mesh->mNumTextures = plyLoaderObject->GetNumberOfTextures();

	// setting render info for mesh
	mesh->mRenderInfo.mNoOfTris = plyLoaderObject->GetNumberOfElements();
	mesh->mRenderInfo.mNoOfIndices = mesh->mNumIndices;
	mesh->mRenderInfo.mNoOfVertices = mesh->mNoOfVertices;
	mesh->mRenderInfo.mNoofTextures = mesh->mNumTextures;

	mesh->mVertices = new Vertex[mesh->mNoOfVertices];
	mesh->mIndices = new WORD[mesh->mNumIndices];
	mesh->mNoOfElements  = mesh->mRenderInfo.mNoOfTris;
	mesh->mElements = new Element[mesh->mNoOfElements];
	if(mesh->mNoOfVertices > MAX_VERTICES_PER_BUFFER)
	{
		mesh->mIsInGlobalVB = false;
	}
	else
	{
		mesh->mIsInGlobalVB = true;
	}

	//Copying vertices's from loader
	for (int index = 0; index != mesh->mNoOfVertices;index++)
	{
		mesh->mVertices[index] = plyLoaderObject->GetVertexAtIndex(index);
	}

	//Copying indices from loader
	for (int index = 0; index != mesh->mRenderInfo.mNoOfTris; index++)
	{
		int indexOffset = ( index * 3 );
		Element ele = plyLoaderObject->GetElementAtIndex(index);

		mesh->mIndices[indexOffset + 0] = ele.index_v1;
		mesh->mIndices[indexOffset + 1] = ele.index_v2;
		mesh->mIndices[indexOffset + 2] = ele.index_v3;

		mesh->mElements[index].index_v1 = ele.index_v1;
		mesh->mElements[index].index_v2 = ele.index_v2;
		mesh->mElements[index].index_v3 = ele.index_v3;
		mesh->mElements[index].restlength1 = ele.restlength1;
		mesh->mElements[index].restlength2 = ele.restlength2;
		mesh->mElements[index].restlength3 = ele.restlength3;
	}
	
	//Copying texture names from loader
	for (int j = 0;j < mesh->mNumTextures;j++)
	{
		mesh->mTextureNameList.push_back(plyLoaderObject->textureFileNames[j]);
	}

	// creating textures
	if(mesh->mNumTextures > 0)
	{
		mesh->mTextureArray  = new Texture[mesh->mNumTextures];
		if(!LoadTexturesAndCreateResourceViews(mesh,device,immediateContext))
		{
			MessageBox(NULL,L"Texture loading for ply file failed",L"TextureLoadingError",MB_OK);
			return NULL;
		}
	}

	mesh->mPlyInfo = plyLoaderObject;
	//SaveMeshToList(mesh);
	return mesh;
}



 
