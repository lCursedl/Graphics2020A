#pragma once
#include "Define.h"

class CDeviceContext
{
public:
	static CDeviceContext * getInstance();
	void operator = (CDeviceContext const&) = delete;
	void init();
	void * getDeviceContext();
#ifdef D3D11
	ID3D11DeviceContext * m_DeviceContext;
#endif // D3D11

private:
	CDeviceContext();
	static CDeviceContext * m_DCInstance;
};