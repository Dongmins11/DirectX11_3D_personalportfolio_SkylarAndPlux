#include "stdafx.h"
#include "..\public\Scaffolding.h"
#include "GameInstance.h"
#include "Rock.h"


CScaffolding::CScaffolding(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CScaffolding::CScaffolding(const CScaffolding & rhs)
	: CGameObject(rhs)
{
}

HRESULT CScaffolding::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CScaffolding::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 13.f, 106.f, 1.f));
		m_pTransformCom->Set_Scailing(0.5f);

	}

	m_vColor = _float4(0.f, 1.f, 0.f, 1.f);

	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int CScaffolding::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	return 0;
}

_int CScaffolding::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Collision_Check()))
		return -1;

	if (FAILED(My_Collision_Check()))
		return -1;

	ColorChange(TimeDelta);
	DownChange(TimeDelta);


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CScaffolding::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pSpinnigModelCom->Render(4)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pAABB->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CScaffolding::Collision_Check()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return -1;

	if (false == m_bPlayer_CollisionCheck)
	{
		if (true == pTargetAABB->Collision_AABBXYZ(m_pAABB,
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

HRESULT CScaffolding::My_Collision_Check()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return -1;

	if (true == m_pAABB->Collision_AABB(pTargetAABB) && false == m_dDownCheck)
	{
		m_dDownCheck = true;
		
	}



	Safe_Release(pGameInstance);

	return S_OK;
}

void CScaffolding::ColorChange(_double TimeDelta)
{
	if (true == m_dDownCheck && false == m_bTransCheck)
	{
		m_dColorTimeAcc += TimeDelta;
		
		if (1.f <=m_vColor.x && 1.f <= m_vColor.y)
		{
			m_vColor.x = 1.f;
			m_dColorTimeAcc = 0.01;
			m_bGreenColor = true;
		}
		else if(false == m_bGreenColor)
			m_vColor.x = 0.01f * (1.0 - m_dColorTimeAcc) + 1.f * m_dColorTimeAcc;
		
		if (true == m_bGreenColor)
		{
			if (0.01f >= m_vColor.y)
			{
				m_vColor.y = 0.01f;
				m_dColorTimeAcc = 0.0;
				m_bGreenColor = false;
				m_bTransCheck = true;
				m_bPlayer_CollisionCheck = true;
				XMStoreFloat4(&m_vDsetBackupPos,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			}
			else
				m_vColor.y = 1.0f * (1.0 - m_dColorTimeAcc) + 0.01f * m_dColorTimeAcc;
		}
	}


}

void CScaffolding::DownChange(_double TimeDelta)
{

	if (true == m_bTransCheck)
	{
		m_dDownTimeAcc += TimeDelta;
		
		m_pTransformCom->Set_PosGravity(-m_dDownTimeAcc);
			
		if (1.0 <= m_dDownTimeAcc)
		{
			XMStoreFloat4(&m_vSourceBackupPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			m_dDownTimeAcc = 0.0;
			m_bTransCheck = false;
			m_bTransUpCheck = true;
		}
	}


	if (true == m_bTransUpCheck)
	{
		_vector		vPosition;
		m_dDownTimeAcc += TimeDelta * 0.5;

		vPosition = XMLoadFloat4(&m_vSourceBackupPos) * (1.0 - m_dDownTimeAcc) + XMLoadFloat4(&m_vDsetBackupPos) * m_dDownTimeAcc;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		if (1.0 <= m_dDownTimeAcc)
		{
			m_dDownTimeAcc = 0.0;
			m_vColor = _float4(0.f, 1.f, 0.f, 1.f);
			m_bPlayer_CollisionCheck = false;
			m_dDownCheck = false;
			m_bTransUpCheck = false;
		}
	}



}

HRESULT CScaffolding::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("SpinnigSphere0.fbx"), TEXT("Com_SpinnigModel"), (CComponent**)&m_pSpinnigModelCom)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.2f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 1.2f, 4.7f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScaffolding::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Scaffolding0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("SpinnigSphere0.fbx"), TEXT("Com_SpinnigModel"), (CComponent**)&m_pSpinnigModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.2f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 1.2f, 4.7f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CScaffolding::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pSpinnigModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pSpinnigModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pSpinnigModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	if (FAILED(m_pSpinnigModelCom->Set_RawValue("g_SpinnigColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

CScaffolding * CScaffolding::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScaffolding* pInstance = new CScaffolding(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CScaffolding");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CScaffolding::Clone(void* pArg, _uint iMyLevelIndex)
{
	CScaffolding* pInstance = new CScaffolding(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CScaffolding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScaffolding::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		if (nullptr != m_pModelDesc_Client)
		{
			Safe_Delete(m_pModelDesc_Client);
		}
	}

	Safe_Release(m_pAABB);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSpinnigModelCom);
	Safe_Release(m_pTransformCom);
}
