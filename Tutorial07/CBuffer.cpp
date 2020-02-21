#include "CBuffer.h"

CBuffer::CBuffer()
{
	m_pBuffer = NULL;
}

void CBuffer::init(BufferStruct B)
{
	m_bufferstruct = B;

	ZeroMemory(&m_bd, sizeof(m_bd));
	m_bd.Usage = (D3D11_USAGE)m_bufferstruct.usage;
	m_bd.ByteWidth = m_bufferstruct.byteWidth;
	m_bd.BindFlags = m_bufferstruct.bindFlags;
	m_bd.CPUAccessFlags = m_bufferstruct.cpuAccessFlags;
}