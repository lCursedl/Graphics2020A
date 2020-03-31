#pragma once
#include "Define.h"
#include "Includes.h"
struct Texture2DStruct
{
	unsigned int W;					/**< unsigned int texture Width */
	unsigned int H;					/**< unsigned int texture Height */
	unsigned int mipLevels;			/**< unsigned int mipmap levels */
	unsigned int arraySize;			/**< unsigned int variable */
	FORMAT format;					/**< FORMAT variable */
	SAMPLEDESC sampleDesc;			/**< SAMPLEDESC variable */
	USAGE usage;					/**< USAGE variable */
	unsigned int flags;				/**< unsigned int texture flag */
	unsigned int cpuAccessFlags;	/**< unsigned int variable */
	unsigned int miscFlags;			/**< unsigned int variable */
};
/**	\struct Texture2DStruct
*	\brief Structure which defines the required attributes to initialize a 2D texture
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
/** \class CBuffer
*	\brief Class which contains basic data for 2D texture creation/use.
*/