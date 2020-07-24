#include "CGraphicsAPI.h"
#ifdef D3D11
CGraphicsAPI::CGraphicsAPI() {}

CGraphicsAPI::~CGraphicsAPI() {}

const aiScene* CGraphicsAPI::loadMesh(const char * path, CSceneManager * SM, const aiScene * model, CDeviceContext * DC, CDevice * dev, Assimp::Importer* Imp)
{
	model = Imp->ReadFile(path, aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded | aiProcess_FindInstances
		| aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes | aiProcess_Debone);

	if (model == nullptr)
	{
		return false;
	}

	CMesh * newMesh = new CMesh();
	char * token = NULL;
	char * nexttoken = NULL;
	std::string spath = path;

	token = strtok_s((char*)spath.c_str(), "/", &nexttoken);
	std::string texturesFolder = token;
	texturesFolder += "\\";
	token = (char*)texturesFolder.c_str();
	newMesh->m_Materials->m_TextureDir = token;

	std::string dirName = newMesh->m_Materials->m_TextureDir;

	meshRead(model, newMesh, 0, dev);
	readMeshTexture(model, newMesh, 0, dev);
	SM->addMesh(newMesh);

	if (model->mNumMeshes > 1)
	{
		for (int i = 1; i < model->mNumMeshes; i++)
		{
			CMesh* childMesh = new CMesh();
			childMesh->setParent(newMesh);
			newMesh->addChildren(childMesh);
			childMesh->m_Materials->m_TextureDir = dirName;
			meshRead(model, childMesh, i, dev);
			readMeshTexture(model, childMesh, i, dev);
			SM->addMesh(childMesh);
		}
	}
	return model;
}

