#pragma once
#include "Define.h"
#include "Includes.h"
struct DepthStencilViewStruct
{
	FORMAT format;									/**< FORMAT variable */
	DSV_DIMENSION viewDimension;					/**< DSV_DIMENSION variable */
	unsigned int flags;								/**< unsigned int DepthstencilView flags */
	union
	{
		TEX1D_DSV textureID;						/**< TEX1D_DSV variable */
		TEX1D_ARRAY_DSV texture1DArray;				/**< TEX1D_ARRAY_DSV variable */
		TEX2D_DSV texture2D;						/**< TEX2D_DSV variable */
		TEX2D_ARRAY_DSV texture2DArray;				/**< TEX2D_ARRAY_DSV variable */
		TEX2DMS_DSV texture2DMS;					/**< TEX2DMS_DSV variable */
		TEX2DMS_ARRAY_DSV texture2DMSArray;			/**< TEX2DMS_ARRAY_DSV variable */
	};
};
/**	\struct DepthStencilViewStruct
*	\brief Structure which defines the required attributes to initialize a Depth Stencil View
*/

class CDepthStencilView
{
public:
	CDepthStencilView();
	void init(DepthStencilViewStruct D, FORMAT F);
	/** \fn void init(DepthStencilViewStruct D, FORMAT F B)
	*	\brief Receives a DepthStencilViewStruct and FORMAT to copy its values.
	*	@param[in] D DepthStencilViewStruct with the desired information.
	*	@param[in] F FORMAT with the desired format information
	*/
	void * getDSV();
	/** \fn void * getDSV()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
#ifdef D3D11
	ID3D11DepthStencilView * m_pDepthStencilView;	/**< Pointer to a DirectX DepthStencilView */
	D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc;			/**< DirectX structure to store data */
#endif // D3D11
};
/** \class CDepthStencilView
*	\brief Class which contains data for Depth Stencil View creation/usage.
*/