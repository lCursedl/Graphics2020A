#include "CDeviceContext.h"

CDeviceContext::CDeviceContext()
{
	m_DeviceContext = NULL;
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