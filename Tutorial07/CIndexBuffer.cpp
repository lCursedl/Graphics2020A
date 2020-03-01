#include "CIndexBuffer.h"

void CIndexBuffer::init(SubresourceData s, BufferStruct b)
{
	m_Buffer.init(b);
#ifdef D3D11
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_Data.pSysMem = s.psysMem;
#endif
}

void * CIndexBuffer::getIB()
{
#ifdef D3D11
	return m_Buffer.getBuffer();
#else
	return nullptr;
#endif // D3D11
}
