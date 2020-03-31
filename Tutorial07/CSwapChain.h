#pragma once
#include "Define.h"
#include "Includes.h"
struct SwapChainStruct
{
	unsigned int bufferCount;			/**< unsigned int which defines the number of buffers */
	unsigned int W;						/**< unsigned int which defines the width */
	unsigned int H;						/**< unsigned int which defines the height */
	FORMAT format;						/**< FORMAT variable */
	unsigned int refreshNumerator;		/**< unsigned int for calculating refresh rate */
	unsigned int refreshDenominator;	/**< unsigned int for calculating refresh rate */
	unsigned int bufferUsage;			/**< unsigned int for defining usage */
	unsigned int outputWND;				/**< unsigned int which saves the output window reference */
	unsigned int count;					/**< unsigned int variable */
	unsigned int quality;				/**< unsigned int variable */
	bool windowed;						/**< bool which defines if window is or not windowed */
};
/**	\struct SwapChainStruct
*	\brief Structure which defines the required attributes to initialize a Swap Chain
*/

class CSwapChain
{
public:

	void init(SwapChainStruct s);
	/** \fn void init(SwapChainStruct s)
	*	\brief Receives a SwapChainStruct to copy its values.
	*	@param[in] s SwapChainStruct with the desired information.
	*/

	static CSwapChain * getInstance();
	/** \fn static CSwapChain * getInstance()
	*	\brief Function to get instance object.
	*	\return a pointer to the instance.
	*/

	void operator = (CSwapChain const&) = delete;
	void * getSwapChain();
	/** \fn void * getSwapChain()
	*	\brief Function to get a void pointer to instance.
	*	\return void pointer to instance.
	*/

#ifdef D3D11
	IDXGISwapChain * m_pSwapChain;		/**< Pointer to a DirectX Swap Chain */
	DXGI_SWAP_CHAIN_DESC m_sd;			/**< DirectX's Swap Chain structure */
#endif // D3D11

private:
	CSwapChain();
	static CSwapChain * m_pSCInstance;	/**< Pointer to the instance which is used for the Singleton */
};
/** \class CSwapChain
*	\brief Class which contains a Singleton for a DirectX Swap Chain implementation.
*
*	Instance can be initialized using a pointer for the class and setting it to NULL.
*	Then when needed, make a pointer of the class and assign its value given by getInstance().
*/