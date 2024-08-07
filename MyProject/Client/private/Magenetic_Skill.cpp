#include "stdafx.h"
#include "..\public\Magenetic_Skill.h"
#include "GameInstance.h"
#include "Rock.h"

CMagenetic_Skill::CMagenetic_Skill(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMagenetic_Skill::CMagenetic_Skill(const CMagenetic_Skill & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMagenetic_Skill::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagenetic_Skill::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component_Load()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}



	m_pTransformCom->Set_Scailing(50.f);


	return S_OK;
}

_int CMagenetic_Skill::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	Set_Position();

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	return 0;
}

_int CMagenetic_Skill::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	if (false == m_bRenderCheck)
		m_pSphere->Set_CheckCollision(true);
	else
		m_pSphere->Set_CheckCollision(false);

	if (LEVEL_STAGE2 == m_iMyLevel)
	{
		if (FAILED(Gravitional_Check(TimeDelta)))
			return E_FAIL;
	}
	if (true == m_bRenderCheck)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
			return E_FAIL;
	}

	return 0;
}

HRESULT CMagenetic_Skill::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	//m_pModelCom->Render(8);

#ifdef _DEBUG
	m_pSphere->Render();
	m_pAABB->Render();
#endif // _DEBUG

	return S_OK;
}

void CMagenetic_Skill::Set_Position()
{
	if (nullptr == m_pTransformCom)
		return;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Magenetic", L"Com_Transform", 0);
	if (nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTargetTransform->Get_State(CTransform::STATE_POSITION));


	Safe_Release(pGameInstance);
}

HRESULT CMagenetic_Skill::Gravitional_Check(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider* pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Gravity_Field", L"Com_AABB", 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return E_FAIL;


	if (true == m_bRenderCheck)
	{
		if (true == m_pAABB->Collision_AABB(pTargetAABB))
		{
			if (FAILED(dynamic_cast<CRock*>(pPlayerObject)->Magenetic_SkillOn()))
				return E_FAIL;
		}
		
	}
	else
	{
		if (FAILED(dynamic_cast<CRock*>(pPlayerObject)->Magenetic_SkillDown(true)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}




HRESULT CMagenetic_Skill::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal5.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 0.14f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagenetic_Skill::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal5.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(0.01f, 0.03f, 0.01f);
	ColliderDesc.fRadius = 0.4f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMagenetic_Skill::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);

	return S_OK;
}

CMagenetic_Skill * CMagenetic_Skill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMagenetic_Skill* pInstance = new CMagenetic_Skill(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CMagenetic_Skill");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMagenetic_Skill::Clone(void* pArg, _uint iMyLevelIndex)
{
	CMagenetic_Skill* pInstance = new CMagenetic_Skill(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CMagenetic_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMagenetic_Skill::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);
	
	Safe_Release(m_pAABB);
	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
