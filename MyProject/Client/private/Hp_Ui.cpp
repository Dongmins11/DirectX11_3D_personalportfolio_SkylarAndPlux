#include "stdafx.h"
#include "..\Public\Hp_Ui.h"
#include "Rock.h"
#include "GameInstance.h"

CHp_Ui::CHp_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CHp_Ui::CHp_Ui(const CHp_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHp_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHp_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		ZeroMemory(&m_tHpUiDesc, sizeof(HPUIDESC));
		memcpy(&m_tHpUiDesc, pArg, sizeof(HPUIDESC));

		m_fSizeX = m_tHpUiDesc.fSizeX;
		m_fSizeY = m_tHpUiDesc.fSizeY;

		m_fPosX = m_tHpUiDesc.fPosX;
		m_fPosY = m_tHpUiDesc.fPosY;

		XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

		for (_uint i = 0; i < 2; ++i)
			m_bIsScale[i] = false;
	}
	else
	{
		MSGBOX("Failed to Create HP_Ui");
		return E_FAIL;
	}

	m_fAlphaColor = 0.2f;

	return S_OK;
}

_int CHp_Ui::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (true == m_bSetTransCheck)
	{
		if (FAILED(Trans_Render()))
			return E_FAIL;
	}
	else
	{
		m_fSizeX = m_tHpUiDesc.fSizeX;
		m_fSizeY = m_tHpUiDesc.fSizeY;

		m_fPosX = m_tHpUiDesc.fPosX;
		m_fPosY = m_tHpUiDesc.fPosY;

		m_fScaleSet = 0.f;
		m_fweight = 0.f;
	}

	return 0;
}

_int CHp_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (false == m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CHp_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_iPassIndex);

	if (FAILED(SetUp_ConstantBackTable()))
		return E_FAIL;

	m_pVIBufferBackCom->Render(2);

	return S_OK;
}

HRESULT CHp_Ui::Trans_Render()
{
	if (false == m_bIsScale[0])
	{
		m_fScaleSet += 0.5f ;
		if (10 < m_fScaleSet)
			m_bIsScale[0] = true;
	}
	else
	{
		m_fScaleSet -= 0.5f;
		if (0 > m_fScaleSet)
			m_bIsScale[0] = false;
	}
	if (false == m_bIsScale[1])
	{
		m_fweight += 0.3f;
		if (10 < m_fweight)
			m_bIsScale[1] = true;
	}
	else
	{
		m_fweight -= 0.3f;
		if (0 > m_fweight)
			m_bIsScale[1] = false;
	}
	return S_OK;
}

HRESULT CHp_Ui::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;



	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Ui"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBufferBack"), (CComponent**)&m_pVIBufferBackCom)))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Ui"), TEXT("Com_TextureBack"), (CComponent**)&m_pTextureBackCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHp_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + m_fScaleSet;
	WorldMatrix._22 = m_fSizeY + m_fScaleSet;
	WorldMatrix._33 = 1.f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	if(12 == m_iPassIndex)
		m_pVIBufferCom->Set_RawValue("g_AlphaColor", &m_fAlphaColor, sizeof(_float));

	

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture",m_pVIBufferCom, 0);

	return S_OK;
}

HRESULT CHp_Ui::SetUp_ConstantBackTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + 15 + m_fScaleSet + m_fweight;
	WorldMatrix._22 = m_fSizeY + 15 + m_fScaleSet + m_fweight;
	WorldMatrix._33 = 1.f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferBackCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferBackCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferBackCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureBackCom->SetUp_OnShader("g_DiffuseTexture",m_pVIBufferBackCom, 1);

	return S_OK;
}

CHp_Ui * CHp_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CHp_Ui*	pInstance = new CHp_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CHp_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHp_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CHp_Ui*	pInstance = new CHp_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CHp_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHp_Ui::Free()
{
	__super::Free();



	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureBackCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferBackCom);


}
