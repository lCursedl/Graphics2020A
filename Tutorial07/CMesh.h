#pragma once

#include "Define.h"
#include "Includes.h"

#include "CBuffer.h"
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CMaterial.h"
#include <vector>
#include <Windows.h>

#ifdef  OPENGL
struct Vertex
{
	glm::vec3 Position;						/**< Vec3 Vertex position vector */
	glm::vec3 Normal;						/**< Vec3 Vertex normal vector */
	glm::vec2 TexCoords;					/**< Vec2 Vertex texcoords vector */
	glm::vec3 Tangent;						/**< Vec3 Vertex tangent vector */
	glm::vec3 Bitangent;					/**< Vec3 Vertex bitangent vector */
};
/**	\struct Vertex
*	\brief Structure which defines the required attributes for a Vertex
*/

struct Texture
{
	unsigned int id;						/**< unsigned int Texture id */
	std::string type;						/**< string Texture type */
	std::string path;						/**< string Texture path */
};
/**	\struct Texture
*	\brief Structure which defines the required attributes for a Texture
*/
#endif // D3D11

class CMesh
{
#ifdef D3D11
public:
	CMesh();
	~CMesh();

	void init();
	/** \fn void init()
	*	\brief Allocates Mesh VB & IB
	*/

	void render(CDeviceContext * DC);
	/** \fn void render(CDeviceContext * DC, CBuffer * bufferData, CDevice * D)
	*	\brief Draws the Mesh
	*	@param[in] DC CDeviceContext pointer for drawing operations
	*/

	void Delete();
	/** \fn void Delete()
	*	\brief Checks for allocated Mesh resources and deletes them
	*/

	void addChildren(CMesh * M);
	/** \fn void addChildren(CMesh * M)
	*	\brief Adds children Mesh into the Mesh container
	*/

	void setParent(CMesh* P);
	/** \fn void setParent(CMesh* P)
	*	\brief Sets the mesh parent
	*	@param[in] P CMesh* which is the parent
	*/

	void setVertex(SimpleVertex* V, int num);
	/** \fn void setVertex(SimpleVertex* V, int num)
	*	\brief Sets the Vertex info and number
	*	@param[in] V SimpleVertex* which has the info
	*	@param[in] num int which has the number
	*/

	void setIndexList(WORD* I, int num);
	/** \fn void setIndexList(WORD* I, int num)
	*	\brief Sets the Index info and number
	*	@param[in] I WORD* which has the info
	*	@param[in] num int which has the number
	*/

	WORD*& getIndexList();
	/** \fn WORD*& getIndexList()
	*	\brief Get the Index array
	*	\return a pointer to WORD with the Indices
	*/

	SimpleVertex* getVertex();
	/** \fn SimpleVertex* getVertex()
	*	\brief Get the Vertex array
	*	\return a pointer to SimpleVertex with the Vertices
	*/

	int getVertexNum();
	/** \fn int getVertexNum()
	*	\brief Get the number of vertices in the Mesh
	*	\return an int with the amount of vertices
	*/

	int getIndexNum();
	/** \fn int getIndexNum()
	*	\brief Get the number of indices in the Mesh
	*	\return an int with the amount of indices
	*/

	CBuffer*& getVB();
	/** \fn CBuffer*& getVB()
	*	\brief Returns the Vertex Buffer of the Mesh
	*	\return a pointer to CBuffer with the Mesh Vertex Buffer data
	*/

	CBuffer*& getIB();
	/** \fn CBuffer*& getIB()
	*	\brief Get the Index Buffer of the Mesh
	*	\return a pointer to CBuffer with the Mesh Index Buffer data
	*/

	void setVB(CBuffer* pVB);
	/** \fn void setVB(CBuffer* pVB)
	*	\brief Sets the Mesh Vertex Buffer
	*	@param[in] pVB CBuffer* with Vertex Buffer data
	*/

	void setIB(CBuffer* pIB);
	/** \fn setIB(CBuffer* pIB)
	*	\brief Sets the Mesh Index Buffer
	*	@param[in] pIB CBuffer* with Index Buffer data
	*/

	void setSceneID(int ID);
	/** \fn void setSceneID(int ID)
	*	\brief Sets the Mesh Scene id
	*	@param[in] ID int with the number
	*/

	int getSceneID();
	/** \fn int getSceneID()
	*	\brief Get the Mesh Scene ID
	*	\return an int with the ID
	*/


	std::string			m_DiffuseName;		/**< string with the diffuse material name */
	glm::vec4			m_Transform;		/**< Vec4 matrix for transfor operations */
	CMaterial*			m_Materials;		/**< CMaterial pointer to the array of the Mesh Materials */
	CMesh*				m_Parent;			/**< CMesh pointer to the Mesh Parent */
	std::vector<CMesh*>	m_Children;			/**< vector of CMesh pointers which contains the Mesh children */
	CBuffer*			m_VB;				/**< CBuffer pointer to the Mesh Vertex Buffer */
	CBuffer*			m_IB;				/**< CBuffer pointer to the Mesh Index Buffer */
	SimpleVertex*		m_MyVertex;			/**< SimpleVertex pointer to the array of vertices */
	WORD*				m_MyIndex;			/**< WORD pointer to the array of indices */
	int					m_VertexNum;		/**< int which defines the amount of vertices */
	int					m_IndexNum;			/**< int which defines the amount of indices */
	int					m_SceneID;			/**< int which defines the Scene ID of the Mesh */

	CBChangesEveryFrame	m_MeshData;			/**< Structure which contains data for a Constant Buffer */
#elif OPENGL
public:
	std::vector<Vertex> vertices;			/**< vector of Vertex which contains the vertices */
	std::vector<unsigned int> indices;		/**< vector of unsigned int which contains the indices */
	std::vector<Texture> textures;			/**< vector to Texture which contains the Mesh textures */
	unsigned int VAO;						/**< unsigned int which defines the Mesh VAO */

	CMesh(std::vector<Vertex>v, std::vector<unsigned int>i, std::vector<Texture>t);
	/** \fn CMesh(std::vector<Vertex>v, std::vector<unsigned int>i, std::vector<Texture>t)
	*	\brief Mesh class constructor
	*	@param[in] v std::vector<Vertex> which contains the Mesh vertices
	*	@param[in] i std::vector<unsigned int> which contains the Mesh indices
	*	@param[in] t std::vector<Texture> which contains the Mesh textures
	*/

	void Draw(int shaderID);
	/** \fn Draw(int shaderID)
	*	\brief Draws the Mesh
	*	@param[in] ShaderID int which defines the ID of the shader to use
	*/

private:
	unsigned int VBO;						/**< unsigned int which defines the Mesh VAO */
	unsigned int EBO;						/**< unsigned int which defines the Mesh EBO */

	void setupMesh();
	/** \fn setupMesh()
	*	\brief Sets the Mesh VAO, VBO and EBO for later use
	*/
#endif
};
/** \class CMesh
*	\brief Class which contains data for Mesh creation/usage.
*/