#pragma once

#include "Define.h"
#include "Includes.h"

#include "include/assimp/cimport.h"
#include "include/assimp/Importer.hpp"
#include "include/assimp/material.h"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

struct BufferStruct
{
	unsigned int byteWidth;				/**< unsigned int variable */
	USAGE usage;						/**< USAGE which defines the buffer usage */
	unsigned int bindFlags;				/**< unsigned int which defines the flags of the buffer*/
	unsigned int cpuAccessFlags;		/**< unsigned int variable */
	unsigned int miscFlags;				/**< unsigned int variable */
	unsigned int structureByteStride;	/**< unsigned int variable */
};
/**	\struct BufferStruct
*	\brief Structure which defines the required attributes to initialize a Buffer
*/
class CBuffer
{
public:
	CBuffer();
	void init(BufferStruct B);
	/** \fn void init(BufferStruct B)
	*	\brief Receives a BufferStruct to copy its values.
	*	@param[in] B BufferStruct with the desired information.
	*/
	BufferStruct m_bufferstruct;	/**< Local variable which stores the information received from init(BufferStruct B) */
	void * getBuffer();
	/** \fn void * getBuffer()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
#ifdef D3D11
	D3D11_BUFFER_DESC m_bd;			/**< DirectX structure to store data */
	ID3D11Buffer * m_pBuffer;		/**< Pointer to a DirectX Buffer */
	
	static void createVertexBuffer(int numVertex, const aiScene* model, SimpleVertex* vertex, ID3D11Buffer*& buffer, ID3D11Device* Dev);
	/** \fn static void createVertexBuffer(int numVertex, const aiScene* model, SimpleVertex* vertex, ID3D11Buffer*& buffer, ID3D11Device* Dev)
	*	\brief Function for creating a VB for model loading.
	*	@param[in] numVertex int with vertex amount.
	*	@param[in] model aiScene* with model info.
	*	@param[in] vertex SimpleVertex* which has stored data.
	*	@param[in] buffer ID3D11Buffer* where the processed data will be stored.
	*	@param[in] Dev ID3D11Buffer double pointer which creates the buffer.
	*/
	static void createIndexBuffer(int numIndex, const aiScene* model, WORD* index, ID3D11Buffer*& buffer, ID3D11Device* Dev);
	/** \fn static void createIndexBuffer(int numIndex, const aiScene* model, WORD* index, ID3D11Buffer*& buffer, ID3D11Device* Dev)
	*	\brief Function for creating a IB for model loading.
	*	@param[in] numIndex int with index amount.
	*	@param[in] model aiScene* with model info.
	*	@param[in] index WORD* which has stored data.
	*	@param[in] buffer ID3D11Buffer* where the processed data will be stored.
	*	@param[in] Dev ID3D11Buffer double pointer which creates the buffer.
	*/
#endif // D3D11
};
/** \class CBuffer
*	\brief Class which contains basic data for buffers.
*
*	Can be used for all buffers except Vertex and Index buffers.
*/