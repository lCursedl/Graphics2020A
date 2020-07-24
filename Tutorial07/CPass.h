#pragma once
#include "CDeviceContext.h"
#include "CViewport.h"
#include "CVertexShader.h"
#include "CPixelShader.h"
#include "CRenderTargetView.h"
#include "CDepthStencilView.h"
#include "CBuffer.h"
#include "CSceneManager.h"
#include "CCamera.h"

struct StructPass
{
	CVertexShader * VertexShader;
	CPixelShader * PixelShader;
	CDeviceContext * DeviceContext;
	CViewport * Viewport;
	CBuffer * LightBuffer;
	CBuffer * BoneBuffer;
};

class CPass
{
public:
	CPass();
	~CPass();

	CVertexShader		* m_pVS;
	CPixelShader		* m_pPS;
	CDeviceContext		* m_pDC;
	CViewport			* m_pVP;
	CBuffer				* m_pLCB;
	CBuffer				* m_pBCB;
	
	void init(StructPass P);

#ifdef D3D11
	void setRT(CRenderTargetView * pRTV, CDepthStencilView * pDSV);
	void setShaders();
	void setCBs(CBuffer * VCB, CBuffer * PCB, CBuffer * LCB);
	void setViewport();
	void clear(CDeviceContext * pDC, CRenderTargetView * pRTV, float ClearColor[4], CDepthStencilView * pDSV);
	void draw(CSceneManager * pSC, CCamera * pCam);
	void render(CRenderTargetView * pRTV, CDepthStencilView * pDTV, CSceneManager * pSC, CCamera * pCam);

#elif OPENGL
	void clear();
#endif // D3D11	
};