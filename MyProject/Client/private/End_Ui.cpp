#include "stdafx.h"
#include "..\Public\End_Ui.h"
#include "GameInstance.h"
#include "BossNpc.h"

CEnd_Ui::CEnd_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEnd_Ui::CEnd_Ui(const CEnd_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnd_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnd_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY / 3;

	m_fPosX = g_iWinCX >> 1;
	m_fPosY = -120.f;

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));


	m_fDownSizeX = g_iWinCX;
	m_fDownSizeY = g_iWinCY	/ 3;

	m_fDownPosX = g_iWinCX >> 1;
	m_fDownPosY = 840.f;

	XMStoreFloat4x4(&m_DownProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	return S_OK;
}

_int CEnd_Ui::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(End_Scene(TimeDelta)))
		return -1;

	return 0;
}

_int CEnd_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return 0;
}

HRESULT CEnd_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEnd_Ui::End_Scene(_double TimeDelta)
{

	if (true == m_bEndCheck)
	{
		m_fDownPosY -= 1.f;
		m_fPosY += 1.f;
		if (725.f >= m_fDownPosY)
		{
			m_fDownPosY = 725.f;
		}
		if (-9 <= m_fPosY)
		{
			m_fPosY = -9.f;
			if (false == m_bNpcAnimStartCheck)
			{
				m_bNpcAnimStartCheck = true;
				m_bEndCheck = false;
			}
		}
	}

	if (true == m_bNpcAnimStartCheck)
	{
		CGameInstance * pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject* pBossNpc = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_BossNpc");
		if (nullptr == pBossNpc)
		{
			MSGBOX("Failed to Finder : Endui : funtion : End_Scene");
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		Safe_Release(pGameInstance);

		dynamic_cast<CBossNpc*>(pBossNpc)->Set_EndStartCheck(true);
		m_bNpcAnimStartCheck = false;
	}

	return S_OK;
}

HRESULT CEnd_Ui::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_UpVIBuffer"), (CComponent**)&m_pUpVIBufferCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_DownVIBuffer"), (CComponent**)&m_pDownVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_DownTexture"), (CComponent**)&m_pDownTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnd_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pUpVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX;
	WorldMatrix._22 = m_fSizeY;
	WorldMatrix._33 = 0.7f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pUpVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pUpVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pUpVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	//m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pUpVIBufferCom, 0);

	m_pUpVIBufferCom->Render(8);

	_float4x4		DownWorldMatrix;
	XMStoreFloat4x4(&DownWorldMatrix, XMMatrixIdentity());

	DownWorldMatrix._11 = m_fDownSizeX;
	DownWorldMatrix._22 = m_fDownSizeY;
	DownWorldMatrix._33 = 0.9f;
	DownWorldMatrix._41 = m_fDownPosX - (g_iWinCX * 0.5f);
	DownWorldMatrix._42 = -m_fDownPosY + (g_iWinCY * 0.5f);

	m_pDownVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&DownWorldMatrix)), sizeof(_float4x4));
	m_pDownVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pDownVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_DownProjectionMatrix)), sizeof(_matrix));

	m_pDownVIBufferCom->Render(8);

	//m_pDownTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pDownVIBufferCom, 0);


	return S_OK;
}

CEnd_Ui * CEnd_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEnd_Ui*	pInstance = new CEnd_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CEnd_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEnd_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEnd_Ui*	pInstance = new CEnd_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CEnd_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnd_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pUpVIBufferCom);
	Safe_Release(m_pDownVIBufferCom);


}
