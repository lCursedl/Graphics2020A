#pragma once
#include "Define.h"
#include "Includes.h"
struct DeviceStruct
{
	DRIVER_TYPE m_DriverType;			/**< DRIVER_TYPE which defines the driver type to create */
	DRIVER_TYPE m_DriverTypes[3];		/**< DRIVER_TYPE array of 3 elements */
	unsigned int m_DeviceFlags;			/**< unsigned int which defines the runtime layers to enable*/
	FEATURE_LEVEL m_FeatureLevels[3];	/**< FEATURE_LEVEL array of 3 elements which determine the order of feature levels to attempt to create */
	unsigned int m_numFeatureLevels;	/**< unsigned int which defines the amount of elements in m_FeatureLevels[]  */
};
/**	\struct DeviceStruct
*	\brief Structure which defines the required attributes to initialize a CDevice
*/

class CDevice
{
public:	
	~CDevice();
	
	static CDevice * getInstance();
	/** \fn static CDevice * getInstance()
	*	\brief Function to get instance object.
	*	\return a pointer to the instance.
	*/
	void operator = (CDevice const&) = delete;
	void init(DeviceStruct s);
	/** \fn void init(DeviceStruct s)
	*	\brief Receives a DeviceStruct to copy its values.
	*	@param[in] s DeviceStruct with the desired information.
	*/
	void * getDevice();
	/** \fn void * getDevice()
	*	\brief Function to get a void pointer to instance.
	*	\return void pointer to instance.
	*/
	DeviceStruct m_struc;				/**< Local variable which stores the information received from init(DeviceStruct s) */
#ifdef D3D11
public:
	ID3D11Device * m_Device;			/**< Pointer to a DirectX Device */
#endif // D3D11
private:
	CDevice();	
	static CDevice * m_DeviceInstance;	/**< Pointer to the instance which is used for the Singleton */
};
/** \class CDevice
*	\brief Class which contains a Singleton for a DirectX Device implementation.
*	
*	Instance can be initialized using a pointer for the class and setting it to NULL.
*	Then when needed, make a pointer of the class and assign its value given by getInstance().
*/