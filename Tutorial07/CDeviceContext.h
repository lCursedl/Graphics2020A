#pragma once
#include "Define.h"
#include "Includes.h"
class CDeviceContext
{
public:
	static CDeviceContext * getInstance();
	/** \fn static CDeviceContext * getInstance()
	*	\brief Function to get instance object.
	*	\return a pointer to the instance.
	*/

	void operator = (CDeviceContext const&) = delete;
	void init();
	void * getDeviceContext();
	/** \fn void * getDeviceContext()
	*	\brief Function to get a void pointer to instance.
	*	\return void pointer to instance.
	*/

#ifdef D3D11
	ID3D11DeviceContext * m_DeviceContext;	/**< Pointer to a DirectX Device Context */
#endif // D3D11

private:
	CDeviceContext();
	static CDeviceContext * m_DCInstance;	/**< Pointer to the instance which is used for the Singleton */
};
/** \class CDeviceContext
*	\brief Class which contains a Singleton for a DirectX Device Context implementation.
*
*	Instance can be initialized using a pointer for the class and setting it to NULL.
*	Then when needed, make a pointer of the class and assign its value given by getInstance().
*/