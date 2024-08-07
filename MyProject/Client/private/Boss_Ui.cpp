#include "stdafx.h"
#include "..\Public\Boss_Ui.h"

#include "GameInstance.h"

CBoss_Ui::CBoss_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CBoss_Ui::CBoss_Ui(const CBoss_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBoss_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = 300;
	m_fSizeY = 18;

	m_fPosX = g_iWinCX >> 1;
	m_fPosY = (g_iWinCY >> 1) + (50 * 5);

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	for (_uint i = 0; i < 2; ++i)
		m_bIsScale[i] = false;

	m_vRectColor = _float4(243.f / 255.f, 199.f / 255.f, 60.f / 255.f, 1.f);
	m_iPassIndex = 1;
	m_bJetPackUsingCheck = false;

	m_fHp = 100.1f;
	m_fTraceHp = 100.1f;

	return S_OK;
}

_int CBoss_Ui::Tick(_double TimeDelta)
{
	if (0 >(__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Trans_Render(TimeDelta)))
		return E_FAIL;



	return 0;
}

_int CBoss_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if(true == m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return 0;
}

HRESULT CBoss_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	if (FAILED(SetUp_ConstantBackTable()))
		return E_FAIL;

	m_pVIBufferBackCom->Render(1);


	if (FAILED(SetUp_ConstantFrontTable()))
		return E_FAIL;

	m_pVIBufferFrontCom->Render(17);

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(16);



	
	

	return S_OK;
}

HRESULT CBoss_Ui::Trans_Render(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	if (pGameInstance->Key_Down('Z'))
		m_fHp -= 5.f;


	if (m_fTraceHp > m_fHp)
		m_fTraceHp -= TimeDelta * 10;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CBoss_Ui::SetUp_Component()
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

HRESULT CBoss_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX;
	WorldMatrix._22 = m_fSizeY;
	WorldMatrix._33 = 1.0f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	_float fHp = m_fHp / 100.f;

	m_pVIBufferCom->Set_RawValue("g_PowerUV", &fHp, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 13);

	return S_OK;
}

HRESULT CBoss_Ui::SetUp_ConstantFrontTable()
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


	_float Trace = m_fTraceHp / 100.f;
	m_pVIBufferFrontCom->Set_RawValue("g_PowerUV", &Trace, sizeof(_float));
	m_pVIBufferFrontCom->Set_RawValue("g_RectColor", &m_vRectColor, sizeof(_float4));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferFrontCom, 13);

	return S_OK;
}

HRESULT CBoss_Ui::SetUp_ConstantBackTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + 30;
	WorldMatrix._22 = m_fSizeY + 20;
	WorldMatrix._33 = 0.8f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferBackCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferBackCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferBackCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureBackCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferBackCom,12);

	return S_OK;
}

CBoss_Ui * CBoss_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBoss_Ui*	pInstance = new CBoss_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CBoss_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBoss_Ui*	pInstance = new CBoss_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CBoss_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureBackCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferFrontCom);
	Safe_Release(m_pVIBufferBackCom);


}
