#include "CSceneManager.h"

#ifdef D3D11
CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::addMesh(CMesh * M)
{
	M->setSceneID(m_ID);
	m_ID++;
	m_MeshList.push_back(M);
}

CMesh *& CSceneManager::getMesh(int ID)
{
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		if (m_MeshList[i]->getIndexNum() == ID)
		{
			return m_MeshList[i];
		}
	}
}

void CSceneManager::render(CDeviceContext * DC)
{
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		m_MeshList[i]->render(DC);
	}
}

void CSceneManager::shutdown()
{
	for (int i = 0; i < m_MeshList.size(); i++)
	{
		m_MeshList[i]->Delete();
	}
}
#endif // D3D11