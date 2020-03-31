#pragma once
#include "CSceneManager.h"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class CGraphicsAPI
{
#ifdef D3D11
public:
	CGraphicsAPI();
	~CGraphicsAPI();

	const aiScene* m_Model = new const aiScene();				/**< aiScenete pointer for the Model */

	Assimp::Importer* m_Importer = new Assimp::Importer();		/**< Importer pointer for Model loading operations */

	bool loadMesh(const char* path, CSceneManager* SM, const aiScene* model, CDeviceContext* DC, CDevice *dev);
	/** \fn bool loadMesh(const char* path, CSceneManager* SM, const aiScene* model, CDeviceContext* DC, Assimp::Importer* importer, CDevice *dev)
	*	\brief
	*	@param[in] path
	*	@param[in] SM
	*	@param[in] model
	*	@param[in] DC
	*	@param[in] dev
	*/

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
#endif // D3D11
};
/** \class CGraphicsAPI
*	\brief Class which manages model loading and Scene creation/usage.
*/