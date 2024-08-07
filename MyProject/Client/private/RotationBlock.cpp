#include "stdafx.h"
#include "..\public\RotationBlock.h"
#include "GameInstance.h"
#include "Rock.h"
#include <random>

CRotationBlock::CRotationBlock(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRotationBlock::CRotationBlock(const CRotationBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRotationBlock::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRotationBlock::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		mt19937 engine((unsigned int)time(NULL));
		uniform_real_distribution<> distribution(20.f, 160.f);
		auto generator = bind(distribution, engine);

		m_pTransformCom->Set_Scailing(2.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.0f, 40.9f, 25.f, 1.f));

		m_fBackUp_RotationX = generator();
		m_fBackUp_RotationY = generator();
		m_fBackUp_RotationZ = generator();
		m_pTransformCom->Set_Rotation(_float3(m_fBackUp_RotationX, m_fBackUp_RotationY, m_fBackUp_RotationZ));
	}

	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int CRotationBlock::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (false == m_bFirstCheck)
	{
		m_fBackUp_RotationX = rand() % 140 + 20;
		m_fBackUp_RotationY = rand() % 140 + 20;
		m_fBackUp_RotationZ = rand() % 140 + 20;

		m_pTransformCom->Set_Rotation(_float3(m_fBackUp_RotationX, m_fBackUp_RotationY, m_fBackUp_RotationZ));

		m_bFirstCheck = true;
	}

	m_pBigSphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());


	return 0;
}

_int CRotationBlock::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Collision_Check(TimeDelta)))
		return -1;

	if (FAILED(Player_Collision_Check(TimeDelta)))
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CRotationBlock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	m_pModelCom->Render(0);
#ifdef _DEBUG
	m_pAABB->Render();
	m_pBigSphere->Render();
	m_pSphere->Render();
#endif // _DEBUG


	return S_OK;
}


HRESULT CRotationBlock::Collision_Check(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	CColider*		pTargetBigSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Magenetic_Skill"), TEXT("Com_Sphere"), 0, 0);
	
	if (nullptr == pTargetAABB || nullptr == pTargetBigSphere || nullptr == pTargetSphere)
		return -1;

	if (true == m_pBigSphere->Collision_Sphere(pTargetBigSphere))
	{
		if (false == pTargetSphere->Get_CheckCollision() && true == m_pSphere->Collision_Sphere(pTargetSphere))
		{
			if (false == m_bChangeCheck)
			{
				m_dTimeAcc += TimeDelta * 5;
				_float RotationX = m_fBackUp_RotationX * (1.0 - m_dTimeAcc) + 0.0 * m_dTimeAcc;
				_float RotationY = m_fBackUp_RotationY * (1.0 - m_dTimeAcc) + 0.0 * m_dTimeAcc;
				_float RotationZ = m_fBackUp_RotationZ * (1.0 - m_dTimeAcc) + 0.0 * m_dTimeAcc;
				m_pTransformCom->Set_Rotation(_float3(RotationX, RotationY, RotationZ));
				
				if (1.0 <= m_dTimeAcc)
				{
					m_pTransformCom->Set_Rotation(_float3(0.f,0.f, 0.f));
					m_dTimeAcc = 0.0;
					m_bChangeCheck = true;
				}
			}

			m_bPlayer_CollisionCheck = true;
		}
		else
		{
			if (true == m_bChangeCheck)
			{
				m_dTimeAcc += TimeDelta * 5;
				_float RotationX = 0.0 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationX * m_dTimeAcc;
				_float RotationY = 0.0 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationY * m_dTimeAcc;
				_float RotationZ = 0.0 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationZ * m_dTimeAcc;
				m_pTransformCom->Set_Rotation(_float3(RotationX, RotationY, RotationZ));

				if (1.0 <= m_dTimeAcc)
				{
					m_dTimeAcc = 0.0;
					m_bChangeCheck = false;
				}
			}

			m_bPlayer_CollisionCheck = false;
		}
	}
	else
	{
		if (true == m_bChangeCheck)
		{
			m_dTimeAcc += TimeDelta * 3;
			_float RotationX = 0.001 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationX * m_dTimeAcc;
			_float RotationY = 0.001 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationY * m_dTimeAcc;
			_float RotationZ = 0.001 * (1.0 - m_dTimeAcc) + m_fBackUp_RotationZ * m_dTimeAcc;
			m_pTransformCom->Set_Rotation(_float3(RotationX, RotationY, RotationZ));

			if (1.0 <= m_dTimeAcc)
			{
				m_dTimeAcc = 0.0;
				m_bChangeCheck = false;
			}
		}

		m_bPlayer_CollisionCheck = false;
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CRotationBlock::Player_Collision_Check(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
		return -1;
	if (true == m_bPlayer_CollisionCheck)
	{
		if (true == pTargetSphere->Collision_AABBXYZ(m_pAABB,
			&m_tPlayerCollisionDesc.bCollisionX, &m_tPlayerCollisionDesc.bCollisionY, &m_tPlayerCollisionDesc.bCollisionZ,
			&m_tPlayerCollisionDesc.fDisX, &m_tPlayerCollisionDesc.fDisY, &m_tPlayerCollisionDesc.fDisZ))
		{
			m_tPlayerCollisionDesc.bMainCollision = true;
			dynamic_cast<CRock*>(pPlayerObject)->Set_PlayerCollision(m_tPlayerCollisionDesc);
		}
	}
	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CRotationBlock::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.5f, 0.0f);
	ColliderDesc.vExtents = _float3(4.f, 1.5f, 4.f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.fRadius = 10.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_BigSphere"), (CComponent**)&m_pBigSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRotationBlock::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RotationBlock0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.5f, 0.0f);
	ColliderDesc.vExtents = _float3(2.f, 1.f, 2.f);
	ColliderDesc.fRadius = 2.f;


	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.fRadius = 10.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_BigSphere"), (CComponent**)&m_pBigSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRotationBlock::SetUp_ConstantTable()
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

CRotationBlock * CRotationBlock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRotationBlock* pInstance = new CRotationBlock(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CRotationBlock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRotationBlock::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRotationBlock* pInstance = new CRotationBlock(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CRotationBlock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRotationBlock::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pBigSphere);
	Safe_Release(m_pSphere);
	Safe_Release(m_pAABB);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
