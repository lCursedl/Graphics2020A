#pragma once
#include "Define.h"
#include "Includes.h"
struct SAMPLER_DESC
{
	FILTER filter;							/**< FILTER which defines filtering method to use */
	TEXTURE_ADDRESS_MODE addresU;			/**< TEXTURE_ADDRESS_MODE which defines method to use for resolving a U texture coord that is outside of range */
	TEXTURE_ADDRESS_MODE addresV;			/**< TEXTURE_ADDRESS_MODE which defines method to use for resolving a V texture coord that is outside of range */
	TEXTURE_ADDRESS_MODE addresW;			/**< TEXTURE_ADDRESS_MODE which defines method to use for resolving a W texture coord that is outside of range */
	float mipLODBias;						/**< float which defines offset from the calculated mipmap level */
	unsigned int maxAnisotropy;				/**< unsigned int which defines clamping value used if FILTER_ANISOTROPIC or FILTER_COMPARISON_ANISOTROPIC is specified in Filter */
	COMPARISON_FUNC comparisonFunc;			/**< COMPARISON_FUN which defines a function that compares sampled data against existing sampled data */
	float borderColor[4];					/**< float which defines border color to use if TEXTURE_ADDRESS_BORDER is specified for addresU, addresV, or addresW */
	float minLOD;							/**< float which defines lower end of the mipmap range to clamp access to */
	float maxLOD;							/**< float which defines upper end of the mipmap range to clamp access to */
};
/**	\struct SAMPLER_DESC
*	\brief Structure which defines the required attributes to initialize a CSamplerState
*/

class CSamplerState
{
public:
	CSamplerState();
	void init(SAMPLER_DESC S);
	/** \fn void init(SAMPLER_DESC S)
	*	\brief Receives a BufferStruct to copy its values.
	*	@param[in] S SAMPLER_DESC with the desired information.
	*/
	void * getSampler();
	/** \fn void * getSampler()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/
#ifdef D3D11
	ID3D11SamplerState * m_pSamplerLinear;	/**< Pointer to a DirectX SamplerState */
	D3D11_SAMPLER_DESC m_Desc;				/**< DirectX structure to store data */
#endif // D3D11
};
/** \class CSamplerState
*	\brief Class which contains data for SamplerState use.
*/