#pragma once
#include "CBuffer.h"
#include "Includes.h"
class CIndexBuffer
{
public:
	void init(SubresourceData s, BufferStruct b);
	/** \fn void init(SubresourceData s, BufferStruct b)
	*	\brief Receives a BufferStruct and SubresourceData to copy its values.
	*	@param[in] B BufferStruct with the desired information.
	*	@param[in] s SubresourceData with the desired information.
	*/
	void * getIB();
	/** \fn void * getIB()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
	CBuffer m_Buffer;				/**< CBuffer object */
#ifdef D3D11
	D3D11_SUBRESOURCE_DATA m_Data;	/**< DirectX structure to store data */
#endif // D3D11
};
/** \class CIndexBuffer
*	\brief Class which contains data for creating and using IndexBuffers.
*
*	Can be used only for IndexBuffers.
*/