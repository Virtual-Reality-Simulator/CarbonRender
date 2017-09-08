#include "..\Inc\CRFbxImportManager.h"

FbxImportManager* FbxImportManager::ins = nullptr;

FbxImportManager::FbxImportManager()
{
	Init();
}

FbxImportManager::~FbxImportManager()
{
	fbxManager->Destroy();
	ioSettings->Destroy();
	ins = nullptr;	
}

FbxImportManager * FbxImportManager::Instance()
{
	if (ins == nullptr)
		ins = new FbxImportManager();

	return ins;
}

void FbxImportManager::Init()
{
	fbxManager = FbxManager::Create();
	ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSettings);
}

FbxColor FbxImportManager::ReadColor(FbxMesh * mesh, int index, int vertexID)
{
	if (mesh->GetElementVertexColorCount() <= 0)
		return FbxColor(0.0f, 0.0f, 0.0f, 1.0f);

	FbxGeometryElementVertexColor* eVertexColor = mesh->GetElementVertexColor(0);
	FbxColor color;
	switch (eVertexColor->GetMappingMode())
	{
	default:
		break;
	case FbxGeometryElement::eByControlPoint:
	{
		switch (eVertexColor->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			color = eVertexColor->GetDirectArray().GetAt(index);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eVertexColor->GetIndexArray().GetAt(index);
			color = eVertexColor->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (eVertexColor->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			color = eVertexColor->GetDirectArray().GetAt(vertexID);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eVertexColor->GetIndexArray().GetAt(vertexID);
			color = eVertexColor->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}
	return color;
}

float4 FbxImportManager::ReadUV(FbxMesh * mesh, int index, int uvIndex)
{
	float4 uvf(-1.0f, -1.0f, -1.0f, -1.0f);
	int elementCount = mesh->GetElementUVCount();
	if (elementCount <= 0)
		return uvf;

	float uva[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
	
	for (int i = 0; i < elementCount; i++)
	{
		FbxGeometryElementUV* eUV = mesh->GetElementUV(i);
		FbxVector2 uv;

		switch (eUV->GetMappingMode())
		{
		default:
			break;
		case FbxGeometryElement::eByControlPoint:
		{
			switch (eUV->GetReferenceMode())
			{
			default:
				break;
			case FbxGeometryElement::eDirect:
				uv = eUV->GetDirectArray().GetAt(index);
			break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = eUV->GetIndexArray().GetAt(index);
				uv = eUV->GetDirectArray().GetAt(id);
			}
			break;
			}
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			
			switch (eUV->GetReferenceMode())
			{
			default:
				break;
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
				uv = eUV->GetDirectArray().GetAt(uvIndex);
			break;
			}
		}
		break;
		}

		uva[i * 2] = uv[0];
		uva[i * 2 + 1] = uv[1];
	}

	uvf.x = uva[0];
	uvf.y = uva[1];
	uvf.z = uva[2];
	uvf.w = uva[3];
	
	return uvf;
}

FbxVector4 FbxImportManager::ReadNormal(FbxMesh * mesh, int index, int vertexID)
{
	FbxVector4 n(0.0f, 0.0f, 1.0f, 1.0f);
	if (mesh->GetElementNormalCount() <= 0)
		return n;

	FbxGeometryElementNormal* eN = mesh->GetElementNormal(0);
	switch (eN->GetMappingMode())
	{
	default:
		break;
	case FbxGeometryElement::eByControlPoint:
	{
		switch (eN->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			n = eN->GetDirectArray().GetAt(index);
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eN->GetIndexArray().GetAt(index);
			n = eN->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (eN->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			n = eN->GetDirectArray().GetAt(vertexID);
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eN->GetIndexArray().GetAt(vertexID);
			n = eN->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return n;
}

FbxVector4 FbxImportManager::ReadTangent(FbxMesh * mesh, int index, int vertexID)
{
	FbxVector4 t(1.0f, 0.0f, 0.0f, 1.0f);
	if (mesh->GetElementTangentCount() <= 0)
		return t;

	FbxGeometryElementTangent* eT = mesh->GetElementTangent(0);
	switch (eT->GetMappingMode())
	{
	default:
		break;
	case FbxGeometryElement::eByControlPoint:
	{
		switch (eT->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			t = eT->GetDirectArray().GetAt(index);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eT->GetIndexArray().GetAt(index);
			t = eT->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (eT->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			t = eT->GetDirectArray().GetAt(vertexID);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eT->GetIndexArray().GetAt(vertexID);
			t = eT->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return t;
}

FbxVector4 FbxImportManager::ReadBinormal(FbxMesh * mesh, int index, int vertexID)
{
	FbxVector4 b(1.0f, 0.0f, 0.0f, 1.0f);
	if (mesh->GetElementBinormalCount() <= 0)
		return b;

	FbxGeometryElementBinormal* eB = mesh->GetElementBinormal(0);
	switch (eB->GetMappingMode())
	{
	default:
		break;
	case FbxGeometryElement::eByControlPoint:
	{
		switch (eB->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			b = eB->GetDirectArray().GetAt(index);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eB->GetIndexArray().GetAt(index);
			b = eB->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (eB->GetReferenceMode())
		{
		default:
			break;
		case FbxGeometryElement::eDirect:
			b = eB->GetDirectArray().GetAt(vertexID);
			break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int id = eB->GetIndexArray().GetAt(vertexID);
			b = eB->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return b;
}

void FbxImportManager::ReadTexture(FbxMesh* mesh, Mesh* crMesh, char* meshFileName)
{
	if (mesh->GetNode()->GetMaterialCount() > 0)
	{
		//surport just 1 material on 1 mesh for now
		FbxSurfaceMaterial* mat = mesh->GetNode()->GetMaterial(0);
		FbxProperty p;

		p = mat->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
		if (p.IsValid())
		{
			crMesh->texs[0] = 0;
			crMesh->texs[1] = 0;
			crMesh->texs[2] = 0;
			int texCount = p.GetSrcObjectCount<FbxTexture>();
			for (int k = 0; k < texCount; k++)
			{
				FbxTexture* fbxTex = FbxCast<FbxTexture>(p.GetSrcObject<FbxTexture>(k));
				if (fbxTex && strlen(fbxTex->GetInitialName()) != 0)
				{
					char* fullName = FileReader::BindString(meshFileName, "\\");
					fullName = FileReader::BindString(fullName, (char*)fbxTex->GetInitialName());
					GLuint tex = TextureManager::Instance()->LoadTexture(fullName);
					if (strstr(fbxTex->GetInitialName(), "_D"))
						crMesh->texs[0] = tex;
					if (strstr(fbxTex->GetInitialName(), "_N"))
						crMesh->texs[1] = tex;
					if (strstr(fbxTex->GetInitialName(), "_S"))
						crMesh->texs[2] = tex;
				}
			}

			if (crMesh->texs[0] == 0)
				crMesh->texs[0] = TextureManager::Instance()->LoadDefaultD();
			if (crMesh->texs[1] == 0)
				crMesh->texs[1] = TextureManager::Instance()->LoadDefaultN();
			if (crMesh->texs[2] == 0)
				crMesh->texs[2] = TextureManager::Instance()->LoadDefaultS();

			return;
		}
	}

	crMesh->texs[0] = TextureManager::Instance()->LoadDefaultD();
	crMesh->texs[1] = TextureManager::Instance()->LoadDefaultN();
	crMesh->texs[2] = TextureManager::Instance()->LoadDefaultS();
	
}

bool FbxImportManager::ImportFbxModel(char * fileName, MeshObject * out, bool loadTex)
{
	char* dir = "Resources\\Models\\";
	char* fullName = FileReader::BindString(dir, fileName);
	fullName = FileReader::BindString(fullName, ".fbx");
	FbxImporter* importer = FbxImporter::Create(this->fbxManager, "");
	if (!importer->Initialize(fullName, -1, this->fbxManager->GetIOSettings()))
	{
		cout << "Fbx Importer init fail" << endl;
		cout << "Error Log: " << importer->GetStatus().GetErrorString();
		return false;
	}

	FbxScene* scene = FbxScene::Create(this->fbxManager, fullName);
	importer->Import(scene);
	importer->Destroy();

	FbxNode* rootNode = scene->GetRootNode();
	int meshCount = scene->GetGeometryCount();
	int skeletonCount = rootNode->GetChildCount() - meshCount;

	out->SetChildCount(meshCount);

	if (rootNode != NULL)
	{
		int readMeshCount = 0;
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* node = rootNode->GetChild(i);

			if (node->GetNodeAttribute())
			{
				switch (node->GetNodeAttribute()->GetAttributeType())
				{
				default:
					break;
				case FbxNodeAttribute::eMesh:
				{
					FbxDouble3 translation = node->LclTranslation.Get();
					FbxDouble3 rotation = node->LclRotation.Get();
					FbxDouble3 scaling = node->LclScaling.Get();

					FbxMesh* mesh = node->GetMesh();
					Mesh crMesh;
					crMesh.name = mesh->GetName();
					crMesh.translation = translation;
					crMesh.rotation = rotation;
					crMesh.scaling = scaling;
					crMesh.modelMatrix = CalculateModelMatrix(crMesh.localCoord, translation, rotation, scaling);
					crMesh.polygonCount = mesh->GetPolygonCount();
					crMesh.vertexCount = crMesh.polygonCount * 3;
					crMesh.vertex = new float[crMesh.vertexCount * 3];
					crMesh.color = new float[crMesh.vertexCount * 4];
					crMesh.uv = new float[crMesh.vertexCount * 4];
					crMesh.index = new unsigned int[crMesh.polygonCount * 3];
					crMesh.normal = new float[crMesh.vertexCount * 3];
					crMesh.tangent = new float[crMesh.vertexCount * 3];
					crMesh.binormal = new float[crMesh.vertexCount * 3];

					//Get Textures
					if (loadTex)
						ReadTexture(mesh, &crMesh, fileName);
					else
					{
						crMesh.texs[0] = TextureManager::Instance()->LoadDefaultD();
						crMesh.texs[1] = TextureManager::Instance()->LoadDefaultN();
						crMesh.texs[2] = TextureManager::Instance()->LoadDefaultS();
					}

					int vertexID;
					FbxDouble4* ctrlPoints = mesh->GetControlPoints();

					for (int i = 0; i < mesh->GetPolygonCount(); i++)
					{
						for (int j = 0; j < 3; j++)
						{
							vertexID = i * 3 + j;
							unsigned int index = (unsigned int)mesh->GetPolygonVertex(i, j);
							crMesh.index[vertexID] = vertexID;

							//Get Vertex
							crMesh.vertex[vertexID * 3] = (float)ctrlPoints[index][0];
							crMesh.vertex[vertexID * 3 + 1] = (float)ctrlPoints[index][1];
							crMesh.vertex[vertexID * 3 + 2] = (float)ctrlPoints[index][2];

							//Get Color
							FbxColor color = ReadColor(mesh, index, vertexID);
							crMesh.color[vertexID * 4] = color.mRed;
							crMesh.color[vertexID * 4 + 1] = color.mGreen;
							crMesh.color[vertexID * 4 + 2] = color.mBlue;
							crMesh.color[vertexID * 4 + 3] = color.mAlpha;

							//Get UV
							int uvIndex = mesh->GetTextureUVIndex(i, j);
							float4 uv = ReadUV(mesh, index, uvIndex);
							crMesh.uv[vertexID * 4] = uv.x;
							crMesh.uv[vertexID * 4 + 1] = uv.y;
							crMesh.uv[vertexID * 4 + 2] = uv.z;
							crMesh.uv[vertexID * 4 + 3] = uv.w;

							//Get Normal
							FbxVector4 n = ReadNormal(mesh, index, vertexID);
							crMesh.normal[vertexID * 3] = n[0];
							crMesh.normal[vertexID * 3 + 1] = n[1];
							crMesh.normal[vertexID * 3 + 2] = n[2];

							//Get Tangent
							FbxVector4 t = ReadTangent(mesh, index, vertexID);
							crMesh.tangent[vertexID * 3] = t[0];
							crMesh.tangent[vertexID * 3 + 1] = t[1];
							crMesh.tangent[vertexID * 3 + 2] = t[2];

							//Get Binormal
							FbxVector4 b = ReadBinormal(mesh, index, vertexID);
							crMesh.binormal[vertexID * 3] = b[0];
							crMesh.binormal[vertexID * 3 + 1] = b[1];
							crMesh.binormal[vertexID * 3 + 2] = b[2];
						}
					}

					out->SetChild(crMesh, readMeshCount);
					readMeshCount++;
				}
				break;
				case FbxNodeAttribute::eSkeleton:
				{

				}
				break;
				}
			}
		}
	}

	return true;
}
