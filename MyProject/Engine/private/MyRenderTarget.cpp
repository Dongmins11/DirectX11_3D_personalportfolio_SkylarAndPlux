#include "..\public\MyRenderTarget.h"
#include "VIBuffer_Rect.h"


CMyRenderTarget::CMyRenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CMyRenderTarget::NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	m_vClearColor = vClearColor;

	D3D11_TEXTURE2D_DESC			TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture)))
		return E_FAIL;

	D3D11_RENDER_TARGET_VIEW_DESC			TargetViewDesc;
	ZeroMemory(&TargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	TargetViewDesc.Format = eFormat;
	TargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	TargetViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture, &TargetViewDesc, &m_pRenderTargetView)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC			ShaderResourceViewDesc;
	ZeroMemory(&ShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	ShaderResourceViewDesc.Format = eFormat;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture, &ShaderResourceViewDesc, &m_pShaderResoureceView)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyRenderTarget::Clear_RenderTarget()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, (_float*)&m_vClearColor);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CMyRenderTarget::Ready_Debug_Desc(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	m_TransformMatrix._11 = fSizeX;
	m_TransformMatrix._22 = fSizeY;

	m_TransformMatrix._41 = fStartX + fSizeX * 0.5f - (Viewport.Width * 0.5f);
	m_TransformMatrix._42 = -(fStartY + fSizeY * 0.5f) + (Viewport.Height * 0.5f);

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));

	return S_OK;

}

HRESULT CMyRenderTarget::Render_Debug_Buffer(CVIBuffer * pVIBuffer)
{
	if (nullptr == pVIBuffer)
		return E_FAIL;

	pVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4));
	pVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pShaderResoureceView);

	pVIBuffer->Render(0);

	return S_OK;
}

#endif // _DEBUG

CMyRenderTarget * CMyRenderTarget::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CMyRenderTarget*	pInstance = new CMyRenderTarget(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(iWidth, iHeight, eFormat, vClearColor)))
	{
		MSGBOX("Failed to Create  CRenderTarget");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMyRenderTarget::Free()
{
	Safe_Release(m_pRenderTargetView);
	Safe_Release(m_pShaderResoureceView);
	Safe_Release(m_pTexture);


	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
