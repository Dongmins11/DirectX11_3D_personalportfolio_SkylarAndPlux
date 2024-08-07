#include "stdafx.h"
#include "..\public\ChangeFloor.h"
#include "GameInstance.h"
#include "Rock.h"

ChangeFloor::ChangeFloor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

ChangeFloor::ChangeFloor(const ChangeFloor & rhs)
	: CGameObject(rhs)
{
}

HRESULT ChangeFloor::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ChangeFloor::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTransformCom->Set_Scailing(1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 10.f, 126.f, 1.f));
	}


	XMStoreFloat3(&m_vDestScale, m_pTransformCom->Get_Scale());
	_vector Scale;
	Scale = XMLoadFloat3(&m_vDestScale);
	XMStoreFloat3(&m_vSourceScale, Scale * 1.5f);
	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int ChangeFloor::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Change_Scale(TimeDelta)))
		return -1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int ChangeFloor::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Collision_Check()))
		return -1;

	if (true == m_bRenderChange)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
			return E_FAIL;
	}

	m_dChangeTimeAcc += TimeDelta;

	return 0;
}

HRESULT ChangeFloor::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(Change_Model()))
		return E_FAIL;

#ifdef _DEBUG
	m_pAABB->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT ChangeFloor::Change_Scale(_double TimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;


	if (true == m_bIsNonChange)
	{
		m_dTimeAcc += TimeDelta * 4.0f;
		_float vScale = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&m_vDestScale), XMLoadFloat3(&m_vSourceScale), (_float)m_dTimeAcc));
		m_pTransformCom->Set_Scailing(vScale);
		if (1.0 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			m_bIsNonChange = false;
			m_bIsChange = true;
		}
	}

	if (true == m_bIsChange)
	{
		m_dTimeAcc += TimeDelta * 5.0f;
		_float vScale = XMVectorGetX(XMVectorLerp(XMLoadFloat3(&m_vSourceScale), XMLoadFloat3(&m_vDestScale), (_float)m_dTimeAcc));
		m_pTransformCom->Set_Scailing(vScale);
		if (1.0 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			m_bIsChange = false;
			m_bRenderChange = true;
		}
	}


	return S_OK;
}

HRESULT ChangeFloor::Change_Model()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;


	if (false == m_bRenderChange)
	{
		if (3.f <= m_dChangeTimeAcc)
		{
			m_dChangeTimeAcc = 0.0;
			m_bIsNonChange = true;
		}
		if (FAILED(m_pModelCom->Render(0)))
			return E_FAIL;
	}

	if (true == m_bRenderChange)
	{
		if (3.f <= m_dChangeTimeAcc)
		{
			m_dChangeTimeAcc = 0.0;
			m_bRenderChange = false;
		}
		
		if (FAILED(m_pModelCom->Render(5)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT ChangeFloor::Collision_Check()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return -1;

	if (false == m_bRenderChange)
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


HRESULT ChangeFloor::SetUp_Component_Load()
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

	ColliderDesc.vCenter = _float3(0.0f, 0.1f, 0.0f);
	ColliderDesc.vExtents = _float3(2.7f, 0.8f, 2.7f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT ChangeFloor::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("ChangeFloor0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.1f, 0.0f);
	ColliderDesc.vExtents = _float3(2.7f, 0.8f, 2.7f);
	ColliderDesc.fRadius = 500.f;


	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT ChangeFloor::SetUp_ConstantTable()
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

ChangeFloor * ChangeFloor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	ChangeFloor* pInstance = new ChangeFloor(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create ChangeFloor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * ChangeFloor::Clone(void* pArg, _uint iMyLevelIndex)
{
	ChangeFloor* pInstance = new ChangeFloor(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create ChangeFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ChangeFloor::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pAABB);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
