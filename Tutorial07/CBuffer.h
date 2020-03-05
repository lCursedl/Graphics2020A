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
	unsigned int byteWidth;
	USAGE usage;
	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
	unsigned int structureByteStride;
};
//Class which contains a ID3D11Buffer pointer
class CBuffer
{
public:
	CBuffer();
	void init(BufferStruct B);
	BufferStruct m_bufferstruct;
	void * getBuffer();
	
	

#ifdef D3D11
	D3D11_BUFFER_DESC m_bd;
	ID3D11Buffer * m_pBuffer;
	
	static void createVertexBuffer(int numVertex, const aiScene* model, SimpleVertex* vertex, ID3D11Buffer*& buffer, void* Dev);
	static void createIndexBuffer(int numIndex, const aiScene* model, WORD* index, ID3D11Buffer*& buffer, void* Dev);

#endif // D3D11
};