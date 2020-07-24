#include "CPass.h"

CPass::CPass()
{
	m_pVS = nullptr;
	m_pPS = nullptr;
	m_pDC = nullptr;
	m_pVP = nullptr;
}

CPass::~CPass()
{
}

void CPass::setShaders()
{
#ifdef D3D11
	m_pDC->m_DeviceContext->IASetInputLayout(m_pVS->m_pInputLayout);
	m_pDC->m_DeviceContext->VSSetShader(m_pVS->m_pVertexShader, NULL, 0);

	m_pDC->m_DeviceContext->PSSetShader(m_pPS->m_pPixelShader, NULL, 0);
#endif // D3D11
}

void CPass::setCBs(CBuffer * VCB, CBuffer * PCB, CBuffer * LCB)
{
#ifdef D3D11
	m_pDC->m_DeviceContext->VSSetConstantBuffers(2, 1, &VCB->m_pBuffer);
	m_pDC->m_DeviceContext->PSSetConstantBuffers(2, 1, &PCB->m_pBuffer);
	m_pDC->m_DeviceContext->PSSetConstantBuffers(3, 1, &LCB->m_pBuffer);
#endif // D3D11	
}

void CPass::setViewport()
{
	m_pDC->m_DeviceContext->RSSetViewports(1, &m_pVP->m_Viewport);
}

void CPass::init(StructPass P)
{
	m_pVS = P.VertexShader;
	m_pPS = P.PixelShader;
	m_pDC = P.DeviceContext;
	m_pVP = P.Viewport;
	m_pLCB = P.LightBuffer;
	m_pBCB = P.BoneBuffer;
}

void CPass::setRT(CRenderTargetView * pRTV, CDepthStencilView * pDSV)
{
#ifdef D3D11
	m_pDC->m_DeviceContext->OMSetRenderTargets(1, &pRTV->m_pRTV, pDSV->m_pDepthStencilView);
#endif
}

void CPass::clear(CDeviceContext * pDC, CRenderTargetView * pRTV, float ClearColor[4], CDepthStencilView * pDSV)
{
	if (pDC != nullptr)
	{
		pDC->m_DeviceContext->ClearRenderTargetView(pRTV->m_pRTV, ClearColor);
		pDC->m_DeviceContext->ClearDepthStencilView(pDSV->m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void CPass::draw(CSceneManager * pSC, CCamera * pCam)
{
	for (int i = 0; i < pSC->m_MeshList.size(); i++)
	{
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;		

		setCBs(&pCam->m_CBChangesEveryFrame, &pCam->m_CBChangesEveryFrame, m_pLCB);

		m_pDC->m_DeviceContext->IASetVertexBuffers(0, 1, &pSC->m_MeshList[i]->m_VB->m_pBuffer, &stride, &offset);
		m_pDC->m_DeviceContext->IASetIndexBuffer(pSC->m_MeshList[i]->m_IB->m_pBuffer, DXGI_FORMAT_R16_UINT, 0);

		m_pDC->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pDC->m_DeviceContext->PSSetShaderResources(0, 1, &pSC->m_MeshList[i]->m_Materials->m_TextureDiffuse);

		m_pDC->m_DeviceContext->DrawIndexed(pSC->m_MeshList[i]->m_IndexNum, 0, 0);
	}
}

void CPass::render(CRenderTargetView * pRTV, CDepthStencilView * pDTV, CSceneManager * pSC, CCamera * pCam)
{
	setRT(pRTV, pDTV);
	setShaders();
	m_pDC->m_DeviceContext->VSSetConstantBuffers(0, 1, &pCam->m_CBNeverChanges.m_pBuffer);
	m_pDC->m_DeviceContext->VSSetConstantBuffers(1, 1, &pCam->m_CBChangesOnResize.m_pBuffer);
	m_pDC->m_DeviceContext->VSSetConstantBuffers(4, 1, &m_pBCB->m_pBuffer);
	draw(pSC, pCam);
}