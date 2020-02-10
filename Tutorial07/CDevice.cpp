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