#include <fbxsdk.h>
#include "FbxHandler.h"
#include "Mesh.h"
#include "SkeletalAnimation.h"
#include "GameObjectManager.h"
FbxManager* g_FbxSdkManager = NULL;


void InitializeFbxSdk()
{
    g_FbxSdkManager = FbxManager::Create();
}

void CleanUpFbxSdk()
{
    if(g_FbxSdkManager != NULL)
    {
        g_FbxSdkManager->Destroy();
        g_FbxSdkManager = NULL;
    }
}

/* Tab character ("\t") counter */
int numTabs = 0; 

/**
 * Print the required number of tabs.
 */
void PrintTabs() {
    for(int i = 0; i < numTabs; i++)
        OutputDebugStringA("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch(type) {
        case FbxNodeAttribute::eUnknown: return "unidentified";
        case FbxNodeAttribute::eNull: return "null";
        case FbxNodeAttribute::eMarker: return "marker";
        case FbxNodeAttribute::eSkeleton: return "skeleton";
        case FbxNodeAttribute::eMesh: return "mesh";
        case FbxNodeAttribute::eNurbs: return "nurb";
        case FbxNodeAttribute::ePatch: return "patch";
        case FbxNodeAttribute::eCamera: return "camera";
        case FbxNodeAttribute::eCameraStereo:    return "stereo";
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
        case FbxNodeAttribute::eLight: return "light";
        case FbxNodeAttribute::eOpticalReference: return "optical reference";
        case FbxNodeAttribute::eOpticalMarker: return "marker";
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case FbxNodeAttribute::eBoundary: return "boundary";
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case FbxNodeAttribute::eShape: return "shape";
        case FbxNodeAttribute::eLODGroup: return "lodgroup";
        case FbxNodeAttribute::eSubDiv: return "subdiv";
        case FbxNodeAttribute::eCachedEffect: return "cached effect";
        case FbxNodeAttribute::eLine: return "line";
        default: return "unknown";
    }
}

/**
 * Print an attribute.
 */
