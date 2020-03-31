#pragma once
#include "Define.h"
#include "Includes.h"
struct RenderTargetViewStruct
{
	FORMAT format;								/**< FORMAT variable */
	RTV_DIMENSION viewDimension;				/**< RTV_DIMENSION variable */
	union
	{
		BUFFER_RTV buffer;						/**< BUFFER_RTV variable */
		TEX1D_RTV texture1D;					/**< TEX1D_RTV variable */
		TEX1D_ARRAY_RTV texture1DArray;			/**< TEX1D_ARRAY_RTV variable */
		TEX2D_RTV texture2D;					/**< TEX2D_RTV variable */
		TEX2D_ARRAY_RTV texture2DArray;			/**< TEX2D_ARRAY_RTV variable */
		TEX2DMS_RTV texture2DMS;				/**< TEX2DMS_RTV variable */
		TEX2DMS_ARRAY_RTV texture2DMSArray;		/**< TEX2DMS_ARRAY_RTV variable */
		TEX3D_RTV texture3D;					/**< TEX3D_RTV variable */
	};
};

class CRenderTargetView
{
public:
	CRenderTargetView();
	void init(RenderTargetViewStruct S);
	/** \fn void init(RenderTargetViewStruct S)
	*	\brief Receives a RenderTargetViewStruct to copy its values.
	*	@param[in] S RenderTargetViewStruct with the desired information.
	*/

	void * getRTV();
	/** \fn void * getRTV()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
#ifdef D3D11
	ID3D11RenderTargetView * m_pRTV;			/**< Pointer to a DirectX RenderTargetView */
	D3D11_RENDER_TARGET_VIEW_DESC m_Desc;		/**< DirectX structure to store data */
#endif // D3D11
};