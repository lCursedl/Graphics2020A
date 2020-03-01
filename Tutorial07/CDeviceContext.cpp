#include "CDeviceContext.h"

CDeviceContext::CDeviceContext()
{
#ifdef D3D11
	m_DeviceContext = NULL;
#endif
}

CDeviceContext * CDeviceContext::getInstance()
{
	if (m_DCInstance == nullptr)
	{
		m_DCInstance = new CDeviceContext();
	}
	return m_DCInstance;
}

void CDeviceContext::init()
{
}

void * CDeviceContext::getDeviceContext()
{
#ifdef D3D11
	return m_DeviceContext;
#else
	return nullptr;
#endif // D3D11	
}
