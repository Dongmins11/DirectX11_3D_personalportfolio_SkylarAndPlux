#include "stdafx.h"
#include "..\Public\Floor_Tool.h"

#include "GameInstance.h"

CFloor_Tool::CFloor_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_bIsClone(false)
{
}

CFloor_Tool::CFloor_Tool(const CFloor_Tool & rhs)
	: CGameObject(rhs)
	, m_bIsClone(true)
	, m_iMyCount(0)
{
}

HRESULT CFloor_Tool::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CFloor_Tool::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc = new MODELDESC;
		memcpy(m_pModelDesc, pArg, sizeof(MODELDESC));

		m_eType = (TYPE)m_pModelDesc->iType;
		if (FAILED(Setup_Component_Tool()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc->vPos));
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}
	
	return S_OK;
}

_int CFloor_Tool::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (TYPE::TYPE_ANIM == m_eType)
	{
		m_pVIBufferCom->Set_Animation(0);
		m_pVIBufferCom->Update(0.01);
		m_pVIBufferCom->Set_PreAnimation(0);
	}

	if (1 == m_iDeadNumber)
		return 1;

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	m_fTimeDelta += (_float)TimeDelta;
	
	if (100 <= m_fTimeDelta)
		m_fTimeDelta = 0;

	return 0;
}

_int CFloor_Tool::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CFloor_Tool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pColiderCom->Render(0);

	if (m_eType == TYPE_FILTER)
		m_pVIBufferCom->Render(2);
	else if (m_eType == TYPE_ROTATION)
	{
		m_pVIBufferCubeCom->Render(0);
		m_pVIBufferCom->Render(0);
	}
	else
		m_pVIBufferCom->Render(0);


#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CFloor_Tool::Set_Pos()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc->vPos));

	return S_OK;
}

HRESULT CFloor_Tool::Set_Scaile()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Scailing(m_pModelDesc->vScaile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor_Tool::Set_ScaileXYZ()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ScailingXYZ(m_pModelDesc->vScaileXYZ)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor_Tool::Set_Rotation()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	
	if (FAILED(m_pTransformCom->Set_Rotation(m_pModelDesc->vRotaition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor_Tool::Set_ModelDesc(MODELDESC * pModelDesc)
{
	if (nullptr == pModelDesc)
		return E_FAIL;
	MODELDESC tModelDesc;
	ZeroMemory(&tModelDesc, sizeof(MODELDESC));

	tModelDesc = *pModelDesc;
	*m_pModelDesc = tModelDesc;

	return S_OK;
}

void CFloor_Tool::Set_MyMinusCount(_uint iCount)
{
	m_iMyCount -= iCount;

	if (0 >= iCount)
		iCount = 0;

}

HRESULT CFloor_Tool::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Floor"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor_Tool::Setup_Component_Tool()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc->wstrPath.c_str(), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	
	/* Com_ColiderTransform*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ColiderTransform"), (CComponent**)&m_pColiderTransformCom)))
		return E_FAIL;

	/* Com_Colider */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_CubeColider"), TEXT("Com_Colider"), (CComponent**)&m_pColiderCom)))
		return E_FAIL;

	if (m_eType == TYPE_FILTER)
	{
		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Filter"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dest"), TEXT("Com_Texture"), (CComponent**)&m_pTextureComFilter)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dest"), TEXT("Com_Texture"), (CComponent**)&m_pNomalTextureCom)))
			return E_FAIL;
	}
	if (m_eType == TYPE_ROTATION)
	{
		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BumpMap"), TEXT("Com_Texture"), (CComponent**)&m_pNomalTextureCom)))
			return E_FAIL;
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCubeCom)))
			return E_FAIL;
	}


	/* For.Com_AABB */
	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.75f, 0.0f);
	ColliderDesc.vExtents = _float3(5.35f, 5.75f, 5.35f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_AABB"), (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloor_Tool::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pColiderTransformCom->Set_Scailing(10.f);
	m_pColiderTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	m_pColiderCom->Set_RawValue("g_WorldMatrix", &m_pColiderTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pColiderCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pColiderCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	if (TYPE_FILTER == m_eType)
	{
		m_pTextureCom->SetUp_OnShader("g_SourceDiffuseTexture", m_pVIBufferCom);
		m_pTextureComFilter->SetUp_OnShader("g_FilterTexture", m_pVIBufferCom);
	}

	if (TYPE_ROTATION == m_eType)
	{
		m_pVIBufferCubeCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pVIBufferCubeCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pVIBufferCubeCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


		m_pVIBufferCom->Set_RawValue("g_WaterTime", &m_fTimeDelta, sizeof(_float));
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

CFloor_Tool * CFloor_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CFloor_Tool*	pInstance = new CFloor_Tool(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFloor_Tool::Clone(void* pArg, _uint iMyLevelIndex)
{
	CFloor_Tool*	pInstance = new CFloor_Tool(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFloor_Tool::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		Safe_Delete(m_pModelDesc);
	}
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureComFilter);
	Safe_Release(m_pColiderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColiderTransformCom);


}