void PrintAttribute(FbxNodeAttribute* pAttribute) {
    if(!pAttribute) return;
 
    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the chararcter array of a FbxString, use its Buffer() method.
    char buffer[500];
    buffer[499] = '\0';
    _snprintf_s(buffer, 499, "<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
    OutputDebugStringA(buffer);
}

/**
 * Print a node, its attributes, and all its children recursively.
 */
void PrintNode(FbxNode* pNode) {
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // print the contents of the node.
    char buffer[500];
    buffer[499] = '\0';
    _snprintf_s(buffer, 499, "<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n", 
        nodeName, 
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
        );
    OutputDebugStringA(buffer);

    numTabs++;

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children nodes.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintNode(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    OutputDebugStringA("</node>\n");
}

void PrintFbxFile(const char* szFileName)
{
    char buffer[500];
    buffer[499] = '\0';
	_snprintf_s(buffer, 499, "********************\nPrintFbxFile='%s'\n", szFileName);
    OutputDebugStringA(buffer);

    // Create the io settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(g_FbxSdkManager, IOSROOT);
    g_FbxSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(g_FbxSdkManager,"");
    
    // Use the first argument as the filename for the importer.
    if(!lImporter->Initialize(szFileName, -1, g_FbxSdkManager->GetIOSettings())) {
        OutputDebugStringA("Call to FbxImporter::Initialize() failed.\n");
        OutputDebugStringA("Error returned:");
        //OutputDebugStringA(lImporter->GetLastErrorString());
        OutputDebugStringA("\n\n");
        exit(-1);
    }
    
    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(g_FbxSdkManager,"myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node, because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
    if(lRootNode) 
	{
        for(int i = 0; i < lRootNode->GetChildCount(); ++i)
        {
            PrintNode(lRootNode->GetChild(i));
        }
        OutputDebugStringA("===============================\n");
        _snprintf_s(buffer, 499, "********************\nPoses %d\n", lScene->GetPoseCount());
        for(int i = 0; i < lScene->GetPoseCount(); ++i)
        {
            FbxPose* pose = lScene->GetPose(i);
            
            _snprintf_s(buffer, 499, "Pose='%s' count='%d' isBindPose='%d' isRestPose='%d'\n", pose->GetName(), pose->GetCount(), pose->IsBindPose(), pose->IsRestPose());
            OutputDebugStringA(buffer);

            for(int j = 0; j < pose->GetCount(); ++j)
            {
                FbxNode* node = pose->GetNode(j);
                if(node)
                {
                    const FbxMatrix& m = pose->GetMatrix(j);

                    FbxVector4 translation;
                    FbxQuaternion rotation;
                    FbxVector4 shearing;
                    FbxVector4 scaling;
                    double sign;
                    m.GetElements(translation, rotation, shearing, scaling, sign);

                    FbxVector4 rotEuler = rotation.DecomposeSphericalXYZ();

                    _snprintf_s(buffer, 499, "  node='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'\n", 
                        node->GetName(), translation[0], translation[1], translation[2], rotEuler[0], rotEuler[1], rotEuler[2], scaling[0], scaling[1], scaling[2]);
                    OutputDebugStringA(buffer);
                }
            }
        }

        OutputDebugStringA("===============================\n");
        _snprintf_s(buffer, 499, "********************\nAnimStacks %d\n", lImporter->GetAnimStackCount());
        OutputDebugStringA(buffer);
        for(int i = 0; i < lImporter->GetAnimStackCount(); ++i)
        {
            FbxTakeInfo* take = lImporter->GetTakeInfo(i);
            int numFrames = static_cast<int>(take->mLocalTimeSpan.GetDuration().GetFrameCount() + 1);
            _snprintf_s(buffer, 499, "  Take[%d][%s] [%d frames]\n", i, take->mName.Buffer(), numFrames);
            OutputDebugStringA(buffer);

            FbxAnimStack* animStack = static_cast<FbxAnimStack*>(lScene->RootProperty.FindSrcObject(FbxCriteria::ObjectType(g_FbxSdkManager->FindClass("FbxAnimStack")), take->mName, 0));
            if(animStack)
            {
                _snprintf_s(buffer, 499, "   animStack[%s]\n", animStack->GetName());
                OutputDebugStringA(buffer);

                FbxAnimLayer* animLayer = static_cast<FbxAnimLayer*>(animStack->RootProperty.GetSrcObject(FbxCriteria::ObjectType(g_FbxSdkManager->FindClass("FbxAnimLayer")), 0));
                if(animLayer)
                {
                    _snprintf_s(buffer, 499, "   animLayer[%s]\n", animLayer->GetName());
                    OutputDebugStringA(buffer);

                    if(take->mName.Compare(FBXSDK_TAKENODE_DEFAULT_NAME) != 0)
                    {
                        /*
                        FbxNode* rootNode = mFbxScene->GetRootNode();
                        if(sequenceIndex == 0)
                        {
                            ParseNode(rootNode, 0, IDENTITY_MATRIX);
                        }
                        if(mLoadOperation == LO_ANIM || mLoadOperation == LO_BASE)
                        {
                            ParseAnimation(rootNode, sequenceIndex, preprocessTask);
                        }
                        */
                    }
                }
            }
        }

        OutputDebugStringA("===============================\n");
    }

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

    /*
    FbxExporter* lExporter = FbxExporter::Create(g_FbxSdkManager, "");
    // initialize the exporter by providing a filename and the IOSettings to use
    int pFileFormat = -1;
    if( pFileFormat < 0 || pFileFormat >= g_FbxSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
    {
        // Write in fall back format in less no ASCII format found
        pFileFormat = g_FbxSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

        //Try to export in ASCII if possible
        int lFormatIndex, lFormatCount = g_FbxSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

        for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
        {
            if (g_FbxSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
            {
                FbxString lDesc =g_FbxSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
                const char *lASCII = "ascii";
                if (lDesc.Find(lASCII)>=0)
                {
                    pFileFormat = lFormatIndex;
                    break;
                }
            }
        } 
    }

    lExporter->Initialize("myfile.fbx", pFileFormat, ios);
    
    // export the scene.
    lExporter->Export(lScene); 
    
    // destroy the exporter
    lExporter->Destroy();
    */
    ios->Destroy();

	lScene->Destroy();

}

void TriangulateRecursive(FbxNode* pNode)
{
	//FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	//if (lNodeAttribute)
	//{
	//	if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
	//		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
	//		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
	//		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
	//	{
	//		const char* nodeName = pNode->GetName();
	//		char buffer[500];
	//		buffer[499] = '\0';
	//		_snprintf_s(buffer, 499, "Triangulate node name='%s'\n", 
	//			nodeName
	//			);
	//		OutputDebugStringA(buffer);

	//		FbxGeometryConverter lConverter(pNode->GetFbxManager());
	//		lConverter.TriangulateInPlace(pNode);
	//	}
	//}

	//const int lChildCount = pNode->GetChildCount();
	//for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	//{
	//	TriangulateRecursive(pNode->GetChild(lChildIndex));
	//}
}

//////////////////////////////////////////////////////////////////////////
//  TriangulateRecursive for 2014 SDK
//////////////////////////////////////////////////////////////////////////
void TriangulateRecursive(FbxScene* pNode)
{
	FbxGeometryConverter lConverter(g_FbxSdkManager);
	lConverter.Triangulate(pNode,true,false);
}

void LoadSkinInfo(FbxNode* pNode, Mesh* pMesh, int vertOffset, int numVerts)
{
    char buffer[500];
    buffer[499] = '\0';

    FbxMesh* lMesh = pNode->GetMesh();

    //_snprintf_s(buffer, 499, "  LoadSkinInformation for mesh[%s] vertOffset %d numVerts %d \n", pNode->GetName(), vertOffset, numVerts);
    //OutputDebugStringA(buffer);


    //skin information
    //we need to get the number of clusters
    const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
    int lClusterCount = 0;
    for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
    {
        lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
    }
    //_snprintf_s(buffer, 499, "  SkinCount ='%d' ClusterCount ='%d'\n", lSkinCount, lClusterCount);
    //OutputDebugStringA(buffer);

    if(lClusterCount == 0)
    {
        // This mesh does not have any skin reformers, so check if it is attached directly to a skeleton joint and
        // do a rigid skinning to that joint
        FbxNode* lParent = pNode->GetParent();
        if(lParent)
        {
            FbxNodeAttribute* lNodeAttribute = lParent->GetNodeAttribute();
	        if(lNodeAttribute && 
		        lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
            {
                for(int b = 0; b < pMesh->mNoOfBones; ++b)
                {
                    if(strcmp(pMesh->mSkeleton[b].name, lParent->GetName()) == 0)
                    {
                       //_snprintf_s(buffer, 499, "  Node [%s] attached to Skeleton [%s][%d]\n", pNode->GetName(), lParent->GetName(), b);
                       // OutputDebugStringA(buffer);

                        if(vertOffset + numVerts <= pMesh->mNoOfVertices)
                        {
                            for(int i = 0; i < numVerts; ++i)
                            {
//                                pMesh->mVerts[i + vertOffset].BoneIndex[0] = b;
//                                pMesh->mVerts[i + vertOffset].BoneWeight[0] = 1.0f;
								//Change
								pMesh->mVertices[i + vertOffset].BoneIndex[0] = b;
								pMesh->mVertices[i + vertOffset].BoneWeight[0] = 1.0f;
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
    else
    {
        FbxSkin * lSkinDeformer = (FbxSkin *)lMesh->GetDeformer(0, FbxDeformer::eSkin);
        FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();
        
        for(int c = 0; c < lClusterCount; ++c)
        {
            FbxCluster* cluster = lSkinDeformer->GetCluster(c);
            
            if(!cluster->GetLink())
            {
                _snprintf_s(buffer, 499, "   cluster[%d] has no link!\n", c);
                OutputDebugStringA(buffer);
                continue;
            }

            _snprintf_s(buffer, 499, "   cluster[%d] has link[%s]!\n", c, cluster->GetLink()->GetName());
            OutputDebugStringA(buffer);

            for(int b = 0; b < pMesh->mNoOfBones; ++b)
            {
                if(strcmp(pMesh->mSkeleton[b].name, cluster->GetLink()->GetName()) == 0)
                {
                    int numIndices = cluster->GetControlPointIndicesCount();
                    if(numIndices > 0)
                    {
                        int* indices = cluster->GetControlPointIndices();
                        double* weights = cluster->GetControlPointWeights();
                        for(int i = 0; i < numIndices; ++i)
                        {
                            if(weights[i] > 0.f)
                            {
                                if(vertOffset + indices[i] < pMesh->mNoOfVertices)
                                {
                                    for(int w = 0; w < 4; ++w)
                                    {
                                        //find the next available weight
                                        //if(weights[i] > pMesh->mVerts[vertOffset + indices[i]].BoneWeight[w])
										if(weights[i] > pMesh->mVertices[vertOffset + indices[i]].BoneWeight[w])
                                        {
										

                                            //ensure that our skinning info is ordered from most significant to least significant
                                            for(int wb = 3; wb > w; --wb)
                                            {
                                               // pMesh->mVerts[vertOffset + indices[i]].BoneIndex[wb] = pMesh->mVerts[vertOffset + indices[i]].BoneIndex[wb-1];
                                               // pMesh->mVerts[vertOffset + indices[i]].BoneWeight[wb] = pMesh->mVerts[vertOffset + indices[i]].BoneWeight[wb-1];
												//Change
												pMesh->mVertices[vertOffset + indices[i]].BoneIndex[wb] = pMesh->mVertices[vertOffset + indices[i]].BoneIndex[wb-1];
												pMesh->mVertices[vertOffset + indices[i]].BoneWeight[wb] = pMesh->mVertices[vertOffset + indices[i]].BoneWeight[wb-1];

                                            }
                                           // pMesh->mVerts[vertOffset + indices[i]].BoneIndex[w] = b;
                                           // pMesh->mVerts[vertOffset + indices[i]].BoneWeight[w] = static_cast<float>(weights[i]);

											//Change
											pMesh->mVertices[vertOffset + indices[i]].BoneIndex[w] = b;
											pMesh->mVertices[vertOffset + indices[i]].BoneWeight[w] = static_cast<float>(weights[i]);

                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        //check that all weights are normalized
        for(int i = 0; i < numVerts; ++i)
        {
            float s = 0.0f;
            for(int w = 0; w < 4; ++w)
            {
               // s += pMesh->mVerts[vertOffset + i].BoneWeight[w];
				 s += pMesh->mVertices[vertOffset + i].BoneWeight[w];

            }
            if(s > 0.0f && (s < 0.999999f || s > 1.000001f))
            {
                for(int w = 0; w < 4; ++w)
                {
                   // pMesh->mVerts[vertOffset + i].BoneWeight[w] /= s;
					 pMesh->mVertices[vertOffset + i].BoneWeight[w] /= s;

                }
            }
        }

        OutputDebugStringA("done\n");
    }
}

void LoadMeshRecursive(FbxNode* pNode, Mesh* pMesh)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && 
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		const char* nodeName = pNode->GetName();
		//char buffer[500];
		//buffer[499] = '\0';
		//_snprintf_s(buffer, 499, "Load Mesh name='%s'\n", nodeName);
		//OutputDebugStringA(buffer);

		FbxMesh* lMesh = pNode->GetMesh();
		const int lVertexCount = lMesh->GetControlPointsCount();

		if(lVertexCount > 0)
		{
			int vertOffset = pMesh->mNoOfVertices;
			//if(pMesh->mVerts && vertOffset > 0)
			if(pMesh->mVertices && vertOffset > 0)
			{
				//SimpleSkinnedVertex* verts = new (std::nothrow) SimpleSkinnedVertex[vertOffset + lVertexCount];
				//change
				Vertex* verts1 = new (std::nothrow) Vertex[vertOffset + lVertexCount];
				if(!verts1)
					return;

				//memcpy(verts, pMesh->mVerts, sizeof(SimpleSkinnedVertex) * vertOffset);
				//change
				memcpy(verts1, pMesh->mVertices, sizeof(Vertex) * vertOffset);

				//delete [] pMesh->mVerts;
				//change
				delete [] pMesh->mVertices;

				//pMesh->mVerts = verts;
				//change
				pMesh->mVertices = verts1;
			}
			else
			{
				//pMesh->mVerts = new (std::nothrow) SimpleSkinnedVertex[lVertexCount];
				//change
				pMesh->mVertices =  new (std::nothrow) Vertex[lVertexCount];
			}
			if(pMesh->mVertices)
			{
				pMesh->mNoOfVertices = vertOffset + lVertexCount;
				for(int i = 0; i < lVertexCount; ++i)
				{
					//pMesh->mVerts[i + vertOffset].Pos.x = static_cast<float>(lMesh->GetControlPoints()[i][0] * 0.01);
					//pMesh->mVerts[i + vertOffset].Pos.y = static_cast<float>(lMesh->GetControlPoints()[i][1] * 0.01);
					//pMesh->mVerts[i + vertOffset].Pos.z = static_cast<float>(lMesh->GetControlPoints()[i][2] * 0.01);
					//pMesh->mVerts[i + vertOffset].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					//change
					pMesh->mVertices[i + vertOffset].m_Position.x = static_cast<float>(lMesh->GetControlPoints()[i][0] * 0.01);
					pMesh->mVertices[i + vertOffset].m_Position.y = static_cast<float>(lMesh->GetControlPoints()[i][1] * 0.01);
					pMesh->mVertices[i + vertOffset].m_Position.z = static_cast<float>(lMesh->GetControlPoints()[i][2] * 0.01);
					pMesh->mVertices[i + vertOffset].m_Position.w = 1.0f;
					
					pMesh->mVertices[i + vertOffset].m_Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

				}
				
				int totalIndices = 0;
				const int polyCount = lMesh->GetPolygonCount();
				for (int i = 0; i < polyCount; ++i)
				{
					const int vertCount = lMesh->GetPolygonSize(i);
					totalIndices += vertCount;
				}

				int indOffset = pMesh->mNumIndices;
				if(pMesh->mIndices && indOffset > 0)
				{
					WORD* indices = new (std::nothrow) WORD[indOffset + totalIndices];
					if(!indices)
						return;
					memcpy(indices, pMesh->mIndices, sizeof(WORD) * indOffset);
					delete [] pMesh->mIndices;
					pMesh->mIndices = indices;
				}
				else
				{
					pMesh->mIndices = new (std::nothrow) WORD[totalIndices];
				}
				if(pMesh->mIndices)
				{
					const int polyCount = lMesh->GetPolygonCount();
					int ind = 0;
					for (int i = 0; i < polyCount; ++i)
					{
						const int vertCount = lMesh->GetPolygonSize(i);
						for (int j = 0; j < vertCount; ++j)
						{
							pMesh->mIndices[ind + indOffset] = static_cast<WORD>(lMesh->GetPolygonVertex(i, j) + vertOffset);
							++ind;
						}
					}
					pMesh->mNumIndices += ind;
				}

          //      _snprintf_s(buffer, 499, "  VertexCount ='%d' PolygonCount ='%d' IndicesCount ='%d'\n", lVertexCount, lMesh->GetPolygonCount(), totalIndices);
		        //OutputDebugStringA(buffer);
			
                LoadSkinInfo(pNode, pMesh, vertOffset, lVertexCount);
            }
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadMeshRecursive(pNode->GetChild(lChildIndex), pMesh);
	}
}

int CountSkeletonRecursive(FbxNode* pNode)//, Mesh* pMesh)
{
    int count = 0;
    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && 
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
        ++count;
    }

    const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		count += CountSkeletonRecursive(pNode->GetChild(lChildIndex));//, pMesh);
	}

    return(count);
}

void LoadSkeletonRecursive(FbxNode* pNode, Mesh* pMesh, int parent)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && 
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
        const char* nodeName = pNode->GetName();
		//char buffer[500];
		//buffer[499] = '\0';
		//_snprintf_s(buffer, 499, "Load Skeleton name='%s' parent='%d'\n", nodeName, parent);
		//OutputDebugStringA(buffer);

        pMesh->mSkeleton[pMesh->mNoOfBones].parent = parent;
        _snprintf_s(pMesh->mSkeleton[pMesh->mNoOfBones].name, 27, "%s", nodeName);

        FbxDouble3 translation = pNode->LclTranslation.Get();
        FbxDouble3 rotation = pNode->LclRotation.Get();
        FbxDouble3 scaling = pNode->LclScaling.Get();

        pMesh->mInitialBonePose[pMesh->mNoOfBones].translation = XMFLOAT3(static_cast<float>(translation[0] * 0.01), static_cast<float>(translation[1] * 0.01), static_cast<float>(translation[2] * 0.01));
        XMVECTOR r = XMQuaternionRotationRollPitchYaw(static_cast<float>(rotation[2]), static_cast<float>(rotation[0]), static_cast<float>(rotation[1]));
        XMStoreFloat4(&(pMesh->mInitialBonePose[pMesh->mNoOfBones].rotation), r);
        pMesh->mInitialBonePose[pMesh->mNoOfBones].scale = XMFLOAT3(static_cast<float>(scaling[0]), static_cast<float>(scaling[1]), static_cast<float>(scaling[2]));


        XMMATRIX m = pMesh->mInitialBonePose[pMesh->mNoOfBones].GetTransform();
        if(pMesh->mSkeleton[pMesh->mNoOfBones].parent >= 0)
        {
            XMMATRIX c = XMLoadFloat4x4(&pMesh->mInitialGlobalPose[pMesh->mSkeleton[pMesh->mNoOfBones].parent]);
            m *= c;
        }
        
        XMStoreFloat4x4(&pMesh->mInitialGlobalPose[pMesh->mNoOfBones], m);
        
        parent = pMesh->mNoOfBones;
        ++pMesh->mNoOfBones;
    }

    const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadSkeletonRecursive(pNode->GetChild(lChildIndex), pMesh, parent);
	}
}

void LoadSkeletonRecursive2(FbxNode* pNode, Bone* pSkeleton, int* numBones, int parent)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && 
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
        const char* nodeName = pNode->GetName();
		//char buffer[500];
		//buffer[499] = '\0';
		//_snprintf_s(buffer, 499, "Load Skeleton name='%s' parent='%d'\n", nodeName, parent);
		//OutputDebugStringA(buffer);

        pSkeleton[*numBones].parent = parent;
        _snprintf_s(pSkeleton[*numBones].name, 27, "%s", nodeName);

        parent = *numBones;
        ++(*numBones);
    }

    const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadSkeletonRecursive2(pNode->GetChild(lChildIndex), pSkeleton, numBones, parent);
	}
}

void LoadBindPose(FbxScene* lScene, Mesh* pMesh)
{
    for(int i = 0; i < lScene->GetPoseCount(); ++i)
    {
        FbxPose* pose = lScene->GetPose(i);
        //char buffer[500];
        //_snprintf_s(buffer, 499, "Pose='%s' count='%d' isBindPose='%d' isRestPose='%d'\n", pose->GetName(), pose->GetCount(), pose->IsBindPose(), pose->IsRestPose());
        //OutputDebugStringA(buffer);

        if(pose->IsBindPose())
        {
            for(int j = 0; j < pose->GetCount(); ++j)
            {
                FbxNode* node = pose->GetNode(j);
                if(node)
                {
                    const FbxMatrix& m = pose->GetMatrix(j);

                    FbxVector4 translation;
                    FbxQuaternion rotation;
                    FbxVector4 shearing;
                    FbxVector4 scaling;
                    double sign;
                    m.GetElements(translation, rotation, shearing, scaling, sign);

                    FbxVector4 rotEuler = rotation.DecomposeSphericalXYZ();

                    //_snprintf_s(buffer, 499, "  node='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'\n", 
                  //  node->GetName(), translation[0], translation[1], translation[2], rotEuler[0], rotEuler[1], rotEuler[2], scaling[0], scaling[1], scaling[2]);
                    //OutputDebugStringA(buffer);

                    //find the same bone in our mesh
                    for(int b = 0; b < pMesh->mNoOfBones; ++b)
                    {
                        if(strcmp(pMesh->mSkeleton[b].name, node->GetName()) == 0)
                        {
                            //found a match
                            _XMMATRIX mb(static_cast<float>(m[0][0] * 1.00), static_cast<float>(m[0][1] * 1.00), static_cast<float>(m[0][2] * 1.00), static_cast<float>(m[0][3] * 1.00),
                                         static_cast<float>(m[1][0] * 1.00), static_cast<float>(m[1][1] * 1.00), static_cast<float>(m[1][2] * 1.00), static_cast<float>(m[1][3] * 1.00),
                                         static_cast<float>(m[2][0] * 1.00), static_cast<float>(m[2][1] * 1.00), static_cast<float>(m[2][2] * 1.00), static_cast<float>(m[2][3] * 1.00),
                                         static_cast<float>(m[3][0] * 0.01), static_cast<float>(m[3][1] * 0.01), static_cast<float>(m[3][2] * 0.01), static_cast<float>(m[3][3] * 1.00));

                            XMStoreFloat4x4(&pMesh->mInitialGlobalPose[b], mb);

                            XMVECTOR det;
                            XMMATRIX invMb = XMMatrixInverse(&det, mb);
                            XMStoreFloat4x4(&pMesh->mSkeleton[b].mInverseBindPoseMatrix, invMb);
                            break;
                        }
                    }
                }
            }
        }
    }

    //Recalculate local transforms for bones
    for(int i = 0; i < pMesh->mNoOfBones; ++i)
    {
        if(pMesh->mSkeleton[i].parent >= 0)
        {
			// multiplying each bone matrix by its parents matrix will convert bone matrix to 
			//parents local transform
            XMMATRIX mb = XMLoadFloat4x4(&pMesh->mInitialGlobalPose[i]) * XMLoadFloat4x4(&pMesh->mSkeleton[pMesh->mSkeleton[i].parent].mInverseBindPoseMatrix);

            XMVECTOR scale;
            XMVECTOR rotQuat;
            XMVECTOR trans;
            if(XMMatrixDecompose(&scale, &rotQuat, &trans, mb))
            {
                XMStoreFloat3(&pMesh->mInitialBonePose[i].translation, trans);
                XMStoreFloat4(&pMesh->mInitialBonePose[i].rotation, rotQuat);
                XMStoreFloat3(&pMesh->mInitialBonePose[i].scale, scale);
            }
        }
    }
}

void LoadMeshFromScene(FbxScene* pScene, Mesh* pMesh)
{
    OutputDebugStringA("=========================================\nTriangulating\n");
	FbxNode* lRootNode = pScene->GetRootNode();
    if(lRootNode) 
	{
		TriangulateRecursive(pScene);
    }

    const int lPoseCount = pScene->GetPoseCount();

    OutputDebugStringA("=========================================\nGetting Skeleton Data\n");
    int numBones = CountSkeletonRecursive(lRootNode);//, pMesh);
    if(numBones > 0)
    {
        pMesh->mSkeleton = new (std::nothrow) Bone[numBones];
        pMesh->mInitialBonePose = new (std::nothrow) BonePose[numBones];
        pMesh->mInitialGlobalPose = new (std::nothrow) _XMFLOAT4X4[numBones];

        if(pMesh->mSkeleton && pMesh->mInitialBonePose)
        {
            pMesh->mNoOfBones = 0;
            LoadSkeletonRecursive(lRootNode, pMesh, -1);

            LoadBindPose(pScene, pMesh);

            // Note we calculate and store the inverse bind-pose transform for skinning later
            for(int i = 0; i < pMesh->mNoOfBones; ++i)
            {
                XMVECTOR det;
                XMMATRIX m = XMLoadFloat4x4(&pMesh->mInitialGlobalPose[i]);
                m = XMMatrixInverse(&det, m);
                XMStoreFloat4x4(&pMesh->mSkeleton[i].mInverseBindPoseMatrix, m);
            }
        }
    }

    OutputDebugStringA("=========================================\nGetting Mesh Data\n");
    LoadMeshRecursive(lRootNode, pMesh);
}

Mesh* LoadMeshFromFbx(const char* szFileName)
{
	if(threaded)
		EnterCriticalSection(&fbxCriticalSection);

	

	Mesh* pMesh = NULL;
	
	// Create the io settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(g_FbxSdkManager, IOSROOT);
    g_FbxSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(g_FbxSdkManager,"");
    
    //// Use the first argument as the filename for the importer.
    if(!lImporter->Initialize(szFileName, -1, g_FbxSdkManager->GetIOSettings())) {
        OutputDebugStringA("Call to FbxImporter::Initialize() failed.\n");
        OutputDebugStringA("Error returned:");
       // OutputDebugStringA(lImporter->GetLastErrorString());
        OutputDebugStringA("\n\n");
        exit(-1);
    }
    
    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(g_FbxSdkManager,"myScene");
	if(threaded)
		LeaveCriticalSection(&fbxCriticalSection);

    // Import the contents of the file into the scene.
	if(lImporter->Import(lScene))
	{
		/*
		// Convert Axis System to what is used in this example, if needed
		FbxAxisSystem SceneAxisSystem = lScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eLeftHanded);
		if( SceneAxisSystem != OurAxisSystem )
		{
			OurAxisSystem.ConvertScene(lScene);
		}
		
		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit SceneSystemUnit = lScene->GetGlobalSettings().GetSystemUnit();
		if( SceneSystemUnit.GetScaleFactor() != 100.0 )
		{
			//The unit in this example is meters
			FbxSystemUnit::m.ConvertScene( lScene);
		}
		*/
	}

    // The file has been imported; we can get rid of the importer.

	pMesh = new (std::nothrow) Mesh();
	pMesh->mFileType = FBX;
	if(pMesh)
	{
        LoadMeshFromScene(lScene, pMesh);

		//if(!pMesh->mVerts)
		//{
		//	delete pMesh;
		//	pMesh = NULL;
		//}
		if(!pMesh->mVertices)
		{
			delete pMesh;
			pMesh = NULL;
		}
	}

	lScene->Destroy();
	lImporter->Destroy();
	ios->Destroy();

	pMesh->mModelName = Helper::SplitFilename(szFileName);
	return(pMesh);

	

}

/*
_snprintf_s(buffer, 499, "********************\nAnimStacks %d\n", lImporter->GetAnimStackCount());
        OutputDebugStringA(buffer);
        for(int i = 0; i < lImporter->GetAnimStackCount(); ++i)
        {
            FbxTakeInfo* take = lImporter->GetTakeInfo(i);
            int numFrames = static_cast<int>(take->mLocalTimeSpan.GetDuration().GetFrameCount() + 1);
            _snprintf_s(buffer, 499, "  Take[%d][%s] [%d frames]\n", i, take->mName.Buffer(), numFrames);
            OutputDebugStringA(buffer);

            FbxAnimStack* animStack = static_cast<FbxAnimStack*>(lScene->RootProperty.FindSrcObject(FbxCriteria::ObjectType(g_FbxSdkManager->FindClass("FbxAnimStack")), take->mName, 0));
            if(animStack)
            {
                _snprintf_s(buffer, 499, "   animStack[%s]\n", animStack->GetName());
                OutputDebugStringA(buffer);

                FbxAnimLayer* animLayer = static_cast<FbxAnimLayer*>(animStack->RootProperty.GetSrcObject(FbxCriteria::ObjectType(g_FbxSdkManager->FindClass("FbxAnimLayer")), 0));
                if(animLayer)
                {
                    _snprintf_s(buffer, 499, "   animLayer[%s]\n", animLayer->GetName());
                    OutputDebugStringA(buffer);

                    if(take->mName.Compare(FBXSDK_TAKENODE_DEFAULT_NAME) != 0)
                    {
                    */

void FlattenHeirarchyRecursive(FbxNode* pNode, FbxNode** pFlattenedNodes, int* numBones, int parent)
{
    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && 
		lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
        pFlattenedNodes[*numBones] = pNode;
        parent = *numBones;
        ++(*numBones);
    }

    const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		FlattenHeirarchyRecursive(pNode->GetChild(lChildIndex), pFlattenedNodes, numBones, parent);
	}
}

void LoadAnimationTake(FbxTakeInfo* pTake, FbxScene* pScene, SkeletalAnimation* pAnim)
{
    //char buffer[500];
    //buffer[499] = '\0';
    //
    int numKeys = static_cast<int>(pTake->mLocalTimeSpan.GetDuration().GetFrameCount() + 1);
    //_snprintf_s(buffer, 499, "  Take[%s] [%d frames]\n", pTake->mName.Buffer(), numKeys);
    //OutputDebugStringA(buffer);

    _snprintf_s(pAnim->mName, 99, "%s", pTake->mName.Buffer());

    // We flatten the joint hierarchy into an array of FbxNode* so it is easier to 
    // sample the joint poses during the animation
    FbxNode** lFlattenedNodes = new (std::nothrow) FbxNode*[pAnim->mNumBones];
    if(!lFlattenedNodes)
    {
        return;
    }
    int numNodes = 0;
    FlattenHeirarchyRecursive(pScene->GetRootNode(), lFlattenedNodes, &numNodes, -1);

    pAnim->mKeys = new (std::nothrow) SkeletalAnimation::Frame[numKeys];

    if(!pAnim->mKeys)
    {
        delete [] lFlattenedNodes;
        return;
    }

    //prepare some intermediate arrays of matricies
    _XMFLOAT4X4* globalPose = new (std::nothrow) _XMFLOAT4X4[numNodes];
    if(!globalPose)
    {
        delete [] lFlattenedNodes;
        return;
    }
    _XMFLOAT4X4* invPose = new (std::nothrow) _XMFLOAT4X4[numNodes];
    if(!invPose)
    {
        delete [] lFlattenedNodes;
        delete [] globalPose;
        return;
    }

    FbxTime curTime = pTake->mLocalTimeSpan.GetStart();
    float startTime = static_cast<float>(curTime.GetSecondDouble());
    FbxTime period;
    // Set our desired sample-rate.
    // Time is specified in Hours, Minutes, Seconds, Frames, Fields
    period.SetTime(0, 0, 0, 1, 0);
    for(int i = 0; i < numKeys; ++i)
    {
        pAnim->mKeys[i].mBones = new (std::nothrow) BonePose[pAnim->mNumBones];
        if(!pAnim->mKeys[i].mBones)
        {
            delete [] lFlattenedNodes;
            delete [] globalPose;
            delete [] invPose;
            return;
        }
        //store the 'time-stamp' for this frame
        pAnim->mKeys[i].mTime = static_cast<float>(curTime.GetSecondDouble()) - startTime;

        for(int b = 0; b < numNodes; ++b)
        {
            // Ask the scene to 'evaluate' the global transform of the node at the specified time
            FbxMatrix m = pScene->GetAnimationEvaluator()->GetNodeGlobalTransform(lFlattenedNodes[b], curTime);
            // note that we are only scaling the translation when we convert to our own matrix format
            _XMMATRIX mb(static_cast<float>(m[0][0] * 1.00), static_cast<float>(m[0][1] * 1.00), static_cast<float>(m[0][2] * 1.00), static_cast<float>(m[0][3] * 1.00),
                static_cast<float>(m[1][0] * 1.00), static_cast<float>(m[1][1] * 1.00), static_cast<float>(m[1][2] * 1.00), static_cast<float>(m[1][3] * 1.00),
                static_cast<float>(m[2][0] * 1.00), static_cast<float>(m[2][1] * 1.00), static_cast<float>(m[2][2] * 1.00), static_cast<float>(m[2][3] * 1.00),
                static_cast<float>(m[3][0] * 0.01), static_cast<float>(m[3][1] * 0.01), static_cast<float>(m[3][2] * 0.01), static_cast<float>(m[3][3] * 1.00));

            XMStoreFloat4x4(&globalPose[b], mb);

            XMVECTOR det;
            XMMATRIX invMb = XMMatrixInverse(&det, mb);
            XMStoreFloat4x4(&invPose[b], invMb);

            XMMATRIX lb = XMLoadFloat4x4(&globalPose[b]);
            if(pAnim->mSkeleton[b].parent >= 0)
            {
                // get the local joint tranform
                lb *= XMLoadFloat4x4(&invPose[pAnim->mSkeleton[b].parent]);
            }

            //store the individual components
            XMVECTOR scale;
            XMVECTOR rotQuat;
            XMVECTOR trans;
            if(XMMatrixDecompose(&scale, &rotQuat, &trans, lb))
            {
                XMStoreFloat3(&pAnim->mKeys[i].mBones[b].translation, trans);
                XMStoreFloat4(&pAnim->mKeys[i].mBones[b].rotation, rotQuat);
                XMStoreFloat3(&pAnim->mKeys[i].mBones[b].scale, scale);
            }
        }
        pAnim->mDuration = static_cast<float>(curTime.GetSecondDouble()) - startTime;
        //advance our time period
        curTime += period;

        ++pAnim->mNumKeys;
    }

    delete [] lFlattenedNodes;
    delete [] globalPose;
    delete [] invPose;
}

SkeletalAnimation* LoadAnimationFromFbx(const char* szFileName)
{
	SkeletalAnimation* pAnim = NULL;

	// Create the io settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(g_FbxSdkManager, IOSROOT);
    g_FbxSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(g_FbxSdkManager,"");
    
    // Use the first argument as the filename for the importer.
    if(!lImporter->Initialize(szFileName, -1, g_FbxSdkManager->GetIOSettings())) {
        OutputDebugStringA("Call to FbxImporter::Initialize() failed.\n");
        OutputDebugStringA("Error returned:");
//        OutputDebugStringA(lImporter->GetLastErrorString());
        OutputDebugStringA("\n\n");
        exit(-1);
    }
    
    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(g_FbxSdkManager,"myScene");

    // Import the contents of the file into the scene.
	if(lImporter->Import(lScene))
	{
        FbxNode* lRootNode = lScene->GetRootNode();
        int numBones = CountSkeletonRecursive(lRootNode);
        if(numBones > 0)
        {
            pAnim = new (std::nothrow) SkeletalAnimation();
	        if(pAnim)
	        {
                pAnim->mSkeleton = new (std::nothrow) Bone[numBones];
                if(pAnim->mSkeleton)
                {
                    // First load the skeletal hierarchy
                    LoadSkeletonRecursive2(lRootNode, pAnim->mSkeleton, &pAnim->mNumBones, -1);

                    // Then go through the stack of takes and load each one
                    for(int i = 0; i < lImporter->GetAnimStackCount(); ++i)
                    {
                        LoadAnimationTake(lImporter->GetTakeInfo(i), lScene, pAnim);
                    }
                }

                if(!pAnim->mKeys || !pAnim->mSkeleton)
                {
                    delete pAnim;
                    pAnim = NULL;
                }
            }
        }
    }

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();
    ios->Destroy();

    lScene->Destroy();

    return(pAnim);
}
