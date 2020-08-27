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
#include "CTexture2D.h"

struct StructPass
{
	CDeviceContext	* DeviceContext;
	CDevice			* Device;
	CViewport		* Viewport;
	unsigned int	RTVcount;
	Texture2DStruct TextureStruct;
#ifdef D3D11
	D3D11_RASTERIZER_DESC rstate;
#endif // D3D11

};

class CPass
{
public:
	CPass();
	~CPass();

	CVertexShader		m_pVS;
	CPixelShader		m_pPS;
	CDeviceContext		* m_pDC;
	CViewport			* m_pVP;
#ifdef D3D11
	ID3D11RasterizerState * m_pRS;
#endif // D3D11

	std::vector<ID3D11RenderTargetView*>	m_pRTVs;
	std::vector<CTexture2D*> m_pTextures;
	std::vector<ID3D11ShaderResourceView*> m_ShaderResources;
	std::vector<ID3D11ShaderResourceView*> m_PassOutput;

	void init(StructPass P, int level);

#ifdef D3D11
	void setRT(CDepthStencilView * pDSV);
	void setShaders();
	void setCBs(CBuffer * VCB, CBuffer * PCB);									//N
	void setViewport();															//N
	void clear(float ClearColor[4], CDepthStencilView * pDSV);
	void draw(CSceneManager * pSC, UINT pStride);
	void render(CDepthStencilView * pDSV, CSceneManager * pSC, CCamera * pCam);	//N
	void setPass(CDepthStencilView * pDSV);
	void addShaderResource(CDevice * Device ,CTexture2D* Tex);
	void initFromTexture(CTexture2D* texture, CDevice* Device);
	void setRasterizerState(ID3D11RasterizerState* RSState);
	void setDepthStencilState(ID3D11DepthStencilState * DSState, UINT Value);
	void releasePass();
#elif OPENGL
	void clear();
#endif // D3D11	
};