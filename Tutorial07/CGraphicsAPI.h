#pragma once
#include "CSceneManager.h"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

class CGraphicsAPI
{
#ifdef D3D11
public:
	CGraphicsAPI();
	~CGraphicsAPI();

	const aiScene* m_Model = new  aiScene();				/**< aiScenete pointer for the Model */
	
	//Assimp::Importer* m_Importer = new Assimp::Importer();		/**< Importer pointer for Model loading operations */	

	const aiScene* loadMesh(const char* path, CSceneManager* SM, const aiScene* model, CDeviceContext* DC, CDevice *dev, Assimp::Importer* Imp);
	/** \fn bool loadMesh(const char* path, CSceneManager* SM, const aiScene* model, CDeviceContext* DC, Assimp::Importer* importer, CDevice *dev)
	*	\brief
	*	@param[in] path
	*	@param[in] SM
	*	@param[in] model
	*	@param[in] DC
	*	@param[in] dev
	*/

	void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms, const aiScene * model, CSceneManager * sc);

private:
	void meshRead(const aiScene* model, CMesh* mesh, int index, CDevice * dev);
	/** \fn void meshRead(const aiScene* model, CMesh* mesh, int index, CDevice * dev)
	*	\brief Reads the model info and creates the data for the Mesh
	*	@param[in] model aiScene pointer from where info is obtained
	*	@param[in] mesh CMesh pointer which contains a Mesh from the model
	*	@param[in] index int number to identify the Mesh
	*	@param[in] dev CDevice pointer for VB & IB creation
	*/

	void readMeshTexture(const aiScene* model, CMesh* mesh, int index, CDevice* dev);
	/** \fn void readMeshTexture(const aiScene* model, CMesh* mesh, int index, CDevice* dev)
	*	\brief Reads the model material info and creates the data for the Mesh
	*	@param[in] model aiScene pointer from where info is obtained
	*	@param[in] mesh CMesh pointer which contains a Mesh from the model
	*	@param[in] index int number to identify the Mesh
	*	@param[in] dev CDevice pointer for ShaderResource creation
	*/

	const aiNodeAnim* FindAnimationNode(const aiAnimation * animation, const std::string node);

	void ReadNodeHierarchy(float animTime, const aiNode * node, const glm::mat4 transformParent, const aiScene * model, CMesh * mesh);

	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);

	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
#endif // D3D11
};
/** \class CGraphicsAPI
*	\brief Class which manages model loading and Scene creation/usage.
*/