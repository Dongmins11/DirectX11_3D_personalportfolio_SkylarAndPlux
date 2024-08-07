#include "stdafx.h"
#include "..\Public\mTurret.h"
#include "GameInstance.h"
#include "Client_AnimationDefine.h"
#include "Rock.h"
#include "Turret_Bullet.h"

CmTerret::CmTerret(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CmTerret::CmTerret(const CmTerret & rhs)
	: CGameObject(rhs)
{
}

HRESULT CmTerret::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CmTerret::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		_vector Position = m_pNavigationCom->Get_IndexPoint(m_iCurrntIndex, CCell::POINT_A);
		_float4 vPos;
		XMStoreFloat4(&vPos, Position);
		vPos.x += 5.f;
		Position = XMLoadFloat4(&vPos);
		m_pTransformCom->Set_Scailing(0.01f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);
	}


	m_iCurMotion = 1;

	return S_OK;
}

_int CmTerret::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;


	m_pVIBufferCom->Set_Animation(m_iCurMotion);

	m_pVIBufferCom->Update(TimeDelta);

	m_pVIBufferCom->Set_PreAnimation(m_iCurMotion);

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pHitSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CmTerret::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	if (FAILED(Trace_Player(TimeDelta)))
		return -1;

	if (FAILED(Hit_Monster(TimeDelta)))
		return 1;


	return 0;
}

HRESULT CmTerret::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(0);

#ifdef _DEBUG
	m_pAABB->Render();
	m_pSphere->Render();
	m_pHitSphere->Render();
#endif // _DEBUG



	return S_OK;
}

HRESULT CmTerret::Trace_Player(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);
	if (nullptr == pTargetSphere || nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	if (true == m_pSphere->Collision_Sphere(pTargetSphere))
	{
		_vector TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		_vector MyLook = TargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
		_float4x4 PivotMat = m_pVIBufferCom->Get_PivotMatrix();
		XMStoreFloat4x4(&PivotMat, (XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(90.0f))));
		m_pVIBufferCom->Set_PivotMat(XMLoadFloat4x4(&PivotMat));
		m_pTransformCom->Set_MyLook(-MyLook);
		m_dTimeAcc += TimeDelta;

		if (2.0 <= m_dTimeAcc)
		{
			MODELDESC_CLIENT tModelDesc;
			ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));
			XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			_vector vPos;
			vPos = XMLoadFloat4(&tModelDesc.vPos);
			vPos -= XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 2.5;
			XMStoreFloat4(&tModelDesc.vPos, vPos);
			XMStoreFloat3(&tModelDesc.vRotaition, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_MONSTER_SHOT);
			CSound_Manager::Get_Instance()->PlaySound(L"Monster_Shot", CSound_Manager::ID_MONSTER_SHOT);

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Turret_Bullet"), m_iMyLevel, L"Clone_Tuerrt_Bullet",&tModelDesc)))
				return E_FAIL;
			m_iCurMotion = 0;
			m_dTimeAcc = 0.f;
		}
	}
	else
		m_iCurMotion = 1;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CmTerret::Hit_Monster(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_HandSphere"), 0, 0);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere || nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL; 
	}

	if (false == pTargetSphere->Get_CheckCollision() && m_pHitSphere->Collision_Sphere(pTargetSphere))
	{
		if (false == m_bCheckCollision)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_MONSTER_HIT);
			CSound_Manager::Get_Instance()->PlaySound(L"Monster_Hit", CSound_Manager::ID_MONSTER_HIT);

			_vector TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_vBackDirection, TargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			m_vBackDirection.y = 0;
			m_pSphere->Set_CheckCollision(true);
			m_bCheckCollision = true;
		}
	}


	if (true == m_bCheckCollision)
	{
		m_dCollisionTimeAcc += TimeDelta;

		if (ATTACKMOTION == dynamic_cast<CRock*>(pPlayerObject)->Get_CurrentAnimation())
		{
			m_pTransformCom->Go_BackDirection(TimeDelta * 2.0, XMLoadFloat3(&m_vBackDirection));
			if (0.5 <= m_dCollisionTimeAcc)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_MONSTER_DEAD);
				CSound_Manager::Get_Instance()->PlaySound(L"Monster_Dead", CSound_Manager::ID_MONSTER_DEAD);

				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				CVIBuffer_PointInstance::POINTINST tPointInst;
				ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
				tPointInst.m_iType = 0;
				tPointInst.m_iSize = 5;
				tPointInst.m_iTextureIndex = 2;
				tPointInst.m_iPassIndex = 0;
				XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PointEffect", &tPointInst)))
					return E_FAIL;

				Safe_Release(pGameInstance);

				m_dCollisionTimeAcc = 0.0;
				m_pSphere->Set_CheckCollision(false);
				m_bCheckCollision = false;
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
		}
		else
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_MONSTER_DEAD);
			CSound_Manager::Get_Instance()->PlaySound(L"Monster_Dead", CSound_Manager::ID_MONSTER_DEAD);
			
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CVIBuffer_PointInstance::POINTINST tPointInst;
			ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
			tPointInst.m_iType = 0;
			tPointInst.m_iSize = 5;
			tPointInst.m_iTextureIndex = 2;
			tPointInst.m_iPassIndex = 0;
			XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PointEffect", &tPointInst)))
				return E_FAIL;

			Safe_Release(pGameInstance);

			m_dCollisionTimeAcc = 0.0;
			m_pSphere->Set_CheckCollision(false);
			m_bCheckCollision = false;
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CmTerret::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, L"mTurret0.fbx", TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	
	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 4;
	m_iCurrntIndex = NaviDesc.iCurrentIndex;
	/* Com_Navigation */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_VIBuffer_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 80.f, 0.0f);
	ColliderDesc.vExtents = _float3(50.f, 90.f, 50.f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.fRadius = 70.f;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_HitSphere"), (CComponent**)&m_pHitSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CmTerret::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CmTerret * CmTerret::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CmTerret*	pInstance = new CmTerret(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CmTerret");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CmTerret::Clone(void* pArg, _uint iMyLevelIndex)
{
	CmTerret*	pInstance = new CmTerret(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CmTerret");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CmTerret::Free()
{
	__super::Free();
	Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAABB);
	Safe_Release(m_pSphere);
	Safe_Release(m_pHitSphere);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);

}
