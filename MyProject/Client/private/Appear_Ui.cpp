#include "stdafx.h"
#include "..\Public\Appear_Ui.h"
#include "Camera_Fly.h"
#include "GameInstance.h"
#include "Rock.h"
#include "End_Ui.h"

CAppear_Ui::CAppear_Ui(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CAppear_Ui::CAppear_Ui(const CAppear_Ui & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAppear_Ui::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAppear_Ui::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = 400;
	m_fSizeY = 150;

	m_fPosX = g_iWinCX >> 1;
	m_fPosY = (g_iWinCY - 250) >> 1;

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_iPassIndex = 11;

	m_fAlphaColor = 0.f;

	return S_OK;
}

_int CAppear_Ui::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if(false == m_bColorCheck)
	{
		m_dColorTimeAcc += TimeDelta * 0.5;
		m_fAlphaColor = 0.01f * (1.0 - m_dColorTimeAcc) + 1.f * m_dColorTimeAcc;

		if (1.0 <= m_dColorTimeAcc)
		{
			m_dColorTimeAcc = 0.0;
			m_bColorCheck = true;
		}
	}



	if (true == m_bColorCheck)
	{
		m_dColorTimeAcc += TimeDelta * 0.5;
		m_fAlphaColor = 1.0f * (1.0 - m_dColorTimeAcc) + 0.01f * m_dColorTimeAcc;

		if (1.0 <= m_dColorTimeAcc)
		{
			m_dColorTimeAcc = 0.0;

			return 1;
		}
	}

	return 0;
}

_int CAppear_Ui::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ENDSCENE, this);

	return 0;
}

HRESULT CAppear_Ui::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(12);

	return S_OK;
}


HRESULT CAppear_Ui::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_CRT"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAppear_Ui::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	WorldMatrix._11 = m_fSizeX;
	WorldMatrix._22 = m_fSizeY;
	WorldMatrix._33 = 0.5f;
	WorldMatrix._41 = m_fPosX - (g_iWinCX * 0.5f);
	WorldMatrix._42 = -m_fPosY + (g_iWinCY * 0.5f);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_float4x4));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_ProjectionMatrix)), sizeof(_matrix));


	m_pVIBufferCom->Set_RawValue("g_AlphaColor", &m_fAlphaColor, sizeof(_float));


	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, m_iCurChatIndex);


	return S_OK;
}

CAppear_Ui * CAppear_Ui::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CAppear_Ui*	pInstance = new CAppear_Ui(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CAppear_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CAppear_Ui::Clone(void* pArg, _uint iMyLevelIndex)
{
	CAppear_Ui*	pInstance = new CAppear_Ui(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CAppear_Ui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAppear_Ui::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
