#include "stdafx.h"
#include "..\Public\JetPack_Ui.h"

#include "GameInstance.h"

CJetPack_Ui::CJetPack_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CJetPack_Ui::CJetPack_Ui(const CJetPack_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CJetPack_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CJetPack_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = 70;
	m_fSizeY = 70;

	m_fPosX = g_iWinCX - (20 * 10);
	m_fPosY = (g_iWinCY >> 1) + (50 * 5);

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	for (_uint i = 0; i < 2; ++i)
		m_bIsScale[i] = false;

	m_vRectColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_fJetPackPower = 0.f;
	m_iPassIndex = 1;
	m_bJetPackUsingCheck = false;

	if (LEVEL_STAGE1 == m_iMyLevel)
		m_bRenderCheck = true;
	else
		m_bRenderCheck = false;

	return S_OK;
}

_int CJetPack_Ui::Tick(_double TimeDelta)
{
	if (0 >(__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Trans_Render(TimeDelta)))
		return E_FAIL;



	return 0;
}

_int CJetPack_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if(false == m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CJetPack_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	if (FAILED(SetUp_ConstantBackTable()))
		return E_FAIL;

	m_pVIBufferBackCom->Render(2);


	if (FAILED(SetUp_ConstantFrontTable()))
		return E_FAIL;

	m_pVIBufferFrontCom->Render(14);


	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(1);

	return S_OK;
}

HRESULT CJetPack_Ui::Trans_Render(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (true == m_bJetPackUsingCheck)
	{
		m_fJetPackPower += (_float)(TimeDelta * 0.25);
		m_vRectColor = _float4(243.f / 255.f, 199.f / 255.f, 60.f / 255.f, 1.f);
		if (1.1f <= m_fJetPackPower)
		{
			m_bPowerCheck = false;
			m_fJetPackPower = 1.1f;
		}
	}
	else
	{
		m_fJetPackPower -= (_float)(TimeDelta * 0.25);
		m_bPowerCheck = true;
		if (0.0f >= m_fJetPackPower)
		{
			m_vRectColor = _float4(1.f, 1.f, 1.f, 1.f);
			m_fJetPackPower = 0.0f;
		}
	}


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CJetPack_Ui::SetUp_Component()
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
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_FrontVIBuffer"), (CComponent**)&m_pVIBufferFrontCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBufferBack"), (CComponent**)&m_pVIBufferBackCom)))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Ui"), TEXT("Com_TextureBack"), (CComponent**)&m_pTextureBackCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CJetPack_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX - 20;
	WorldMatrix._22 = m_fSizeY - 20;
	WorldMatrix._33 = 1.0f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 7);

	return S_OK;
}

HRESULT CJetPack_Ui::SetUp_ConstantFrontTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX;
	WorldMatrix._22 = m_fSizeY;
	WorldMatrix._33 = 0.9f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferFrontCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferFrontCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferFrontCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pVIBufferFrontCom->Set_RawValue("g_PowerUV", &m_fJetPackPower, sizeof(_float));
	m_pVIBufferFrontCom->Set_RawValue("g_RectColor", &m_vRectColor, sizeof(_float4));
	

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferFrontCom, 1);

	return S_OK;
}

HRESULT CJetPack_Ui::SetUp_ConstantBackTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + 3;
	WorldMatrix._22 = m_fSizeY + 3;
	WorldMatrix._33 = 0.8f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f) + 5;
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f) - 5;

	m_pVIBufferBackCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferBackCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferBackCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureBackCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferBackCom, 10);

	return S_OK;
}

CJetPack_Ui * CJetPack_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CJetPack_Ui*	pInstance = new CJetPack_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CJetPack_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJetPack_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CJetPack_Ui*	pInstance = new CJetPack_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CJetPack_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJetPack_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureBackCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferFrontCom);
	Safe_Release(m_pVIBufferBackCom);


}
