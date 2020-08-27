#include "CPass.h"

CPass::CPass()
{
	m_pDC = nullptr;
	m_pVP = nullptr;
}

CPass::~CPass(){}

void CPass::setShaders()
{
#ifdef D3D11
	m_pDC->m_DeviceContext->IASetInputLayout(m_pVS.m_pInputLayout);
	m_pDC->m_DeviceContext->VSSetShader(m_pVS.m_pVertexShader, NULL, 0);

	m_pDC->m_DeviceContext->PSSetShader(m_pPS.m_pPixelShader, NULL, 0);
#endif // D3D11
}

void CPass::setCBs(CBuffer * VCB, CBuffer * PCB)
{
#ifdef D3D11
	m_pDC->m_DeviceContext->VSSetConstantBuffers(2, 1, &VCB->m_pBuffer);
	m_pDC->m_DeviceContext->PSSetConstantBuffers(2, 1, &PCB->m_pBuffer);
#endif // D3D11	
}

void CPass::setViewport()
{
	m_pDC->m_DeviceContext->RSSetViewports(1, &m_pVP->m_Viewport);
}

void CPass::init(StructPass P, int level)
{
	m_pDC = P.DeviceContext;

	for (int i = 0; i < P.RTVcount; i++)
	{
		CTexture2D * T = new CTexture2D();
		T->init(P.TextureStruct);

		if (FAILED(P.Device->m_Device->CreateTexture2D(&T->m_Desc, NULL, &T->m_pTexture)))
		{
			return;
		}

		m_pTextures.push_back(T);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		ID3D11RenderTargetView * R;
		if (FAILED(P.Device->m_Device->CreateRenderTargetView(T->m_pTexture, &rtvDesc, &R)))
		{
			return;
		}

		m_pRTVs.push_back(R);

		D3D11_SHADER_RESOURCE_VIEW_DESC view;
		view.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view.Texture2D.MostDetailedMip = 0;
		view.Texture2D.MipLevels = level;

		ID3D11ShaderResourceView * S;

		if (FAILED(P.Device->m_Device->CreateShaderResourceView(T->m_pTexture, &view, &S)))
		{
			return;
		}

		m_PassOutput.push_back(S);
	}
}

void CPass::setRT(CDepthStencilView * pDSV)
{
#ifdef D3D11
	if (pDSV)
	{
		m_pDC->m_DeviceContext->OMSetRenderTargets(m_pRTVs.size(), &m_pRTVs[0], pDSV->m_pDepthStencilView);
	}
	else
	{
		m_pDC->m_DeviceContext->OMSetRenderTargets(m_pRTVs.size(), &m_pRTVs[0], NULL);
	}
	
	//m_pDC->m_DeviceContext->RSSetState(m_pRS);
#endif
}

void CPass::clear(float ClearColor[4], CDepthStencilView * pDSV)
{
	if (m_pDC != nullptr)
	{
		for (int i = 0; i < m_pRTVs.size(); i++)
		{
			m_pDC->m_DeviceContext->ClearRenderTargetView(m_pRTVs[i], ClearColor);
		}
		m_pDC->m_DeviceContext->ClearDepthStencilView(pDSV->m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void CPass::draw(CSceneManager * pSC, UINT pStride)
{
	for (int i = 0; i < pSC->m_MeshList.size(); i++)
	{
		UINT offset = 0;

		m_pDC->m_DeviceContext->IASetVertexBuffers(0, 1, &pSC->m_MeshList[i]->m_VB->m_pBuffer, &pStride, &offset);
		m_pDC->m_DeviceContext->IASetIndexBuffer(pSC->m_MeshList[i]->m_IB->m_pBuffer, DXGI_FORMAT_R16_UINT, 0);

		m_pDC->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (pSC->m_MeshList[i]->m_Materials->m_TextureDiffuse != nullptr)
		{
			m_pDC->m_DeviceContext->PSSetShaderResources(0, 1, &pSC->m_MeshList[i]->m_Materials->m_TextureDiffuse);
			if (pSC->m_MeshList[i]->m_Materials->m_TextureNormal != nullptr && pSC->m_MeshList[i]->m_Materials->m_TextureSpecular != nullptr)
			{
				m_pDC->m_DeviceContext->PSSetShaderResources(1, 1, &pSC->m_MeshList[i]->m_Materials->m_TextureNormal);
				m_pDC->m_DeviceContext->PSSetShaderResources(2, 1, &pSC->m_MeshList[i]->m_Materials->m_TextureSpecular);
			}			
		}
		else
		{
			for (int i = 0; i < m_ShaderResources.size(); i++)
			{
				m_pDC->m_DeviceContext->PSSetShaderResources(i, 1, &m_ShaderResources[i]);
			}
		}
		m_pDC->m_DeviceContext->DrawIndexed(pSC->m_MeshList[i]->m_IndexNum, 0, 0);
	}
}

void CPass::render(CDepthStencilView * pDTV, CSceneManager * pSC, CCamera * pCam)
{
}

void CPass::setPass(CDepthStencilView * pDSV)
{
	setRT(pDSV);
	setShaders();
}

void CPass::addShaderResource(CDevice * Device, CTexture2D * Tex)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC D;
	ZeroMemory(&D, sizeof(D));
	D.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	D.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	D.Texture2D.MostDetailedMip = 0;
	D.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView * S;

	if (FAILED(Device->m_Device->CreateShaderResourceView(Tex->m_pTexture, &D, &S)))
	{
		return;
	}

	m_ShaderResources.push_back(S);
}

void CPass::initFromTexture(CTexture2D * texture, CDevice * Device)
{
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	ID3D11RenderTargetView * R;
	if (FAILED(Device->m_Device->CreateRenderTargetView(texture->m_pTexture, &rtvDesc, &R)))
	{
		return;
	}

	m_pRTVs.push_back(R);

	D3D11_SHADER_RESOURCE_VIEW_DESC view;
	view.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view.Texture2D.MostDetailedMip = 0;
	view.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView * S;

	if (FAILED(Device->m_Device->CreateShaderResourceView(texture->m_pTexture, &view, &S)))
	{
		return;
	}

	m_PassOutput.push_back(S);
}

void CPass::setRasterizerState(ID3D11RasterizerState * RSState)
{
	m_pDC->m_DeviceContext->RSSetState(RSState);
}

void CPass::setDepthStencilState(ID3D11DepthStencilState * DSState, UINT Value)
{
	m_pDC->m_DeviceContext->OMSetDepthStencilState(DSState, Value);
}

void CPass::releasePass()
{
	for (int i = 0; i < m_pTextures.size(); i++)
	{
		m_pTextures[i]->m_pTexture->Release();
		m_pRTVs[i]->Release();
		m_PassOutput[i]->Release();
	}

	for (int i = 0; i < m_ShaderResources.size(); i++)
	{
		m_ShaderResources[i] = nullptr;
	}

	m_pTextures.clear();
	m_pRTVs.clear();
	m_PassOutput.clear();
	m_ShaderResources.clear();
}