#pragma once

#include "Define.h"
#include "Includes.h"
struct DeviceStruct
{
	DRIVER_TYPE m_DriverType;
	DRIVER_TYPE m_DriverTypes[3];
	unsigned int m_DeviceFlags;
	FEATURE_LEVEL m_FeatureLevels[3];
	unsigned int m_numFeatureLevels;
};

class CDevice
{
public:	
	~CDevice();

	static CDevice * getInstance();
	void operator = (CDevice const&) = delete;
	void init(DeviceStruct s);
	void * getDevice();
	DeviceStruct m_struc;
#ifdef D3D11
	ID3D11Device * m_Device;
#endif // D3D11
private:
	CDevice();	
	static CDevice * m_DeviceInstance;
};