#include "CMesh.h"

CMesh::CMesh()
{
	m_VB = nullptr;
	m_IB = nullptr;
	m_MyVertex = nullptr;
	m_MyIndex = nullptr;
	m_VertexNum = 0;
	m_IndexNum = 0;
	m_SceneID = -1;

	m_VB = new CBuffer();
	m_IB = new CBuffer();
	m_Materials = new CMaterial();
	m_MeshData.mWorld =
	{
		1, 0, 0, .010,
		0, 1, 0, .010,
		0, 0, 1, .010,
		0, 0, 0, 1
	};
	m_MeshData.vMeshColor = { 1, 0, 0, 1 };
}

CMesh::~CMesh()
{
}

void CMesh::init()
{
	m_VB = new CBuffer();
	m_IB = new CBuffer();
}

void CMesh::update()
{
}

void CMesh::render(CDeviceContext * DC, CBuffer * bufferData, CDevice * D)
{
	m_MeshData.mWorld =
	{
		1, 0, 0, .010,
		0, 1, 0, .010,
		0, 0, 1, .010,
		0, 0, 0, 1
	};
	m_MeshData.vMeshColor = { 1, 0, 0, 1 };
#ifdef D3D11
	DC->m_DeviceContext->PSSetShaderResources(0, 1, &m_Materials->m_TextureDiffuse);
	DC->m_DeviceContext->VSSetShaderResources(0, 1, &m_Materials->m_TextureDiffuse);
#endif // D3D11
	

	std::string title;
	std::string title2;

	std::stringstream ss;

	ss << "mesh ";
	ss << m_SceneID;
	ss >> title;
	ss >> title2;

	title += " ";
	title += title2;

	unsigned int stride = sizeof(SimpleVertex);
	unsigned int offset = 0;

#ifdef D3D11
	DC->m_DeviceContext->IASetVertexBuffers(0, 1, &m_VB->m_pBuffer, &stride, &offset);
	DC->m_DeviceContext->IASetIndexBuffer(m_IB->m_pBuffer, (DXGI_FORMAT)FORMAT_R16_UINT, 0);

	DC->m_DeviceContext->DrawIndexed(m_IndexNum, 0, 0);
#endif // D3D11	
}

void CMesh::Delete()
{
	m_Materials->shutdown();
	if (m_VB != nullptr)
	{
		delete m_VB;
		m_VB = nullptr;
	}
	if (m_IB != nullptr)
	{
		delete m_IB;
		m_IB = nullptr;
	}
	if (m_MyVertex != nullptr)
	{
		delete m_MyVertex;
		m_MyVertex = nullptr;
	}
	if (m_MyIndex != nullptr)
	{
		delete m_MyIndex;
		m_MyIndex = nullptr;
	}
}

void CMesh::addChildren(CMesh * M)
{
	m_Children.push_back(M);
}

void CMesh::setParent(CMesh * P)
{
	m_Parent = P;
}

void CMesh::setVertex(SimpleVertex * V, int num)
{
	m_VertexNum = num;
	m_MyVertex = V;
}

void CMesh::setIndexList(WORD * I, int num)
{
	m_IndexNum = num;
	m_MyIndex = I;
}

WORD *& CMesh::getIndexList(SimpleVertex * V, int num)
{
	return m_MyIndex;
}

SimpleVertex * CMesh::getVertex()
{
	return m_MyVertex;
}

int CMesh::getVertexNum()
{
	return m_VertexNum;
}

int CMesh::getIndexNum()
{
	return m_IndexNum;
}

CBuffer *& CMesh::getVB()
{
	return m_VB;
}

CBuffer *& CMesh::getIB()
{
	return m_IB;
}

void CMesh::setVB(CBuffer * pVB)
{
	m_VB = pVB;
}

void CMesh::setIB(CBuffer * pIB)
{
	m_IB = pIB;
}

void CMesh::setSceneID(int ID)
{
	m_SceneID = ID;
}

int CMesh::getSceneID()
{
	return m_SceneID;
}