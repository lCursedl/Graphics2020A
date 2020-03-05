#pragma once
#include "CBuffer.h"
#include "Includes.h"
//Class which contains a CBuffer
class CVertexBuffer
{
public:

	void init(SubresourceData s, BufferStruct b);
	void * getVB();
	CBuffer m_Buffer;
#ifdef D3D11
	D3D11_SUBRESOURCE_DATA m_Data;
#endif // D3D11
};