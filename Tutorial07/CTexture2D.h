#pragma once
#include "Define.h"
#include "Includes.h"
struct Texture2DStruct
{
	unsigned int W;
	unsigned int H;
	unsigned int mipLevels;
	unsigned int arraySize;
	FORMAT format;
	SAMPLEDESC sampleDesc;
	USAGE usage;
	unsigned int flags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
};

class CTexture2D
{
public:

	CTexture2D();
	void init(Texture2DStruct S);

	Texture2DStruct m_Data;
	void * getTexture();
#ifdef D3D11
	ID3D11Texture2D * m_pTexture;
	D3D11_TEXTURE2D_DESC m_Desc;
#endif // D3D11

};