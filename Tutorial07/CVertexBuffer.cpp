#include "CVertexBuffer.h"

void CVertexBuffer::init(SubresourceData s, BufferStruct b)
{
	m_Buffer.init(b);
#ifdef D3D11
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_Data.pSysMem = s.psysMem;
#endif
}

void * CVertexBuffer::getVB()
{
#ifdef D3D11
	return m_Buffer.getBuffer();
#else
	return nullptr;
#endif
}
