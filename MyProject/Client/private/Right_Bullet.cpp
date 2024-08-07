#include "stdafx.h"
#include "..\public\Right_Bullet.h"
#include "GameInstance.h"
#include "Magenetic_Skill.h"
#include "BossCRT.h"
#include "Rock.h"
#include "Effect_Point.h"
#include "Camera_Fly.h"
#include "Sound_Manager.h"
CRight_Bullet::CRight_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRight_Bullet::CRight_Bullet(const CRight_Bullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRight_Bullet::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRight_Bullet::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	if (nullptr == m_pModelCom)
		return E_FAIL;
	if (nullptr != pArg)
	{
		MODELDESC_CLIENT tModelDesc;
		ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));
		memcpy(&tModelDesc, pArg, sizeof(MODELDESC_CLIENT));

		//tModelDesc.vPos.y += 0.6f;
		m_pTransformCom->Set_MyLook(-XMLoadFloat3(&tModelDesc.vRotaition));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tModelDesc.vPos));

	}



	CVIBuffer_PointInstance::POINTINST tPointInst;
	ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
	tPointInst.m_iType = 5;
	tPointInst.m_iSize = 5;
	tPointInst.m_iTextureIndex = 2;
	tPointInst.m_iPassIndex = 0;

	XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

_int CRight_Bullet::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (false == m_bDirGoCheck)
	{
		if (FAILED(Go_Direction(TimeDelta)))
			return -1;
	}

	if (FAILED(Magenetic_IsIn(TimeDelta)))
		return -1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	if (true == m_bGoCheck)
	{
		m_fAgeTime += TimeDelta;
		if (3.f <= m_fAgeTime)
		{
			m_fAgeTime = 0.f;
			HitEffect_Create(TimeDelta);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_ROKET_RIGHTDEAD);
			CSound_Manager::Get_Instance()->PlaySound(L"RoketHit", CSound_Manager::ID_ROKET_RIGHTDEAD);
			return 1;
		}
	}
	//else
	//{
	//	m_fAgeTime += TimeDelta;
	//	if (4.f <= m_fAgeTime)
	//	{
	//		m_fAgeTime = 0.f;
	//		return 1;
	//	}
	//}

	if (FAILED(Effect_Create(TimeDelta)))
		return -1;

	return _int();
}

_int CRight_Bullet::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (1 == Boss_CollisionCheck(TimeDelta))
	{
		HitEffect_Create(TimeDelta);
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_ROKET_RIGHTDEAD);
		CSound_Manager::Get_Instance()->PlaySound(L"RoketHit", CSound_Manager::ID_ROKET_RIGHTDEAD);
		return 1;
	}

	Floor_CollisionCheck(TimeDelta);
	if (true == m_bDeadCheck)
	{
		HitEffect_Create(TimeDelta);
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_ROKET_RIGHTDEAD);
		CSound_Manager::Get_Instance()->PlaySound(L"RoketHit", CSound_Manager::ID_ROKET_RIGHTDEAD);
		return 1;
	}


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);

	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
		return -1;

	if (m_pSphere->Collision_Sphere(pTargetSphere))
	{
		if (false == m_bDirGoCheck)
		{
			pPlayerObject->Set_Collision_Check(true);
			//이펙트텀둬야해서 return1하고 이펙트객체를생성해서 이펙트를보여준후에 return해주기로하자 일단은보류 사라지는것까지만 플레이어 히트모션을보자
			//히트모션끝`
			if (false == dynamic_cast<CRock*>(pPlayerObject)->Get_HitCheck())
			{
				CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
				if (nullptr == pCam)
				{
					MSGBOX("Failed to Finder : Right_Bullet Cam in : pCam");
					Safe_Release(pGameInstance);
					return -1;
				}

				dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
				dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(9);

				_float4 PlayerLook;
				XMStoreFloat4(&PlayerLook, pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				PlayerLook.y = 0;
				pTargetTransform->Set_MyLook(-XMLoadFloat4(&PlayerLook));
			}

			Safe_Release(pGameInstance);

			HitEffect_Create(TimeDelta);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_ROKET_RIGHTDEAD);
			CSound_Manager::Get_Instance()->PlaySound(L"RoketHit", CSound_Manager::ID_ROKET_RIGHTDEAD);
			return 1;
		}
	}
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	Safe_Release(pGameInstance);


	return 0;
}

HRESULT CRight_Bullet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pModelCom->Render(0);

#ifdef _DEBUG
	//m_pAABB->Render();
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CRight_Bullet::Effect_Create(_double TimeDelta)
{
	m_dCreateTimeAcc += TimeDelta;
	if (0.1 <= m_dCreateTimeAcc)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		MODELDESC	tModelDesc;
		ZeroMemory(&tModelDesc, sizeof(MODELDESC));
		XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - (XMVector4Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 2));

		if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Roket"), LEVEL_BOSS_STAGE, L"Clone_Effect_Roket", &tModelDesc)))
			return E_FAIL;

		Safe_Release(pGameInstance);
		m_dCreateTimeAcc = 0.0;
	}
	return S_OK;
}

