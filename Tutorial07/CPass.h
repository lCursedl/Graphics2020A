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
	//Vector para almacenar render target views
	std::vector<ID3D11RenderTargetView*>	m_pRTVs;
	//Vector para almacenar las texturas del pase, que generar rtvs y shader resource views
	std::vector<CTexture2D*> m_pTextures;
	//Vector que almacena recursos provenientes de otros pases
	std::vector<ID3D11ShaderResourceView*> m_ShaderResources;
	//Vector que almacena shader resource views, para compartir con otros pases y dibujar con imgui
	std::vector<ID3D11ShaderResourceView*> m_PassOutput;

	void init(StructPass P, int level);

#ifdef D3D11
	//Funcion para asignar los render target views
	void setRT(CDepthStencilView * pDSV);
	//Funcion para asignar los shaders del pase
	void setShaders();
	void setCBs(CBuffer * VCB, CBuffer * PCB);									//N
	void setViewport();															//N
	//Funcion para limpiar los render target views del pase
	void clear(float ClearColor[4], CDepthStencilView * pDSV);
	//Funcion para dibujar el pase con las configuraciones aplicadas
	void draw(CSceneManager * pSC, UINT pStride);
	void render(CDepthStencilView * pDSV, CSceneManager * pSC, CCamera * pCam);	//N
	//Funcion que llama a setRT y setShaders
	void setPass(CDepthStencilView * pDSV);
	void addShaderResource(CDevice * Device ,CTexture2D* Tex);
	void initFromTexture(CTexture2D* texture, CDevice* Device);
	void setRasterizerState(ID3D11RasterizerState* RSState);
	void setDepthStencilState(ID3D11DepthStencilState * DSState, UINT Value);
	//Funcion para liberar todos los recursos creados por el pase y tomados de otros
	void releasePass();
	//Funcion para vaciar los recursos tomados de otros pases
	void clearShaderResources();
#elif OPENGL
	void clear();
#endif // D3D11	
};