#pragma once

#include <sstream>
#include <fstream>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define PIDIV4           0.785398163f

/** \file Define.h
*	\brief	Header with declarations.
*
*	Header which includes required independent structures and enums for application.
*/

enum DRIVER_TYPE
{
	DRIVER_TYPE_UNKNOWN = 0,
	DRIVER_TYPE_HARDWARE = (DRIVER_TYPE_UNKNOWN + 1),
	DRIVER_TYPE_REFERENCE = (DRIVER_TYPE_HARDWARE + 1),
	DRIVER_TYPE_NULL = (DRIVER_TYPE_REFERENCE + 1),
	DRIVER_TYPE_SOFTWARE = (DRIVER_TYPE_NULL + 1),
	DRIVER_TYPE_WARP = (DRIVER_TYPE_SOFTWARE + 1)
};
/**	\enum DRIVER_TYPE
*	\brief Indepentent enum equivalent to DirectX's D3D11_DRIVER_TYPE
*/
enum FEATURE_LEVEL
{
	FEATURE_LEVEL_9_1 = 0x9100,
	FEATURE_LEVEL_9_2 = 0x9200,
	FEATURE_LEVEL_9_3 = 0x9300,
	FEATURE_LEVEL_10_0 = 0xa000,
	FEATURE_LEVEL_10_1 = 0xa100,
	FEATURE_LEVEL_11_0 = 0xb000
};
/**	\enum FEATURE_LEVEL
*	\brief Indepentent enum equivalent to DirectX's D3D11_FEATURE_LEVEL
*/

enum USAGE
{
	USAGE_DEFAULT,
	USAGE_INMUTABLE,
	USAGE_DYNAMIC,
	USAGE_STAGING
};
/**	\enum USAGE
*	\brief Indepentent enum equivalent to DirectX's D3D11_USAGE
*/
enum FORMAT
{
	FORMAT_UNKNOWN,
	FORMAT_R32G32B32A32_TYPELESS,
	FORMAT_R32G32B32A32_FLOAT,
	FORMAT_R32G32B32A32_UINT,
	FORMAT_R32G32B32A32_SINT,
	FORMAT_R32G32B32_TYPELESS,
	FORMAT_R32G32B32_FLOAT,
	FORMAT_R32G32B32_UINT,
	FORMAT_R32G32B32_SINT,
	FORMAT_R16G16B16A16_TYPELESS,
	FORMAT_R16G16B16A16_FLOAT,
	FORMAT_R16G16B16A16_UNORM,
	FORMAT_R16G16B16A16_UINT,
	FORMAT_R16G16B16A16_SNORM,
	FORMAT_R16G16B16A16_SINT,
	FORMAT_R32G32_TYPELESS,
	FORMAT_R32G32_FLOAT,
	FORMAT_R32G32_UINT,
	FORMAT_R32G32_SINT,
	FORMAT_R32G8X24_TYPELESS,
	FORMAT_D32_FLOAT_S8X24_UINT,
	FORMAT_R32_FLOAT_X8X24_TYPELESS,
	FORMAT_X32_TYPELESS_G8X24_UINT,
	FORMAT_R10G10B10A2_TYPELESS,
	FORMAT_R10G10B10A2_UNORM,
	FORMAT_R10G10B10A2_UINT,
	FORMAT_R11G11B10_FLOAT,
	FORMAT_R8G8B8A8_TYPELESS,
	FORMAT_R8G8B8A8_UNORM,
	FORMAT_R8G8B8A8_UNORM_SRGB,
	FORMAT_R8G8B8A8_UINT,
	FORMAT_R8G8B8A8_SNORM,
	FORMAT_R8G8B8A8_SINT,
	FORMAT_R16G16_TYPELESS,
	FORMAT_R16G16_FLOAT,
	FORMAT_R16G16_UNORM,
	FORMAT_R16G16_UINT,
	FORMAT_R16G16_SNORM,
	FORMAT_R16G16_SINT,
	FORMAT_R32_TYPELESS,
	FORMAT_D32_FLOAT,
	FORMAT_R32_FLOAT,
	FORMAT_R32_UINT,
	FORMAT_R32_SINT,
	FORMAT_R24G8_TYPELESS,
	FORMAT_D24_UNORM_S8_UINT,
	FORMAT_R24_UNORM_X8_TYPELESS,
	FORMAT_X24_TYPELESS_G8_UINT,
	FORMAT_R8G8_TYPELESS,
	FORMAT_R8G8_UNORM,
	FORMAT_R8G8_UINT,
	FORMAT_R8G8_SNORM,
	FORMAT_R8G8_SINT,
	FORMAT_R16_TYPELESS,
	FORMAT_R16_FLOAT,
	FORMAT_D16_UNORM,
	FORMAT_R16_UNORM,
	FORMAT_R16_UINT,
	FORMAT_R16_SNORM,
	FORMAT_R16_SINT,
	FORMAT_R8_TYPELESS,
	FORMAT_R8_UNORM,
	FORMAT_R8_UINT,
	FORMAT_R8_SNORM,
	FORMAT_R8_SINT,
	FORMAT_A8_UNORM,
	FORMAT_R1_UNORM,
	FORMAT_R9G9B9E5_SHAREDEXP,
	FORMAT_R8G8_B8G8_UNORM,
	FORMAT_G8R8_G8B8_UNORM,
	FORMAT_BC1_TYPELESS,
	FORMAT_BC1_UNORM,
	FORMAT_BC1_UNORM_SRGB,
	FORMAT_BC2_TYPELESS,
	FORMAT_BC2_UNORM,
	FORMAT_BC2_UNORM_SRGB,
	FORMAT_BC3_TYPELESS,
	FORMAT_BC3_UNORM,
	FORMAT_BC3_UNORM_SRGB,
	FORMAT_BC4_TYPELESS,
	FORMAT_BC4_UNORM,
	FORMAT_BC4_SNORM,
	FORMAT_BC5_TYPELESS,
	FORMAT_BC5_UNORM,
	FORMAT_BC5_SNORM,
	FORMAT_B5G6R5_UNORM,
	FORMAT_B5G5R5A1_UNORM,
	FORMAT_B8G8R8A8_UNORM,
	FORMAT_B8G8R8X8_UNORM,
	FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
	FORMAT_B8G8R8A8_TYPELESS,
	FORMAT_B8G8R8A8_UNORM_SRGB,
	FORMAT_B8G8R8X8_TYPELESS,
	FORMAT_B8G8R8X8_UNORM_SRGB,
	FORMAT_BC6H_TYPELESS,
	FORMAT_BC6H_UF16,
	FORMAT_BC6H_SF16,
	FORMAT_BC7_TYPELESS,
	FORMAT_BC7_UNORM,
	FORMAT_BC7_UNORM_SRGB,
	FORMAT_FORCE_UINT
};
/**	\enum FORMAT
*	\brief Indepentent enum equivalent to DirectX's D3D11_FORMAT
*/

