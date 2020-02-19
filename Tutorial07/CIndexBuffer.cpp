#include "CIndexBuffer.h"

void CIndexBuffer::init(SubresourceData s, BufferStruct b)
{
	m_Buffer.init(b);
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_Data.pSysMem = s.psysMem;
}
