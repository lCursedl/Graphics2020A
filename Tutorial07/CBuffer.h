#pragma once

#include "Define.h"

struct BufferStruct
{
	unsigned int byteWidth;
	USAGE usage;
	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
	unsigned int structureByteStride;
};

class CBuffer
{
public:
	CBuffer();
	void init(BufferStruct B);
	BufferStruct m_bufferstruct;
#ifdef D3D11
	D3D11_BUFFER_DESC m_bd;
	ID3D11Buffer * m_pBuffer;
#endif // D3D11
};