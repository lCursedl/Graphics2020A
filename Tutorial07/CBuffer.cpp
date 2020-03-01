#include "CBuffer.h"

CBuffer::CBuffer()
{
#ifdef D3D11
	m_pBuffer = NULL;
#endif
}

void CBuffer::init(BufferStruct B)
{
	m_bufferstruct = B;
#ifdef D3D11
	ZeroMemory(&m_bd, sizeof(m_bd));
	m_bd.Usage = (D3D11_USAGE)m_bufferstruct.usage;
	m_bd.ByteWidth = m_bufferstruct.byteWidth;
	m_bd.BindFlags = m_bufferstruct.bindFlags;
	m_bd.CPUAccessFlags = m_bufferstruct.cpuAccessFlags;
#endif
}

void * CBuffer::getBuffer()
{
#ifdef D3D11
	return m_pBuffer;
#else
	return nullptr;
#endif	
}