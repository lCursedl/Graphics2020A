#include "CMesh.h"

#ifdef D3D11
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
	m_NumBones = 0;
}

CMesh::~CMesh()
{
}

void CMesh::init()
{
	m_VB = new CBuffer();
	m_IB = new CBuffer();
}

void CMesh::render(CDeviceContext * DC)
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

WORD *& CMesh::getIndexList()
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
#elif OPENGL
CMesh::CMesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t)
{
	this->vertices = v;
	this->indices = i;
	this->textures = t;
	setupMesh();
}

void CMesh::Draw(int shaderID)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaderID, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}
void CMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}
#endif // D3D11

void VertexBoneData::addBoneData(unsigned int boneid, float weight)
{
	for (unsigned int i = 0; i < NUM_BONES_PERVERTEX; i++)
	{
		if (Weights[i] == 0.f)
		{
			BoneID[i] = boneid;
			Weights[i] = weight;
			return;
		}
	}
}