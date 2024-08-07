#include "stdafx.h"
#include "..\public\BossCRT.h"
#include "GameInstance.h"
#include "Boss_AnimState.h"
#include "Rock.h"
#include "NpcSphere.h"
#include "Camera_Fly.h"
#include "FadeInOut_UI.h"
#include "Boss_Ui.h"
#include "Sound_Manager.h"
#include "Effect_FireCrt.h"


#include "Magenetic_Ui.h"
#include "Hp_Ui.h"
#include "JetPack_Ui.h"
#include "Crystal_Ui.h"
#include "Box_Ui.h"


CBossCRT::CBossCRT(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBossCRT::CBossCRT(const CBossCRT & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBossCRT::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossCRT::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	}


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(131.4f, 27.0f, 143.84f, 1.f));
	m_pTransformCom->Set_Scailing(10.f);

	m_pLeftTransformCom->Set_Scailing(0.1f);
	m_pRightTransformCom->Set_Scailing(0.1f);

	m_pAnimState = CBoss_AnimState::Create(this, m_pTransformCom, m_pLeftTransformCom, m_pRightTransformCom, m_pModelCom, m_pLeftModelCom, m_pRightModelCom, m_pAABB, m_pLeftAABB, m_pRightAABB, m_iMyLevel);
	if (nullptr == m_pAnimState)
	{
		MSGBOX("Failed to Boss AnimState");
	}
	m_pLeftModelCom->Set_Animation(1);
	m_pLeftModelCom->Set_PreAnimation(1);

	m_pRightModelCom->Set_Animation(1);
	m_pRightModelCom->Set_PreAnimation(1);

	m_iHp = 100;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pLeftAABB->Update(m_pLeftTransformCom->Get_WorldMatrix());
	m_pRightAABB->Update(m_pRightTransformCom->Get_WorldMatrix());


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Boss_Ui"), LEVEL_BOSS_STAGE, L"Clone_Boss_Ui",nullptr,(CGameObject**)&m_pBoss_Ui)))
		return E_FAIL;

	m_pBoss_Ui->Set_HitHp(m_iHp);

	Safe_Release(pGameInstance);

	return S_OK;
}

_int CBossCRT::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;


	if (true == m_bRenderCheck)
	{
		if (FAILED(Anim_Start(TimeDelta)))
			return E_FAIL;

		m_bFristCheck = true;
	}


	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pLeftAABB->Update(m_pLeftTransformCom->Get_WorldMatrix());
	m_pRightAABB->Update(m_pRightTransformCom->Get_WorldMatrix());
	m_pLeftSphere->Update(m_pLeftTransformCom->Get_WorldMatrix());
	m_pRightSphere->Update(m_pRightTransformCom->Get_WorldMatrix());


	if (true == m_bLastCheck)
	{
		if (false == m_bHitCheck)
		{
			if (FAILED(Boss_Look(TimeDelta)))
				return E_FAIL;

			if (FAILED(Lerp_Y(TimeDelta)))
				return E_FAIL;

		}
		m_pAnimState->Update(TimeDelta);
	}
	else
	{
		m_pLeftModelCom->Set_Animation(1);
		m_pLeftModelCom->Update(TimeDelta);
		m_pLeftModelCom->Set_PreAnimation(1);

		m_pRightModelCom->Set_Animation(1);
		m_pRightModelCom->Update(TimeDelta);
		m_pRightModelCom->Set_PreAnimation(1);

		if (FAILED(Boss_Look(TimeDelta)))
			return E_FAIL;

		if (FAILED(Lerp_LeftHand(TimeDelta)))
			return E_FAIL;

		if (FAILED(Lerp_RightHand(TimeDelta)))
			return E_FAIL;
	}


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (pGameInstance->Key_Down('K'))
		m_iHp = 0;

	Safe_Release(pGameInstance);

	m_pBoss_Ui->Set_HitHp(m_iHp);


	return 0;
}

_int CBossCRT::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Left_Player_Collision(TimeDelta)))
		return -1;

	if (FAILED(Right_Player_Collision(TimeDelta)))
		return -1;

	if (1 == Hit_Crt(TimeDelta))
	{
		CSound_Manager::Get_Instance()->StopAll();
		return 1;
	}

	if (true == m_bRenderCheck)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
			return E_FAIL;
	}

	if (false == m_bFaceCheck)
		m_iFaceIndex = 0;
	else
		m_iFaceIndex = 15;

	return 0;
}

