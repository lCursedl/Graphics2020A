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

#ifdef D3D11
void CBuffer::createVertexBuffer(int numVertex, const aiScene * model, SimpleVertex * vertex, ID3D11Buffer *& buffer, ID3D11Device * Dev)
{

	//ID3D11Device * tempdevice = static_cast<ID3D11Device*>(Dev);
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertex;
	HRESULT hr = Dev->CreateBuffer(&bd, &data, &buffer);
	if (FAILED(hr))
	{
		return;
	}


}

void CBuffer::createIndexBuffer(int numIndex, const aiScene * model, WORD * index, ID3D11Buffer *& buffer, ID3D11Device * Dev)
{
	//ID3D11Device * tempdevice = static_cast<ID3D11Device*>(Dev);
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = index;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	HRESULT hr = Dev->CreateBuffer(&bd, &data, &buffer);
	if (FAILED(hr))
	{
		return;
	}
}
#endif // D3D11