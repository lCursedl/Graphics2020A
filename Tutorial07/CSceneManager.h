#pragma once

#include "CMesh.h"

class CSceneManager
{
#ifdef D3D11
public:
	CSceneManager();
	~CSceneManager();

	void addMesh(CMesh * M);
	/** \fn void addMesh(CMesh * M)
	*	\brief Adds the specified Mesh to the Scene
	*	@param[in] M CMesh pointer with the Mesh to add
	*/

	CMesh*& getMesh(int ID);
	/** \fn CMesh*& getMesh(int ID)
	*	\brief Gets a specified Mesh from the Scene
	*	@param[in] ID ID from the Mesh to retrieve
	*	\return a pointer to the required Mesh
	*/

	void render(CDeviceContext * DC);
	/** \fn void render(CDeviceContext * DC, CDevice* D)
	*	\brief Renders the Scene
	*	@param[in] DC CDeviceContext pointer for drawing operations
	*/

	void shutdown();
	/** \fn void shutdown()
	*	\brief Navigates the vector of Meshes and calls their Delete() function
	*/

	std::vector<CMesh*> m_MeshList;		/**< vector<CMesh*> which contains pointers to the Scene Meshes */
	int m_ID;							/**< int which defines the Scene ID */
#endif
};
/** \class CSceneManager
*	\brief Class which contains data for Scene creation and Mesh management.
*/