HRESULT CBossCRT::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;
	if(false == m_bFaceCheck)
	{
		if (FAILED(m_pModelCom->Render(0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTwoModelCom->Render(0)))
			return E_FAIL;
	}

	if (FAILED(m_pLeftModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pRightModelCom->Render(2)))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable_Buffer()))
		return E_FAIL;


#ifdef _DEBUG
	m_pLeftAABB->Render();
	m_pLeftSphere->Render();
	m_pRightAABB->Render();
	m_pRightSphere->Render();
	m_pAABB->Render();
	
#endif // _DEBUG

	return S_OK;
}

void CBossCRT::Set_HitMotion(_double TimeDelta)
{
	if (nullptr == m_pAnimState)
		return;
	m_pAnimState->Boss_Hit_Check();
}

_int CBossCRT::Hit_Crt(_double TimeDelta)
{
	if (0 >= m_iHp)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject* pNpcSphere = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Npc_Sphere");
		CGameObject* pFadeOut = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_FadeInOut_Ui");
		CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
		CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		CTransform* pNpcTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_BossNpc", L"Com_Transform", 0, 0);
		CGameObject* pTargetObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Effect_FireCrt");
		
		CGameObject* pCrystal_Ui = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Crystal_Ui");;
		CGameObject* pBox_Ui = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Box_Ui");;
		CGameObject* pJetPack_Ui = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_JetPack_Ui");;
		CGameObject* pMagenetic_Ui = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Magenetic_Ui");;

		
		if (nullptr == pNpcSphere)
			return -1;
		dynamic_cast<CFadeInOut_UI*>(pFadeOut)->Set_Value(2);
		dynamic_cast<CFadeInOut_UI*>(pFadeOut)->Set_ShutCheck_Open(true);


		pPlayerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(97.5f, 6.9f, 143.6f, 1.f));
		_float3 Dir;
		XMStoreFloat3(&Dir, pNpcTransform->Get_State(CTransform::STATE_POSITION) - pPlayerTransform->Get_State(CTransform::STATE_POSITION));
		Dir.y = 0;
		pPlayerTransform->Set_MyLook(XMLoadFloat3(&Dir));

		m_pBoss_Ui->Set_RenderCheck(false);
		dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
		dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(7);
		dynamic_cast<CNpcSphere*>(pNpcSphere)->Set_BossDeadCheck(true);
		dynamic_cast<CEffect_FireCrt*>(pTargetObject)->Set_DeadCheck(true);
		
		dynamic_cast<CRock*>(pPlayerObject)->Set_All_HP_RenderCheck(true);
		dynamic_cast<CCrystal_Ui*>(pCrystal_Ui)->Set_RenderCheck(true);
		dynamic_cast<CBox_Ui*>(pBox_Ui)->Set_RenderCheck(true);
		dynamic_cast<CJetPack_Ui*>(pJetPack_Ui)->Set_JetPackUi_Render(true);
		dynamic_cast<CMagenetic_Ui*>(pMagenetic_Ui)->Set_RenderCheck(true);

		Safe_Release(pGameInstance);
		return 1;
	}
	
	return _int();
}


HRESULT CBossCRT::Lerp_RightHand(_double TimeDelta)
{
	m_vDistance = XMVectorSet(3.f, 0., 0.f, 1.f);
	_vector CrtPos = XMVector3TransformCoord(m_vDistance, m_pTransformCom->Get_WorldMatrix());
	_vector RightArmPos = m_pRightTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, RightArmPos, 0.1);
	m_pRightTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pRightTransformCom->Set_MyLook(-m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBossCRT::Anim_Start(_double TimeDelta)
{
	if (true == m_bAnimCheck)
	{
		m_dRatio += TimeDelta * 0.5;

		_vector vPos = XMVectorCatmullRom(XMVectorSet(51.2, 80.f, 143.8, 1.f), XMVectorSet(71.2, 75.f, 113.8, 1.f),
			XMVectorSet(91.2, 60.f, 95.8, 1.f), XMVectorSet(131.2, 27.0f, 143.8, 1.f),  m_dRatio);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		if (1.65 <= m_dRatio)
		{
			m_dRatio = 0.0;
			m_bAnimCheck = false;
			m_bNextCheck = true;
		}
	}
	if (true == m_bNextCheck)
	{
		m_dRatio += TimeDelta * 0.4;
		_vector vPos = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMVectorSet(131.4f, 27.0f, 143.84f, 1.f), m_dRatio);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		if (1.0 <= m_dRatio)
		{
			m_dRatio = 0.0;
			m_pBoss_Ui->Set_RenderCheck(true);
			m_bNextCheck = false;
			m_bLastCheck = true;
		}
	
	}


	return S_OK;
}

