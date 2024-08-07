#include "stdafx.h"
#include "..\Public\Crystal_Ui.h"

#include "GameInstance.h"

CCrystal_Ui::CCrystal_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CCrystal_Ui::CCrystal_Ui(const CCrystal_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrystal_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrystal_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = 20;
	m_fSizeY = 50;

	m_fPosX = g_iWinCX - (m_fSizeX*7);
	m_fPosY = (g_iWinCY>>1) - (m_fSizeY * 5);

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	for (_uint i = 0; i < 2; ++i)
		m_bIsScale[i] = false;

	wsprintf(m_szBuffer, TEXT("%d"), m_iCristal);

	return S_OK;
}

_int CCrystal_Ui::Tick(_double TimeDelta)
{
	if (0 >(__super::Tick(TimeDelta)))
		return -1;

	wsprintf(m_szBuffer, TEXT("%d"), m_iCristal);

	if (FAILED(Trans_Render()))
		return E_FAIL;

	return 0;
}

_int CCrystal_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;
	
	if (false == m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CCrystal_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Draw_Font(TEXT("Font_Gulim"), m_szBuffer, _float2(1062, 102), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.5f)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	
	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(1);

	if (FAILED(SetUp_ConstantBackTable()))
		return E_FAIL;

	m_pVIBufferBackCom->Render(2);

	return S_OK;
}

HRESULT CCrystal_Ui::Trans_Render()
{
	
	return S_OK;
}

HRESULT CCrystal_Ui::SetUp_Component()
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

HRESULT CCrystal_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + 10;
	WorldMatrix._22 = m_fSizeY + 10;
	WorldMatrix._33 = 1.f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture",m_pVIBufferCom, 3);

	return S_OK;
}

HRESULT CCrystal_Ui::SetUp_ConstantBackTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX + 40;
	WorldMatrix._22 = m_fSizeY + 10;
	WorldMatrix._33 = 1.f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferBackCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferBackCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferBackCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureBackCom->SetUp_OnShader("g_DiffuseTexture",m_pVIBufferBackCom,2);

	return S_OK;
}

CCrystal_Ui * CCrystal_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCrystal_Ui*	pInstance = new CCrystal_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CCrystal_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCrystal_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CCrystal_Ui*	pInstance = new CCrystal_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CCrystal_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrystal_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureBackCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pVIBufferBackCom);


}
