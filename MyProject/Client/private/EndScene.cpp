#include "stdafx.h"
#include "..\Public\EndScene.h"
#include "Camera_Fly.h"
#include "GameInstance.h"
#include "Rock.h"
#include "End_Ui.h"

CEndScene::CEndScene(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEndScene::CEndScene(const CEndScene & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEndScene::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndScene::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_fSizeX = 600;
	m_fSizeY = 300;

	m_fPosX = g_iWinCX >> 1;
	m_fPosY = 650;

	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_iPassIndex = 10;

	return S_OK;
}

_int CEndScene::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Chat_IndexChange(TimeDelta)))
		return -1;

	if (true == m_bRealEndCheck)
	{
		m_iCurChatIndex = 3;

		m_iPassIndex = 11;

		m_fSizeX = g_iWinCX;
		m_fSizeY = g_iWinCY;

		m_fPosX = g_iWinCX >> 1;
		m_fPosY = g_iWinCY >> 1;

		m_fAlphaColor += TimeDelta * 0.5;
		if (1.0 <= m_fAlphaColor)
		{
			m_fAlphaColor = 1.f;
			m_bRealEndCheck = false;
		}
	}


	return 0;
}

_int CEndScene::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ENDSCENE, this);

	return 0;
}

HRESULT CEndScene::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(m_iPassIndex);

	return S_OK;
}

HRESULT CEndScene::Chat_IndexChange(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down(VK_RETURN))
	{
		if (1 >= m_iCurChatIndex)
		{
			++m_iCurChatIndex;
		}
		if (2 == m_iCurChatIndex)
		{
			m_bChatCheck = true;
		}
	}


	if (true == m_bChatCheck)
	{
		m_bChatCheckTime += TimeDelta;
		if (3.0 <= m_bChatCheckTime)
		{
			m_bRealEndCheck = true;
			m_bChatCheck = false;

		}
	}

	Safe_Release(pGameInstance);



	return S_OK;
}

HRESULT CEndScene::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_EndScene"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndScene::SetUp_ConstantTable()
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

	if (m_iCurChatIndex == 3)
		m_pVIBufferCom->Set_RawValue("g_AlphaColor", &m_fAlphaColor, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, m_iCurChatIndex);


	return S_OK;
}

CEndScene * CEndScene::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEndScene*	pInstance = new CEndScene(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CEndScene");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEndScene::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEndScene*	pInstance = new CEndScene(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CEndScene");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEndScene::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
