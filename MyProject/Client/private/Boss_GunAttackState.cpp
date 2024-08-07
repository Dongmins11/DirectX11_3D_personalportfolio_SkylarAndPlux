#include "stdafx.h"
#include "..\public\Boss_GunAttackState.h"
#include "Boss_IdleState.h"
#include "Camera_Fly.h"


IMPLEMENT_SINGLETON(CBoss_GunAttackState)
CBoss_GunAttackState::CBoss_GunAttackState()
{
}

void CBoss_GunAttackState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	if (false == m_bFirstCheck)
	{
		if (FAILED(SetUp_TargetBoneMatrices("indexhinge01_bind")))
		{
			MSGBOX("Failed to Serch indexhinge01_bind BossGunAttack");
			return;
		}
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
		CSound_Manager::Get_Instance()->PlaySound(L"BossRelroad", CSound_Manager::ID_BOSS_ATTCK);

		m_iLeftCurMotion = GUN_ATTACK_READY_MOTION;
		m_iRightCurMotion = GUN_ATTACK_READY_MOTION;
		m_bFirstCheck = true;
	}


	if (true == m_bRightAnimEndCheck)
	{
		m_bFirstCheck = false;
		m_bLeftAnimEndCheck = false;
		m_bRightAnimEndCheck = false;
		pBossAnimState->SetUp_State(CBoss_IdleState::Get_Instance());
		CBoss_IdleState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
		return;
	}

}

void CBoss_GunAttackState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{

	if (FAILED(Set_MatrixBone()))
		return;

	if (FAILED(Left_HandAnim(TimeDelta)))
		return;
	if (FAILED(Right_HandAnim(TimeDelta)))
		return;

	if (true == m_bLoopCheck)
	{
		m_iLeftCurMotion = GUN_ATTACK_MOTION;
		m_iRightCurMotion = GUN_ATTACK_MOTION;
		m_dAttackTimeAcc += TimeDelta;
		if (0.55 <= m_dAttackTimeAcc)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->PlaySound(L"GunShot", CSound_Manager::ID_BOSS_ATTCK);

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			MODELDESC_CLIENT tLeftModelDesc;
			ZeroMemory(&tLeftModelDesc, sizeof(MODELDESC_CLIENT));
			tLeftModelDesc.vPos = m_vLeftBonePos;
			XMStoreFloat3(&tLeftModelDesc.vRotaition, -m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Left_Bullet"), m_iMyLevel, L"Clone_Left_Bullet", &tLeftModelDesc)))
				return;

			CVIBuffer_PointInstance::POINTINST tPointInst;
			ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
			tPointInst.m_iType = 3;
			tPointInst.m_iSize = 5;
			tPointInst.m_iTextureIndex = 3;
			tPointInst.m_iPassIndex = 0;
			tPointInst.m_Matrix = m_LeftMatWorld;
			
			XMStoreFloat4(&tPointInst.m_vPosition, XMLoadFloat4(&m_vLeftBonePos));

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PetPointEffect", &tPointInst)))
				return;

			MODELDESC_CLIENT tRightModelDesc;
			ZeroMemory(&tRightModelDesc, sizeof(MODELDESC_CLIENT));
			tRightModelDesc.vPos = m_vRightBonePos;
			XMStoreFloat3(&tRightModelDesc.vRotaition, -m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));


			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Right_Bullet"), m_iMyLevel, L"Clone_Right_Bullet", &tRightModelDesc)))
				return;

			tPointInst.m_iType = 4;
			tPointInst.m_Matrix = m_RightMatWorld;
			XMStoreFloat4(&tPointInst.m_vPosition, XMLoadFloat4(&m_vRightBonePos));

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PetPointEffect", &tPointInst)))
				return;

			CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
			if (nullptr == pCam)
			{
				MSGBOX("Failed to Finder : Boss GunAttack Cam in : pCam");
				Safe_Release(pGameInstance);
				return;
			}

			dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
			dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(8);


			Safe_Release(pGameInstance);

			m_dAttackTimeAcc = 0.0;
			m_bLoopCheck = false;

		}
	}
	if (true == m_bLeftAnimEndCheck)
	{
		m_iLeftCurMotion = GUN_ATTACK_LOOP_MOTION;
		m_iRightCurMotion = GUN_ATTACK_LOOP_MOTION;
		m_bLeftAnimEndCheck = false;
		m_bLoopCheck = true;
	}




	if (m_iLeftCurMotion == GUN_ATTACK_READY_MOTION)
	{
		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta * 1.3f, &m_bLeftAnimEndCheck,true);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta * 1.3f ,nullptr ,true);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
	else if (m_iLeftCurMotion == GUN_ATTACK_LOOP_MOTION)
	{
		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
	else if (m_iLeftCurMotion == GUN_ATTACK_MOTION)
	{
		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta,&m_bRightAnimEndCheck);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
}

