#pragma once
#include "CBuffer.h"

class CIndexBuffer
{
public:
	void init(SubresourceData s, BufferStruct b);
	void * getIB();
	CBuffer m_Buffer;
#ifdef D3D11
	D3D11_SUBRESOURCE_DATA m_Data;
#endif // D3D11
};