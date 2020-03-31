#pragma once
#include "Define.h"
#include "Includes.h"
class CVertexShader
{
public:
	CVertexShader();
	void * getBlob();
	/** \fn void * getBlob()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/

	void * getVS();
	/** \fn void * getVS()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/

	void * getInputLayout();
	/** \fn void * getInputLayout()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/

#ifdef D3D11
	ID3DBlob * m_pVSBlob;					/**< Pointer to compiled info from VS */
	ID3D11VertexShader * m_pVertexShader;	/**< Pointer to DirectX VS */
	ID3D11InputLayout * m_pInputLayout;		/**< Pointer to DirectX InputLayout */
#endif // D3D11
};
/** \class CVertexShader
*	\brief Class which contains data for VertexShader usage.
*/