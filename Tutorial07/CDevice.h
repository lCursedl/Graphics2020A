#pragma once

#include "Define.h"

struct DeviceStruct
{
#ifdef D3D11
	D3D_DRIVER_TYPE m_DriverType;
	D3D_DRIVER_TYPE m_DriverTypes[3];
	UINT m_DeviceFlags;
	D3D_FEATURE_LEVEL m_FeatureLevels[3];
	UINT m_numFeatureLevels;
#endif // D3D11
};

class CDevice
{
public:	
	~CDevice();

	static CDevice * getInstance();
	void operator = (CDevice const&) = delete;
	void init(DeviceStruct s);
	DeviceStruct m_struc;
#ifdef D3D11
	ID3D11Device * m_Device;
#endif // D3D11
private:
	CDevice();	
	static CDevice * m_DeviceInstance;
};