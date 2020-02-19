#pragma once
#include "Define.h"

struct ViewportStruct
{
	float topLeftX;
	float topLeftY;
	float W;
	float H;
	float minDepth;
	float maxDepth;
};

class CViewport
{
public:
	CViewport();
	void init(ViewportStruct V);

#ifdef D3D11
	D3D11_VIEWPORT m_Viewport;
#endif // D3D11
};