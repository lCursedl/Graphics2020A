#pragma once
#include "CSceneManager.h"

#include "include/assimp/cimport.h"
#include "include/assimp/Importer.hpp"
#include "include/assimp/material.h"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

class CGraphicsAPI
{
public:
	CGraphicsAPI();
	~CGraphicsAPI();

	const aiScene* m_Model = new const aiScene();
	Assimp::Importer* m_Importer = new Assimp::Importer();

	bool loadMesh(const char* path, CSceneManager* SM, const aiScene* model, CDeviceContext* DC, Assimp::Importer* importer, void *dev);

private:
	void meshRead(const aiScene* model, CMesh* mesh, int index, void * dev);
	void readMeshTexture(const aiScene* model, CMesh* mesh, int index, void* dev);
};