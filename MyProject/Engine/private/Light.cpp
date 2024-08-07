#include "..\public\Light.h"
#include "VIBuffer_Rect.h"



CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}


HRESULT CLight::NativecConstruct(const LIGHTDESC & tLight)
{
	m_pLightDesc = tLight;
	return S_OK;
}

HRESULT CLight::Render(CVIBuffer_Rect * pVIBuffer)
{
	if (nullptr == pVIBuffer)
		return E_FAIL;


	_uint			iPassIndex = 0;

	if (LIGHTDESC::TYPE_DIRECTIONAL == m_pLightDesc.iType)
	{
		iPassIndex = 4;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightDir", &m_pLightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightDiffuse", &m_pLightDesc.vDiffuse, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightAmbient", &m_pLightDesc.vAmbient, sizeof(_float4))))
			return E_FAIL;

		pVIBuffer->Render(iPassIndex);
	}
	if (LIGHTDESC::TYPE_POINT == m_pLightDesc.iType && true == m_bLightOn)
	{
		iPassIndex = 22;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightPos", &m_pLightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(pVIBuffer->Set_RawValue("g_fRange", &m_pLightDesc.fRange, sizeof(_float))))
			return E_FAIL;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightDiffuse", &m_pLightDesc.vDiffuse, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(pVIBuffer->Set_RawValue("g_vLightAmbient", &m_pLightDesc.vAmbient, sizeof(_float4))))
			return E_FAIL;

		pVIBuffer->Render(iPassIndex);
	}





	return S_OK;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & tLight)
{
	CLight* pLight = new CLight(pDevice, pDeviceContext);

	if (FAILED(pLight->NativecConstruct(tLight)))
	{
		MSGBOX("Failed to Create Light");
		Safe_Release(pLight);
	}
	return pLight;
}

void CLight::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