void CBoss_GunAttackState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
	m_bFirstCheck = false;
	m_bLeftAnimEndCheck = false;
	m_bRightAnimEndCheck = false;
}

void CBoss_GunAttackState::Set_Collision(CBoss_AnimState * pBossAnimState)
{

}

HRESULT CBoss_GunAttackState::Left_HandAnim(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	if (nullptr == pTargetTransform)
	{
		MSGBOX("Failed to Boss_Gun_Attack Find Target Transform");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}



	XMStoreFloat4(&m_vDistance, XMVectorSet(-3.f, 0., 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(LeftArmPos, CrtPos, 0.1);
	m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	_vector vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);

	m_pLeftHandTrasform->Set_MyLook(vLook);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBoss_GunAttackState::Right_HandAnim(_double TimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	if (nullptr == pTargetTransform)
	{
		MSGBOX("Failed to Boss_Gun_Attack Find Target Transform");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	XMStoreFloat4(&m_vDistance, XMVectorSet(3.f, 0., 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector RightArmPos = m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(RightArmPos, CrtPos, 0.1);
	m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	_vector vLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);

	m_pRightHandTrasform->Set_MyLook(vLook);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBoss_GunAttackState::SetUp_TargetBoneMatrices(const char * pBoneName)
{
	m_LeftOffsetMatrix = m_pLeftHandModel->Get_OffsetMatrix(pBoneName);
	m_LeftPivotMatrix = m_pLeftHandModel->Get_PivotMatrix();
	m_pLeftCombinedMatrix = m_pLeftHandModel->Get_CombinedMatrixPtr(pBoneName);
	m_pLeftTargetWorldMatrix = m_pLeftHandTrasform->Get_WorldMatrixPtr();

	_matrix LeftMatrix;
	LeftMatrix = XMMatrixIdentity();
	LeftMatrix.r[3] = XMVectorSet(0.42f, 0.3f, -2.20f, 1.f);
	XMStoreFloat4x4(&m_LeftMatWorld, LeftMatrix);

	m_RightOffsetMatrix = m_pRightHandModel->Get_OffsetMatrix(pBoneName);
	m_RightPivotMatrix = m_pRightHandModel->Get_PivotMatrix();
	m_pRightCombinedMatrix = m_pRightHandModel->Get_CombinedMatrixPtr(pBoneName);
	m_pRightTargetWorldMatrix = m_pRightHandTrasform->Get_WorldMatrixPtr();

	_matrix RightMatrix;
	RightMatrix = XMMatrixIdentity();
	RightMatrix.r[3] = XMVectorSet(0.39f, 0.35f, -2.20f, 1.f);
	XMStoreFloat4x4(&m_RightMatWorld, RightMatrix);


	return S_OK;
}

HRESULT CBoss_GunAttackState::Set_MatrixBone()
{

	if (nullptr != m_pLeftCombinedMatrix ||
		nullptr != m_pLeftTargetWorldMatrix)
	{
		_matrix		TransformMatrix = XMLoadFloat4x4(&m_LeftOffsetMatrix) * XMLoadFloat4x4(m_pLeftCombinedMatrix) * XMLoadFloat4x4(&m_LeftPivotMatrix) * XMLoadFloat4x4(m_pLeftTargetWorldMatrix);
		_float4x4	Matrix;
		XMStoreFloat4x4(&Matrix, XMLoadFloat4x4(&m_LeftMatWorld) * TransformMatrix);
		m_vLeftBonePos = *(_float4*)&Matrix.m[3];
	}


	if (nullptr != m_pRightCombinedMatrix ||
		nullptr != m_pRightTargetWorldMatrix)
	{
		_matrix		TransformMatrix = XMLoadFloat4x4(&m_RightOffsetMatrix) * XMLoadFloat4x4(m_pRightCombinedMatrix) * XMLoadFloat4x4(&m_RightPivotMatrix) * XMLoadFloat4x4(m_pRightTargetWorldMatrix);
		_float4x4	Matrix;
		XMStoreFloat4x4(&Matrix, XMLoadFloat4x4(&m_RightMatWorld) * TransformMatrix);
		m_vRightBonePos = *(_float4*)&Matrix.m[3];
	}

	return S_OK;
}

void CBoss_GunAttackState::Free()
{
}
