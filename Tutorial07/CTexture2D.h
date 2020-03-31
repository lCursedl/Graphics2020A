#pragma once
#include "Define.h"
#include "Includes.h"
struct Texture2DStruct
{
	unsigned int W;					/**< unsigned int texture Width in texels */
	unsigned int H;					/**< unsigned int texture Height in texels */
	unsigned int mipLevels;			/**< unsigned int maximum number of mipmap levels in the texture */
	unsigned int arraySize;			/**< unsigned int number of textures in the texture array */
	FORMAT format;					/**< FORMAT which defines texture format */
	SAMPLEDESC sampleDesc;			/**< SAMPLEDESC which specifies multisampling parameters for the texture */
	USAGE usage;					/**< USAGE which defines how the texture is to be read from and written to */
	unsigned int flags;				/**< unsigned int which defines flags for binding to pipeline stages */
	unsigned int cpuAccessFlags;	/**< unsigned int which specifies the types the types of CPU access allowed */
	unsigned int miscFlags;			/**< unsigned int which identifies less common resource options */
};
/**	\struct Texture2DStruct
*	\brief Structure which defines the required attributes to initialize a CTexture2D
*/

class CTexture2D
{
public:

	CTexture2D();
	void init(Texture2DStruct S);
	/** \fn void init(Texture2DStruct S)
	*	\brief Receives a Texture2DStruct to copy its values.
	*	@param[in] S Texture2DStruct with the desired information.
	*/

	Texture2DStruct m_Data;			/**< Local variable which stores the information received from init(Texture2DStruct S) */
	void * getTexture();
	/** \fn void * getTexture()
	*	\brief Function to get a void pointer to object.
	*	\return void pointer to object.
	*/

#ifdef D3D11
	ID3D11Texture2D * m_pTexture;	/**< Pointer to a DirectX 2D Texture */
	D3D11_TEXTURE2D_DESC m_Desc;	/**< DirectX structure to store data */
#endif // D3D11
};
/** \class CTexture2D
*	\brief Class which contains basic data for 2D texture creation/use.
*/