enum DSV_DIMENSION
{
	DSV_DIMENSION_UNKNOWN = 0,
	DSV_DIMENSION_TEXTURE1D = 1,
	DSV_DIMENSION_TEXTURE1DARRAY = 2,
	DSV_DIMENSION_TEXTURE2D = 3,
	DSV_DIMENSION_TEXTURE2DARRAY = 4,
	DSV_DIMENSION_TEXTURE2DMS = 5,
	DSV_DIMENSION_TEXTURE2DMSARRAY = 6
};
/**	\enum DSV_DIMENSION
*	\brief Indepentent enum equivalent to DirectX's D3D11_DIMENSION
*/
struct TEX1D_DSV
{
	unsigned int mipSlice;
};
/**	\struct TEX1D_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX1D_DSV
*/

struct TEX1D_ARRAY_DSV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};
/**	\struct TEX1D_ARRAY_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX1D_ARRAY_DSV
*/

struct TEX2D_DSV
{
	unsigned int mipSlice;
};
/**	\struct TEX2D_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2D_DSV
*/

struct TEX2D_ARRAY_DSV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};
/**	\struct TEX2D_ARRAY_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2D_ARRAY_DSV
*/

struct TEX2DMS_DSV
{
	unsigned int unusedField_NothingToDefine;
};
/**	\struct TEX2DMS_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2DMS_DSV
*/

struct TEX2DMS_ARRAY_DSV
{
	unsigned int firstArraySize;
	unsigned int arraySize;
};
/**	\struct TEX2DMS_ARRAY_DSV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2DMS_ARRAY_DSV
*/

struct SAMPLEDESC
{
	unsigned int count;
	unsigned int quality;
};
/**	\struct SAMPLEDESC
*	\brief Indepentent structure equivalent to DirectX's D3D11_SAMPLEDESC
*/

struct SubresourceData
{
	const void * psysMem;			/**< const void* which defines a pointer to the initialization data */
	unsigned int sysMemPitch;		/**< unsigned int which defines the distance in bytes from the begginning of one line of a texture to the next one */
	unsigned int sysMemSlicePitch;	/**< unsigned int which defines the distance in bytes from the beginning of one depth level to the next */
};
/**	\struct SubresourceData
*	\brief Indepentent structure equivalent to DirectX's D3D11_SUBRESOURCE_DATA
*/

