#include "CDevice.h"

CDevice::CDevice(){}

CDevice::~CDevice(){}

 CDevice * CDevice::getInstance()
{
	 if (m_DeviceInstance == nullptr)
	 {
		 m_DeviceInstance = new CDevice();
	 }
	 return m_DeviceInstance;
}

 void CDevice::init(DeviceStruct s)
 {
	 m_struc = s;
 }

 void * CDevice::getDevice()
 {
#ifdef D3D11
	 return m_Device;
#else
	 return nullptr;
#endif // D3D11
 }