HRESULT CBossCRT::Lerp_Y(_double TimeDelta)
{
	if (false == m_YCheck)
	{
		m_dLerpTime += TimeDelta * 0.5;

		m_fDisY = 27 * (1.0 - m_dLerpTime) + 32 * m_dLerpTime;

		if (1.0 <= m_dLerpTime)
		{
			m_dLerpTime = 0.0;
			m_YCheck = true;
		}
	}
	else
	{
		m_dLerpTime += TimeDelta * 0.5;

		m_fDisY = 32 * (1.0 - m_dLerpTime) + 27 * m_dLerpTime;

		if (1.0 <= m_dLerpTime)
		{
			m_dLerpTime = 0.0;
			m_YCheck = false;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(131.4f, m_fDisY, 143.84f, 1.f));

	return S_OK;
}

HRESULT CBossCRT::Left_Player_Collision(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform",0,0);
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return -1;


	if (true == m_pLeftSphere->Collision_SphereAABB(pTargetAABB))
	{
		if(true == m_pLeftAABB->Collision_OBBAABB(pTargetAABB))
		{
			//pPlayerObject->Set_Collision_Check(true);
			if (false == m_bBounceCheck)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_HIT);
				CSound_Manager::Get_Instance()->PlaySound(L"PunchHit", CSound_Manager::ID_PLAYER_HIT);

				_float3 Dir;
				XMStoreFloat3(&Dir, -pTargetTransform->Get_State(CTransform::STATE_LOOK));
				Dir.y = 0.2f;
				dynamic_cast<CRock*>(pPlayerObject)->Set_Bounce(Dir, 25.f, true);

				m_bBounceCheck = true;
			}
		}
	}
	
	if (true == m_bBounceCheck)
	{
		m_bBounceTimeAcc += TimeDelta;
		if (2.0 <= m_bBounceTimeAcc)
		{
			m_bBounceTimeAcc = 0.0;
			m_bBounceCheck = false;
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;

}

HRESULT CBossCRT::Right_Player_Collision(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	CColider*		pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetAABB)
		return -1;


	if (true == m_pRightSphere->Collision_SphereAABB(pTargetAABB))
	{
		if (true == m_pRightAABB->Collision_OBBAABB(pTargetAABB))
		{
			//pPlayerObject->Set_Collision_Check(true);
			if (false == m_bBounceCheck)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_HIT);
				CSound_Manager::Get_Instance()->PlaySound(L"PunchHit", CSound_Manager::ID_PLAYER_HIT);

				_float3 Dir;
				XMStoreFloat3(&Dir, -pTargetTransform->Get_State(CTransform::STATE_LOOK));
				Dir.y = 0.2f;
				dynamic_cast<CRock*>(pPlayerObject)->Set_Bounce(Dir, 25.f, true);

				m_bBounceCheck = true;
			}
		}
	}

	if (true == m_bBounceCheck)
	{
		m_bBounceTimeAcc += TimeDelta;
		if (2.0 <= m_bBounceTimeAcc)
		{
			m_bBounceTimeAcc = 0.0;
			m_bBounceCheck = false;
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CBossCRT::Boss_Look(_double TimeDelta)
{
	_vector BossLook = m_pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vLook;
	XMStoreFloat3(&vLook, BossLook);
	vLook.y = 0.f;
	m_pTransformCom->Set_MyLook(-XMLoadFloat3(&vLook));

	return S_OK;
}

HRESULT CBossCRT::Lerp_LeftHand(_double TimeDelta)
{
	m_vDistance = XMVectorSet(-3.f, 0., 0.f, 1.f);
	_vector CrtPos = XMVector3TransformCoord(m_vDistance, m_pTransformCom->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, LeftArmPos, 0.1);
	m_pLeftTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pLeftTransformCom->Set_MyLook(-m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}



HRESULT CBossCRT::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.0f;


	/* Com_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossCRT::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("CRTHead1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("CRTHead0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pTwoModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BossRightHand0.fbx"), TEXT("Com_LeftModel"), (CComponent**)&m_pLeftModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BossLeftHand0.fbx"), TEXT("Com_RightModel"), (CComponent**)&m_pRightModelCom)))
		return E_FAIL;


	

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftTransform"), (CComponent**)&m_pLeftTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightTransform"), (CComponent**)&m_pRightTransformCom, &TransformDesc)))
		return E_FAIL;

	
	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.fRadius = 200.0f;

	/* Com_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vCenter = _float3(0.0f, 70.f, 50.0f);
	ColliderDesc.vExtents = _float3(60.f, 80.f, 80.f);

	/* Com_OBB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_LeftOBB"), (CComponent**)&m_pLeftAABB, &ColliderDesc)))
		return E_FAIL;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_LeftSphere"), (CComponent**)&m_pLeftSphere, &ColliderDesc)))
		return E_FAIL;



	ColliderDesc.vCenter = _float3(30.0f, 70.f, 50.0f);
	ColliderDesc.vExtents = _float3(60.f, 80.f, 80.f);

	/* Com_OBB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_RightOBB"), (CComponent**)&m_pRightAABB, &ColliderDesc)))
		return E_FAIL;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_RightSphere"), (CComponent**)&m_pRightSphere, &ColliderDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_BossFace"), TEXT("Com_Texture"), (CComponent**)&m_pFaceTexture)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_BossEffect"), TEXT("Com_Texture"), (CComponent**)&m_pEffectTexture)))
		return E_FAIL;

	

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pTargetTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0));
	if (nullptr == m_pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	Safe_AddRef(m_pTargetTransform);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBossCRT::SetUp_Component_Buffer()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_LeftUpVIBuffer"), (CComponent**)&m_pLeftUpBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_RightUpVIBuffer"), (CComponent**)&m_pRightUpBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_LeftDownVIBuffer"), (CComponent**)&m_pLeftDownBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_RightDownVIBuffer"), (CComponent**)&m_pRightDownBuffer)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftUpTransform"), (CComponent**)&m_pLeftUpTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pLeftUpTransformCom->Set_Rotation(_float3(0.f, 0.f, 120.f));
	m_pLeftUpTransformCom->Set_ScailingXYZTwo(_float3(4.f,9.f,4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftDownTransform"), (CComponent**)&m_pLeftDownTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pLeftDownTransformCom->Set_Rotation(_float3(0.f, 0.f, 180.f));
	m_pLeftDownTransformCom->Set_ScailingXYZTwo(_float3(4.f, 9.f, 4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightUpTransform"), (CComponent**)&m_pRightUpTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pRightUpTransformCom->Set_Rotation(_float3(0.f, 0.f, 120.f));
	m_pRightUpTransformCom->Set_ScailingXYZTwo(_float3(4.f, 9.f, 4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightDownTransform"), (CComponent**)&m_pRightDownTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pRightDownTransformCom->Set_Rotation(_float3(0.f, 0.f, 180.f));


	return S_OK;
}

HRESULT CBossCRT::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pTwoModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pTwoModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pTwoModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pLeftModelCom->Set_RawValue("g_WorldMatrix", &m_pLeftTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLeftModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLeftModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pRightModelCom->Set_RawValue("g_WorldMatrix", &m_pRightTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRightModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRightModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBossCRT::SetUp_ConstantTable_Buffer()
{
	if (true == m_bFristCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CGameObject* pTargetObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Effect_FireCrt");
		Safe_Release(pGameInstance);

		if (false == m_bEffectRenderCheck)
			dynamic_cast<CEffect_FireCrt*>(pTargetObject)->Set_RenderCheck(true);
		else
			dynamic_cast<CEffect_FireCrt*>(pTargetObject)->Set_RenderCheck(false);
	}
	
	return S_OK;
}

CBossCRT * CBossCRT::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBossCRT* pInstance = new CBossCRT(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CBossCRT");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBossCRT::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBossCRT* pInstance = new CBossCRT(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CBossCRT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossCRT::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pEffectTexture);
	Safe_Release(m_pBoss_Ui);
	Safe_Release(m_pLeftSphere);
	Safe_Release(m_pRightSphere);
	Safe_Release(m_pAnimState);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pAABB);
	Safe_Release(m_pLeftAABB);
	Safe_Release(m_pRightAABB);
	Safe_Release(m_pLeftModelCom);
	Safe_Release(m_pRightModelCom);
	Safe_Release(m_pTwoModelCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pLeftTransformCom);
	Safe_Release(m_pRightTransformCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTargetTransform);
}
