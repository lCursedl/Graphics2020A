#pragma once
#include "Define.h"
#include "Includes.h"
class CPixelShader
{
public:
	CPixelShader();
	void * getPS();
	/** \fn void * getPS()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/

#ifdef D3D11
	ID3DBlob * m_pPSBlob;				/**< Pointer to compiled info from PS */
	ID3D11PixelShader * m_pPixelShader;	/**< Pointer to DirectX PS */
#endif // D3D11
};
/** \class CPixelShader
*	\brief Class which contains data for PixelShader usage.
*/