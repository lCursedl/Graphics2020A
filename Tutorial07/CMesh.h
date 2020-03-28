#pragma once

#include "Define.h"
#include "CBuffer.h"
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CMaterial.h"
#include <vector>
#include <Windows.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class CMesh
{
#ifdef D3D11
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
#elif OPENGL
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	CMesh(std::vector<Vertex>v, std::vector<unsigned int>i, std::vector<Texture>t);
	void Draw(int shaderID);

private:
	unsigned int VBO, EBO;

	void setupMesh();
#endif	
};