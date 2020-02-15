#pragma once
#include "CBuffer.h"

struct SubresourceData
{
	const void * psysMem;
	unsigned int sysMemPitch;
	unsigned int sysMemSlicePitch;
};

class CVertexBuffer
{
public:

	void init(SubresourceData s, BufferStruct b);

	CBuffer m_Buffer;
	SubresourceData m_Data;
#ifdef D3D11
	D3D11_SUBRESOURCE_DATA m_D3D11Data;
#endif // D3D11
};