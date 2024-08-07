#include "..\public\RenderTarget_Manager.h"
#include "MyRenderTarget.h"
#include "VIBuffer_Rect.h"

IMPLEMENT_SINGLETON(CRenderTarget_Manager)

CRenderTarget_Manager::CRenderTarget_Manager()
{
}
ID3D11ShaderResourceView * CRenderTarget_Manager::Get_ShaderResourceView(const _tchar * pTargetTag)
{
	CMyRenderTarget*	pTarget = Finder_RenderTargetView(pTargetTag);
	if (nullptr == pTarget)
		return nullptr;

	return pTarget->Get_ShaderResourceView();
}
HRESULT CRenderTarget_Manager::NativeConsturct(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Rect::Create(pDevice, pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	_uint				iNumViewPorts = 1;
	D3D11_VIEWPORT		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));

	pDeviceContext->RSGetViewports(&iNumViewPorts, &ViewPort);

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewPort.Width, ViewPort.Height, 0.f, 1.f)));

#endif // _DEBUG


	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr == pDevice || nullptr == pDeviceContext)
		return E_FAIL;

	if (nullptr != Finder_RenderTargetView(pRenderTargetTag))
		return E_FAIL;

	CMyRenderTarget*	pRenderTarget = CMyRenderTarget::Create(pDevice, pDeviceContext, iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_MapRenderTargets.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_MultiRenderTarget(const _tchar * pMultiTargetTag, const _tchar * pRenderTargetTag)
{
	CMyRenderTarget*	pRenderTarget = Finder_RenderTargetView(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CMyRenderTarget*>*		pMultTargetList = Finder_MultiRenderTargetView(pMultiTargetTag);
	if (nullptr == pMultTargetList)
	{
		list<CMyRenderTarget*>	MRTList;
		MRTList.push_back(pRenderTarget);

		m_MapMultiRenderTargets.emplace(pMultiTargetTag, MRTList);
	}
	else
		pMultTargetList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MultiRenderTarget(ID3D11DeviceContext * pDeviceContext, const _tchar * pMultiTargetTag)
{
	if (nullptr == pDeviceContext)
		return E_FAIL;

	list<CMyRenderTarget*>*	pMultiTargetList = Finder_MultiRenderTargetView(pMultiTargetTag);
	if (nullptr == pMultiTargetList)
		return E_FAIL;

	pDeviceContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	ID3D11RenderTargetView*		pRenderTargerts[8];

	_uint iNumTargerts = 0;


	for (auto& pTarget : *pMultiTargetList)
	{
		pTarget->Clear_RenderTarget();
		pRenderTargerts[iNumTargerts++] = pTarget->Get_RenderTargetView();
	}

	pDeviceContext->OMSetRenderTargets(iNumTargerts, pRenderTargerts, m_pDepthStencilView);


	return S_OK;
}

HRESULT CRenderTarget_Manager::End_MultiRenderTarget(ID3D11DeviceContext * pDeviceContext)
{
	if (nullptr == pDeviceContext)
		return E_FAIL;

	pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	ID3D11ShaderResourceView*		ShaderResourceView[8] = { nullptr };
	pDeviceContext->PSSetShaderResources(0, 8, ShaderResourceView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget_Manager::Ready_Debug_Buffer_Desc(const _tchar * pTargetTag, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{

	CMyRenderTarget*	pRenderTarget = Finder_RenderTargetView(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	if (FAILED(pRenderTarget->Ready_Debug_Desc(fStartX, fStartY, fSizeX, fSizeY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderTarget_Manager::Render_Debug_Buffer(const _tchar * pMRTTag)
{
	list<CMyRenderTarget*>*		pMultiRenderTargetList = Finder_MultiRenderTargetView(pMRTTag);
	if (nullptr == pMultiRenderTargetList)
		return E_FAIL;

	_float4x4		ViewMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	m_pVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjectionMatrix, sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4));

	for (auto& pRenderTarget : *pMultiRenderTargetList)
	{
		pRenderTarget->Render_Debug_Buffer(m_pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CMyRenderTarget * CRenderTarget_Manager::Finder_RenderTargetView(const _tchar * pRenderTargetTag)
{
	auto iter = find_if(m_MapRenderTargets.begin(), m_MapRenderTargets.end(), CTagFinder(pRenderTargetTag));

	if (iter == m_MapRenderTargets.end())
		return nullptr;

	return iter->second;
}

list<CMyRenderTarget*>* CRenderTarget_Manager::Finder_MultiRenderTargetView(const _tchar * pMultiRenderTargetTag)
{
	auto iter = find_if(m_MapMultiRenderTargets.begin(), m_MapMultiRenderTargets.end(), CTagFinder(pMultiRenderTargetTag));

	if (iter == m_MapMultiRenderTargets.end())
		return nullptr;

	return &iter->second;
}

void CRenderTarget_Manager::Free()
{
	for (auto& Pair : m_MapMultiRenderTargets)
	{
		for (auto& pTarget : Pair.second)
			Safe_Release(pTarget);
		Pair.second.clear();
	}
	m_MapMultiRenderTargets.clear();

	for (auto& Pair : m_MapRenderTargets)
		Safe_Release(Pair.second);

	m_MapRenderTargets.clear();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif // _DEBUG

}
