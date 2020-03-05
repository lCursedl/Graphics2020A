#pragma once

#include "CMesh.h"

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	void addMesh(CMesh * M);
	CMesh*& getMesh(int ID);
	void update();
	void render(CDeviceContext * DC, CBuffer* B, CDevice* D);
	void shutdown();

	CBuffer * m_meshData;
	std::vector<CMesh*> m_MeshList;
	int m_ID;
};