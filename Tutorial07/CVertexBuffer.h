#pragma once
#include "CBuffer.h"
#include "Includes.h"
class CVertexBuffer
{
public:
	void init(SubresourceData s, BufferStruct b);
	/** \fn void init(SubresourceData s, BufferStruct b)
	*	\brief Receives a BufferStruct and SubresourceData to copy its values.
	*	@param[in] B BufferStruct with the desired information.
	*	@param[in] s SubresourceData with the desired information.
	*/
	void * getVB();
	/** \fn void * getVB()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
	CBuffer m_Buffer;				/**< CBuffer object */
#ifdef D3D11
	D3D11_SUBRESOURCE_DATA m_Data;	/**< DirectX structure to store data */
#endif // D3D11
};
/** \class CVertexBuffer
*	\brief Class which contains data for creating and using VertexBuffers.
*
*	Can be used only for VertexBuffers.
*/