void CGraphicsAPI::meshRead(const aiScene * model, CMesh * mesh, int index, CDevice * dev)
{
	std::vector <std::uint32_t> indis;
	indis.reserve(model->mMeshes[index]->mNumFaces * 3);
	int numVertex = model->mMeshes[index]->mNumVertices;
	int numIndex = model->mMeshes[index]->mNumFaces * 3;
	int mNumBones = (model->mMeshes[index]->mNumBones);
	mesh->m_BonesPerVertex.resize(numVertex);

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numIndex];

	for (unsigned int i = 0; i < mNumBones; i++)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(model->mMeshes[index]->mBones[i]->mName.data);

		if (mesh->m_BoneMapping.find(BoneName) == mesh->m_BoneMapping.end())
		{
			BoneIndex = mesh->m_NumBones;
			mesh->m_NumBones++;
			BoneInfo bi;
			mesh->m_Boneinfo.push_back(bi);
		}
		else
		{
			BoneIndex = mesh->m_BoneMapping[BoneName];
		}

		mesh->m_BoneMapping[BoneName] = BoneIndex;
		//glm matrix is column major, assimp matrix is row major transpose
		mesh->m_Boneinfo[BoneIndex].BoneOffset = glm::transpose(glm::make_mat4(&model->mMeshes[index]->mBones[i]->mOffsetMatrix.a1));
		
		for (int j = 0; j < model->mMeshes[index]->mBones[i]->mNumWeights; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				meshVertex[model->mMeshes[index]->mBones[i]->mWeights[j].mVertexId].BoneID[k] = BoneIndex;
				meshVertex[model->mMeshes[index]->mBones[i]->mWeights[j].mVertexId].Weights[k] = model->mMeshes[index]->mBones[i]->mWeights[j].mWeight;
			}			
		}
	}

	for (std::uint32_t faceIdx = 0u; faceIdx < model->mMeshes[index]->mNumFaces; faceIdx++)
	{
		indis.push_back(model->mMeshes[index]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(model->mMeshes[index]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(model->mMeshes[index]->mFaces[faceIdx].mIndices[2u]);
	}	

	for (int i = 0; i < numVertex; i++)
	{
		meshVertex[i].msPos.x = model->mMeshes[index]->mVertices[i].x;
		meshVertex[i].msPos.y = model->mMeshes[index]->mVertices[i].y;
		meshVertex[i].msPos.z = model->mMeshes[index]->mVertices[i].z;
		meshVertex[i].texcoord.x = model->mMeshes[index]->mTextureCoords[0][i].x;
		meshVertex[i].texcoord.y = model->mMeshes[index]->mTextureCoords[0][i].y;
		/*meshVertex[i].msNormal.x = model->mMeshes[index]->mNormals[i].x;
		meshVertex[i].msNormal.y = model->mMeshes[index]->mNormals[i].y;
		meshVertex[i].msNormal.z = model->mMeshes[index]->mNormals[i].z;
		meshVertex[i].msNormal.w = 1;
		meshVertex[i].msBinormal.x = model->mMeshes[index]->mBitangents[i].x;
		meshVertex[i].msBinormal.y = model->mMeshes[index]->mBitangents[i].y;
		meshVertex[i].msBinormal.z = model->mMeshes[index]->mBitangents[i].z;
		meshVertex[i].msTangent.x = model->mMeshes[index]->mTangents[i].x;
		meshVertex[i].msTangent.y = model->mMeshes[index]->mTangents[i].y;
		meshVertex[i].msTangent.z = model->mMeshes[index]->mTangents[i].z;*/
	}

	mesh->setVertex(meshVertex, numVertex);
#ifdef D3D11
	//Create VB
	CBuffer::createVertexBuffer(numVertex, model, meshVertex, mesh->getVB()->m_pBuffer, dev->m_Device);
#endif

	for (int i = 0; i < numIndex; i++)
	{
		meshIndex[i] = (WORD)indis[i];
	}

	mesh->setIndexList(meshIndex, numIndex);
#ifdef D3D11
	//Create IB
	CBuffer::createIndexBuffer(numIndex, model, meshIndex, mesh->getIB()->m_pBuffer, dev->m_Device);
#endif // D3D11
}

void CGraphicsAPI::readMeshTexture(const aiScene * model, CMesh * mesh, int index, CDevice * dev)
{
	const aiMaterial* pMaterial = model->mMaterials[model->mMeshes[index]->mMaterialIndex];

	//Diffuse texture
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString Path;
		mesh->m_Materials->m_HasDiffuse = true;
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			mesh->m_Materials->m_Diffuse = mesh->m_Materials->m_TextureDir;
			mesh->m_Materials->m_Diffuse = (std::string)Path.data;
			wchar_t wtext[50];
			char* token = NULL;
			char* nextToken = NULL;
			token = strtok_s((char*)mesh->m_Materials->m_Diffuse.c_str(), ".", &nextToken);
			std::string pngextention = token;
			pngextention += ".png";
			token = (char*)pngextention.c_str();

			LPCSTR direcTexture = token;
			LPCWSTR dir = (LPCWSTR)pngextention.c_str();

			int stringLength = MultiByteToWideChar(CP_ACP, 0, pngextention.data(), pngextention.length(), 0, 0);
			std::wstring wstr(stringLength, 0);
			MultiByteToWideChar(CP_ACP, 0, pngextention.data(), pngextention.length(), &wstr[0], stringLength);
			dir = (LPCWSTR)wstr.c_str();

#ifdef D3D11
			D3DX11CreateShaderResourceViewFromFile(dev->m_Device, dir, NULL, NULL, &mesh->m_Materials->m_TextureDiffuse, NULL);
#endif // D3D11
		}
	}
}
const aiNodeAnim * CGraphicsAPI::FindAnimationNode(const aiAnimation * animation, const std::string node)
{
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		const aiNodeAnim * T = animation->mChannels[i];
		if (std::string(T->mNodeName.data) == node)
		{
			return T;
		}
	}
	return nullptr;
}
void CGraphicsAPI::ReadNodeHierarchy(float animTime, const aiNode * node, const glm::mat4 transformParent, const aiScene * model, CMesh * mesh)
{
	std::string NodeName(node->mName.data);
	const aiAnimation * anim = model->mAnimations[0];

	//transpose
	glm::mat4 glmTransform = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
	
	const aiNodeAnim * animNode = FindAnimationNode(anim, NodeName);

	if (animNode)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, animTime, animNode);
		glm::mat4 ScalingM = glm::mat4(1.f);
		//Translate the values from aiVector3D to glm::vec3
		glm::vec3 ScalingGLM(Scaling.x, Scaling.y, Scaling.z);
		glm::scale(ScalingM, ScalingGLM);

		//Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animTime, animNode);
		glm::quat RotationQGLM(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		glm::mat4 RotationM = glm::toMat4(RotationQGLM);

		//Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, animTime, animNode);
		glm::mat4 TranslationM = glm::mat4(1.f);
		TranslationM[3][0] = Translation.x;
		TranslationM[3][1] = Translation.y;
		TranslationM[3][2] = Translation.z;

		//Combine The above transformations
		glmTransform = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = transformParent * glmTransform;

	if (mesh->m_BoneMapping.find(NodeName) != mesh->m_BoneMapping.end())
	{
		unsigned int BoneIndex = mesh->m_BoneMapping[NodeName];
		mesh->m_Boneinfo[BoneIndex].FinalTransformation = GlobalTransformation * mesh->m_Boneinfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ReadNodeHierarchy(animTime, node->mChildren[i], GlobalTransformation, model, mesh);
	}
}

unsigned int CGraphicsAPI::FindPosition(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int CGraphicsAPI::FindRotation(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int CGraphicsAPI::FindScaling(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

void CGraphicsAPI::CalcInterpolatedPosition(aiVector3D & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void CGraphicsAPI::CalcInterpolatedRotation(aiQuaternion & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void CGraphicsAPI::CalcInterpolatedScaling(aiVector3D & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void CGraphicsAPI::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms, const aiScene * model,CSceneManager * sc)
{
	glm::mat4 Identity = glm::mat4(1.f);
	int TotalBones = 0;
	for (int i = 0; i < sc->m_MeshList.size(); i++)
	{
		TotalBones += sc->m_MeshList.at(i)->m_NumBones;
	}

	float TicksPerSecond = model->mAnimations[0]->mTicksPerSecond != 0 ?
							model->mAnimations[0]->mTicksPerSecond : 25.f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, model->mAnimations[0]->mDuration);

	for (int i = 0; i < sc->m_MeshList.size(); i++)
	{
		ReadNodeHierarchy(AnimationTime, model->mRootNode, glm::mat4(1.f), model, sc->m_MeshList[i]);
	}	

	Transforms.resize(TotalBones);

	for (unsigned int i = 0; i < TotalBones; i++)
	{
		for (int j = 0; j < sc->m_MeshList.size(); j++)
		{
			for (int k = 0; k < sc->m_MeshList[j]->m_Boneinfo.size(); k++)
			{
				Transforms[i] = sc->m_MeshList[j]->m_Boneinfo[k].FinalTransformation;
				i++;
			}
		}		
	}
}
#endif // D3D11