#include "stdafx.h"
#include "..\public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTerrain::Native_Construct_Prototype()
{

	return S_OK;
}

HRESULT CTerrain::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scailing(1.f);
	return S_OK;
}

_int CTerrain::Tick(_double TimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CTerrain::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRenderCom)
		return -1;

	if (FAILED(m_pRenderCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	return 0;
}

HRESULT CTerrain::Render()
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

#ifdef _DEBUG
	m_pNavigationCom->Render(0);
#endif // _DEBUG

	return S_OK;
}

HRESULT CTerrain::Setup_Component()
{
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Terrain"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Terrain"), TEXT("Com_Nomal_Texture"), (CComponent**)&m_pTextureNomalCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_HeightMap"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Navigation */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_VIBuffer_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Setup_ConstantTable()
{
	if (nullptr == m_pVIBufferCom || nullptr == m_pTransformCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose() , sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	_float		fDetail = 100.f;
	m_pVIBufferCom->Set_RawValue("g_fTileDetail", &fDetail, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 0);
	m_pTextureCom->SetUp_OnShader("g_NomalTexture",m_pVIBufferCom, 1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureNomalCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRenderCom);
}