HRESULT CRight_Bullet::HitEffect_Create(_double TimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	MODELDESC	tModelDesc;
	ZeroMemory(&tModelDesc, sizeof(MODELDESC));
	XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_RoketHit"), LEVEL_BOSS_STAGE, L"Clone_Effect_Roket", &tModelDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	m_dCreateTimeAcc = 0.0;

	return S_OK;
}

HRESULT CRight_Bullet::Floor_CollisionCheck(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CColider* pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, L"ColliderObject0.fbx", L"Com_AABB", 0, 0);
	if (nullptr == pTargetAABB)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);


	if (true == m_pSphere->Collision_SphereAABB(pTargetAABB))
	{
		m_bDeadCheck = true;
	}


	return S_OK;
}

_int CRight_Bullet::Boss_CollisionCheck(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pBoss = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Boss_CRT");
	CColider*	pBossHeadAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Boss_CRT", L"Com_AABB", 0, 0);
	CGameObject* pTargetSkillObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Magenetic_Skill");

	if (nullptr == pBoss || nullptr == pTargetSkillObject)
	{
		Safe_Release(pGameInstance);
		return -1;
	}
	if (true == dynamic_cast<CMagenetic_Skill*>(pTargetSkillObject)->Get_Magenetic_Bullet_Check())
	{
		if (true == m_pSphere->Collision_SphereAABB(pBossHeadAABB))
		{
			//Add 보스 피격입히는거처리
			dynamic_cast<CMagenetic_Skill*>(pTargetSkillObject)->Set_Magenetic_Bullet(false);
			dynamic_cast<CBossCRT*>(pBoss)->Set_HitMotion(TimeDelta);
			return 1;
		}
	}


	Safe_Release(pGameInstance);

	return 0;
}

HRESULT CRight_Bullet::Magenetic_IsIn(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	CTransform* pBossTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Boss_CRT", L"Com_Transform", 0, 0);
	CColider*	pTargetSkillSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Magenetic_Skill"), TEXT("Com_Sphere"), 0, 0);
	CGameObject* pTargetSkillObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Magenetic_Skill");
	if (nullptr == pTargetTransform)
	{
		MSGBOX("Failde to Finder Left_Bullet TargetTransform");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	Safe_Release(pGameInstance);

	if (false == pTargetSkillSphere->Get_CheckCollision() && m_pSphere->Collision_Sphere(pTargetSkillSphere))
	{
		_vector Dir = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float Length = XMVectorGetX(XMVector3Length(Dir));
		if (16.0 >= Length)
		{
			if (false == dynamic_cast<CMagenetic_Skill*>(pTargetSkillObject)->Get_Magenetic_Bullet_Check())
			{
				dynamic_cast<CMagenetic_Skill*>(pTargetSkillObject)->Set_Magenetic_Bullet(true);
				m_bMageneticCheck = true;
				m_bDirGoCheck = true;
			}
		}
	}
	if (true == m_bMageneticCheck)
	{
		m_fAgeTime = 0.0;
		if (false == m_bBossLookBackUpCheck)
		{
			XMStoreFloat3(&m_vBossDir, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			m_bBossLookBackUpCheck = true;
		}
		_vector Look = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_LOOK), pBossTransform->Get_State(CTransform::STATE_POSITION) - pTargetTransform->Get_State(CTransform::STATE_POSITION), 0.3);
		m_pTransformCom->Set_MyLook(Look);
		_float3 vDis = _float3(0.f, 3.f, 4.f);
		_vector Pos = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMVector3TransformCoord(XMLoadFloat3(&vDis), pTargetTransform->Get_WorldMatrix()), 0.05);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);
		if (true == pGameInstance->Key_Down('V'))
		{

			m_bGoCheck = true;
			m_bMageneticCheck = false;
		}
	}
	if (true == m_bGoCheck)
	{
		m_pTransformCom->Go_Straight(TimeDelta * 7);
	}

	return S_OK;
}

HRESULT CRight_Bullet::Go_Direction(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	if (nullptr == pTargetTransform)
	{
		MSGBOX("Failde to Finder Left_Bullet TargetTransform");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	Safe_Release(pGameInstance);

	_vector Dir = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float Length = XMVectorGetX(XMVector3Length(Dir));

	if (18.0 < Length)
	{
		m_pTransformCom->Go_Direction(TimeDelta * 6, XMVector3Normalize(Dir));
		m_pTransformCom->Set_MyLook(Dir);
	}
	else
		m_pTransformCom->Go_Straight(TimeDelta * 6);

	return S_OK;
}


HRESULT CRight_Bullet::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, L"Rocket2.fbx", TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(50.f, 90.f, 50.f);
	ColliderDesc.fRadius = 0.7f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRight_Bullet::SetUp_ConstantTable()
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

CRight_Bullet * CRight_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRight_Bullet*	pInstance = new CRight_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CRight_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRight_Bullet::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRight_Bullet*	pInstance = new CRight_Bullet(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CRight_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRight_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pAABB);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
