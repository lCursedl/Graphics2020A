#include "CVertexBuffer.h"

void CVertexBuffer::init(SubresourceData s, BufferStruct b)
{
	m_Buffer.init(b);
	m_Data = s;

	ZeroMemory(&m_D3D11Data, sizeof(m_D3D11Data));
	m_D3D11Data.pSysMem = m_Data.psysMem;
}