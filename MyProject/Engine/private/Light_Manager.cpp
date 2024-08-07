#include "..\public\Light_Manager.h"
#include "Light.h"
#include "VIBuffer_Rect.h"
#include "RenderTarget_Manager.h"
#include "PipeLine.h"


IMPLEMENT_SINGLETON(CLight_Manager)
CLight_Manager::CLight_Manager()
{
}

HRESULT CLight_Manager::NativeConstruct(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	m_pVIBuffer = CVIBuffer_Rect::Create(pDevice, pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = Viewport.Width;
	m_TransformMatrix._22 = Viewport.Height;

	m_TransformMatrix._41 = 0.0f;
	m_TransformMatrix._42 = 0.0f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));

	return S_OK;
}

HRESULT CLight_Manager::Set_LightDesc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & tLightDesc)
{
	if (nullptr == pDevice ||
		nullptr == pDeviceContext)
		return E_FAIL;

	CLight* pLight = CLight::Create(pDevice, pDeviceContext, tLightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_listLight.emplace_back(pLight);

	return S_OK;
}

LIGHTDESC * CLight_Manager::Get_LighDesc(_uint iIndex)
{
	auto iter = m_listLight.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

void CLight_Manager::Set_LightRenderCheck(_uint iIndex, _bool bCheck)
{
	auto iter = m_listLight.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_LightOn(bCheck);
}


HRESULT CLight_Manager::Render()
{
	CRenderTarget_Manager*	pTarget_Manager = GET_INSTANCE(CRenderTarget_Manager);

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pVIBuffer->Set_TextureValue("g_NormalTexture", pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Normal")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Set_TextureValue("g_DepthTexture", pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Depth")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Set_TextureValue("g_vMtrlEmissiveTexture", pTarget_Manager->Get_ShaderResourceView(TEXT("Target_EmmissiveColor")))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ViewMatrixInverse, ProjMatrixInverse;
	_float4		vCamPosition;

	XMStoreFloat4x4(&ViewMatrixInverse, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW))));
	m_pVIBuffer->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInverse, sizeof(_float4x4));

	XMStoreFloat4x4(&ProjMatrixInverse, XMMatrixTranspose(XMMatrixInverse(nullptr, pPipeLine->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION))));
	m_pVIBuffer->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInverse, sizeof(_float4x4));

	XMStoreFloat4(&vCamPosition, pPipeLine->Get_CamPosition());
	m_pVIBuffer->Set_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4));

	for (auto& pLight : m_listLight)
	{
		if (nullptr != pLight)
			pLight->Render(m_pVIBuffer);
	}

	RELEASE_INSTANCE(CPipeLine);

	RELEASE_INSTANCE(CRenderTarget_Manager);

	return S_OK;
}

HRESULT CLight_Manager::Clear_Light()
{
	for (auto& pLight : m_listLight)
	{
		Safe_Release(pLight);
	}
	m_listLight.clear();

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& iter : m_listLight)
	{
		Safe_Release(iter);
	}
	m_listLight.clear();

	Safe_Release(m_pVIBuffer);

}
