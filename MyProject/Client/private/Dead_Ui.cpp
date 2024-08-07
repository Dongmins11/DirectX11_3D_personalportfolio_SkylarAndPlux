#include "stdafx.h"
#include "..\Public\Dead_Ui.h"

#include "GameInstance.h"
#include "Rock.h"

CDead_Ui::CDead_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CDead_Ui::CDead_Ui(const CDead_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDead_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDead_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;

	m_fPosX = g_iWinCX >> 1;
	m_fPosY = g_iWinCY >> 1;

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	return S_OK;
}

_int CDead_Ui::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");

	if (true == dynamic_cast<CRock*>(pPlayerObject)->Get_DeadCheck() && false == m_bShutOpen)
	{
		m_bRenderCheck = true;

		m_dDeadTime += TimeDelta;
		XMStoreFloat4(&m_vColor,XMVectorLerp(XMVectorSet(0.f, 0.f, 0.f, 00.1f), XMVectorSet(0.f, 0.f, 0.f, 1.f), m_dDeadTime));
		if (1.0 <= m_dDeadTime)
		{
			dynamic_cast<CRock*>(pPlayerObject)->Set_ShutDownCheck(true);
			m_bShutOpen = true;
			m_dDeadTime = 0.0;
		}
	}

	if (false == dynamic_cast<CRock*>(pPlayerObject)->Get_DeadCheck() && true == m_bShutOpen)
	{
		m_dDeadTime += TimeDelta;
		XMStoreFloat4(&m_vColor, XMVectorLerp(XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.01f), m_dDeadTime));
		if (1.0 <= m_dDeadTime)
		{
			m_dDeadTime = 0.0;
			dynamic_cast<CRock*>(pPlayerObject)->Set_ShutDownCheck(false);
			m_bShutOpen = false;
			m_bRenderCheck = false;
		}

	}


	Safe_Release(pGameInstance);

	return 0;
}

_int CDead_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (true == m_bRenderCheck)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
	return 0;
}

HRESULT CDead_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(6);

	return S_OK;
}

HRESULT CDead_Ui::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDead_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX;
	WorldMatrix._22 = m_fSizeY;
	WorldMatrix._33 = 1.f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 0);

	m_pVIBufferCom->Set_RawValue("g_RectColor", &m_vColor, sizeof(_float4));
	return S_OK;
}

CDead_Ui * CDead_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CDead_Ui*	pInstance = new CDead_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CDead_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDead_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CDead_Ui*	pInstance = new CDead_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CDead_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDead_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
