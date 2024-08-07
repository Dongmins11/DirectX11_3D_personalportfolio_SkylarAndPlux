#include "stdafx.h"
#include "..\public\Terrain_Tool.h"
#include "GameInstance.h"

CTerrain_Tool::CTerrain_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTerrain_Tool::CTerrain_Tool(const CTerrain_Tool & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Tool::Native_Construct_Prototype()
{

	return S_OK;
}

HRESULT CTerrain_Tool::Native_Construct(void* pArg, _uint iMyLevelIndex)
{

	if (FAILED(Setup_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrain_Tool::Tick(_double TimeDelta)
{

	return 0;
}

_int CTerrain_Tool::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRenderCom)
		return -1;

	if (FAILED(m_pRenderCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;




	return 0;
}

HRESULT CTerrain_Tool::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(Setup_ConstantTable()))
		return E_FAIL;

	if (GetKeyState(VK_F1) & 0x8000)
	{
		if (FAILED(m_pVIBufferCom->Render(1)))
			return E_FAIL;
	}
	else
		if (FAILED(m_pVIBufferCom->Render(0)))
			return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Tool::Setup_Component()
{
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CTerrain_Tool::Setup_ConstantTable()
{
	if (nullptr == m_pVIBufferCom || nullptr == m_pTransformCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	_float		fDetail = 100.f;
	m_pVIBufferCom->Set_RawValue("g_fTileDetail", &fDetail, sizeof(_float));

	//m_pVIBufferCom->Set_RawValue("g_CamPos", &pGameInstance->Get_CamPosition(), sizeof(_float4));


	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 0);
	m_pTextureCom->SetUp_OnShader("g_NomalTexture", m_pVIBufferCom, 1);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTerrain_Tool * CTerrain_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTerrain_Tool* pInstance = new CTerrain_Tool(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain_Tool::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTerrain_Tool* pInstance = new CTerrain_Tool(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Tool::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRenderCom);
}
