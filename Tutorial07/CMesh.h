#pragma once

#include "Define.h"
#include "CBuffer.h"
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CMaterial.h"
#include <vector>
#include <Windows.h>


class CMesh
{
public:
	CMesh();
	~CMesh();

	void init();
	void update();
	void render(CDeviceContext * DC, CBuffer * bufferData, CDevice * D);
	void Delete();
	void addChildren(CMesh * M);
	void setParent(CMesh* P);
	void setVertex(SimpleVertex* V, int num);
	void setIndexList(WORD* I, int num);
	WORD*& getIndexList(SimpleVertex* V, int num);
	SimpleVertex* getVertex();
	int getVertexNum();
	int getIndexNum();
	CBuffer*& getVB();
	CBuffer*& getIB();
	void setVB(CBuffer* pVB);
	void setIB(CBuffer* pIB);
	void setSceneID(int ID);
	int getSceneID();

	std::string			m_DiffuseName;
	glm::vec4			m_Transform;
	CMaterial*			m_Materials;
	CMesh*				m_Parent;
	std::vector<CMesh*>	m_Children;
	CBuffer*			m_VB;
	CBuffer*			m_IB;
	SimpleVertex*		m_MyVertex;
	WORD*				m_MyIndex;
	int					m_VertexNum;
	int					m_IndexNum;
	int					m_SceneID;

	CBChangesEveryFrame	m_MeshData;
};