enum RTV_DIMENSION
{
	RTV_DIMENSION_UNKNOWN = 0,
	RTV_DIMENSION_BUFFER = 1,
	RTV_DIMENSION_TEXTURE1D = 2,
	RTV_DIMENSION_TEXTURE1DARRAY = 3,
	RTV_DIMENSION_TEXTURE2D = 4,
	RTV_DIMENSION_TEXTURE2DARRAY = 5,
	RTV_DIMENSION_TEXTURE2DMS = 6,
	RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
	RTV_DIMENSION_TEXTURE3D = 8
};
/**	\enum RTV_DIMENSION
*	\brief Indepentent enum equivalent to DirectX's D3D11_RTV_DIMENSION
*/
struct BUFFER_RTV
{
	union
	{
		unsigned int firstElement;
		unsigned int elementOffset;
	};
	union
	{
		unsigned int numElements;
		unsigned int elementWidth;
	};
};
/**	\struct BUFFER_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_BUFFER_RTV
*/

struct TEX1D_RTV
{
	unsigned int mipSlice;
};
/**	\struct TEX1D_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX1D_RTV
*/
struct TEX1D_ARRAY_RTV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};
/**	\struct TEX1D_ARRAY_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX1D_ARRAY_RTV
*/
struct TEX2D_RTV
{
	unsigned int mipSlice;
};
/**	\struct TEX2D_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2D_RTV
*/
struct TEX2D_ARRAY_RTV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};
/**	\struct TEX2D_ARRAY_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2D_ARRAY_RTV
*/
struct TEX2DMS_RTV
{
	unsigned int unusedField_NothingToDefine;
};
/**	\struct TEX2DMS_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2DMS_RTV
*/
struct TEX2DMS_ARRAY_RTV
{
	unsigned int firstArraySlice;
	unsigned int arraySize;
};
/**	\struct TEX2DMS_ARRAY_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX2DMS_ARRAY_RTV
*/
struct TEX3D_RTV
{
	unsigned int mipSlice;
	unsigned int firstWSlice;
	unsigned int wSize;
};
/**	\struct TEX3D_RTV
*	\brief Indepentent structure equivalent to DirectX's D3D11_TEX3D_RTV
*/
enum FILTER
{
	FILTER_MIN_MAG_MIP_POINT = 0,
	FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
	FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
	FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
	FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
	FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
	FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
	FILTER_MIN_MAG_MIP_LINEAR = 0x15,
	FILTER_ANISOTROPIC = 0x55,
	FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
	FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
	FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
	FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
	FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
	FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
	FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
	FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
	FILTER_COMPARISON_ANISOTROPIC = 0xd5
};
/**	\enum FILTER
*	\brief Indepentent enum equivalent to DirectX's D3D11_FILTER
*/
enum TEXTURE_ADDRESS_MODE
{
	TEXTURE_ADDRESS_WRAP = 1,
	TEXTURE_ADDRESS_MIRROR = 2,
	TEXTURE_ADDRESS_CLAMP = 3,
	TEXTURE_ADDRESS_BORDER = 4,
	TEXTURE_ADDRESS_MIRROR_ONCE = 5
};
/**	\enum TEXTURE_ADDRES_MODE
*	\brief Indepentent enum equivalent to DirectX's D3D11_TEXTURE_ADRESS_MODE
*/
enum COMPARISON_FUNC
{
	COMPARISON_NEVER = 1,
	COMPARISON_LESS = 2,
	COMPARISON_EQUAL = 3,
	COMPARISON_LESS_EQUAL = 4,
	COMPARISON_GREATER = 5,
	COMPARISON_NOT_EQUAL = 6,
	COMPARISON_GREATER_EQUAL = 7,
	COMPARISON_ALWAYS = 8
};
/**	\enum COMPARISON_FUNC
*	\brief Indepentent enum equivalent to DirectX's D3D11_COMPARISON_FUNC
*/
struct SimpleVertex
{
	glm::vec3 Pos;			/**< Vector 3 for Position */
	glm::vec2 Tex;			/**< Vector 2 for Texture coordinates */
};
/**	\struct SimpleVertex
*	\brief Structure which defines a buffer with position and texture coords
*/
struct CBNeverChanges
{
	glm::mat4 mView;		/**< Matrix4 for View */
};
/**	\struct CBNeverChanges
*	\brief Structure which defines a buffer with a View matrix
*/
struct CBChangeOnResize
{
	glm::mat4 mProjection;	/**< Matrix4 for Projection */
};
/**	\struct CBChangeOnResize
*	\brief Structure which defines a buffer with a Projection matrix
*/struct CBChangesEveryFrame
{
	glm::mat4 mWorld;		/**< Matrix4 for World */
	glm::vec4 vMeshColor;	/**< Vec4 for Color */
};
/**	\struct CBChangesEveryFrame
*	\brief Structure which defines a World matrix and a color
*/