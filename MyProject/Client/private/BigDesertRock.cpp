#include "stdafx.h"
#include "..\public\BigDesertRock.h"
#include "GameInstance.h"


CBigDesertRock::CBigDesertRock(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBigDesertRock::CBigDesertRock(const CBigDesertRock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBigDesertRock::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBigDesertRock::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
	}
	else
		return E_FAIL;

	return S_OK;
}

_int CBigDesertRock::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;





	return 0;
}

_int CBigDesertRock::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CBigDesertRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pModelCom->Render(2);

	return S_OK;
}

HRESULT CBigDesertRock::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_FilterDiffuse"), TEXT("Com_Texture"), (CComponent**)&m_pSourceTextureCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Filter"), TEXT("Com_Texture"), (CComponent**)&m_pFilterTextureCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_FilterNormal"), TEXT("Com_Texture"), (CComponent**)&m_pNomalTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBigDesertRock::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	if (FAILED(m_pSourceTextureCom->SetUp_OnShader("g_SourceDiffuseTexture", m_pModelCom)))
		return E_FAIL;

	if (FAILED(m_pNomalTextureCom->SetUp_OnShader("g_SoureNomalTexture", m_pModelCom)))
		return E_FAIL;

	if (FAILED(m_pFilterTextureCom->SetUp_OnShader("g_FilterTexture", m_pModelCom, 1)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CBigDesertRock * CBigDesertRock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBigDesertRock* pInstance = new CBigDesertRock(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CBigDesertRock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBigDesertRock::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBigDesertRock* pInstance = new CBigDesertRock(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CBigDesertRock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBigDesertRock::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pSourceTextureCom);
	Safe_Release(m_pFilterTextureCom);
	Safe_Release(m_pNomalTextureCom);
}
