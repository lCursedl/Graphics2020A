#pragma once
#include "Define.h"
#include "Includes.h"
struct ViewportStruct
{
	float topLeftX;					/**< float which defines the initial x position */
	float topLeftY;					/**< float which defines the initial y position */
	float W;						/**< float which defines the width of the area */
	float H;						/**< float which defines the height of the area */
	float minDepth;					/**< float which defines the minimum depth */
	float maxDepth;					/**< float which defines the maxmimum depth */
};
/**	\struct ViewportStruct
*	\brief Structure which defines the required attributes to initialize a CViewport
*/

class CViewport
{
public:
	CViewport();
	void init(ViewportStruct V);
	/** \fn void init(ViewportStruct V)
	*	\brief Receives a ViewportStruct to copy its values.
	*	@param[in] V ViewportStruct with the desired information.
	*/

#ifdef D3D11
	D3D11_VIEWPORT m_Viewport;		/**< Viewport object of Directx */
#endif // D3D11
};
/** \class CViewport
*	\brief Class which contains data for Viewport usage.
*/