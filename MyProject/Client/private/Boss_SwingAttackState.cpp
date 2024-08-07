#include "stdafx.h"
#include "..\public\Boss_SwingAttackState.h"
#include "GameInstance.h"
#include "Boss_IdleState.h"
#include "Boss_GunAttackState.h"

IMPLEMENT_SINGLETON(CBoss_SwingAttackState)
CBoss_SwingAttackState::CBoss_SwingAttackState()
{
}

void CBoss_SwingAttackState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	if (false == m_bFirstCheck)
	{
		if (FAILED(SetUp_TargetBoneMatrices("indexhinge01_bind")))
		{
			MSGBOX("Failed to Serch indexhinge01_bind BossGunAttack");
			return;
		}

		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
		CSound_Manager::Get_Instance()->PlaySound(L"BossEletric", CSound_Manager::ID_BOSS_ATTCK);

		m_iLeftCurMotion = SWING_ATTACK_LOOP_MOTION;
		m_iRightCurMotion = SWING_ATTACK_LOOP_MOTION;

		m_bFirstCheck = true;
	}

	if (true == m_bLeftPosEndCheck)
	{
		m_LeftEffectCheckOne = false;
		m_LeftEffectCheckTwo = false;
		m_LeftEffectCheckThree = false;
		m_LeftEffectCheckFor = false;
		m_LeftEffectCheckFive = false;
		m_LeftEffectCheckSix = false;

		m_RightEffectCheckOne = false;
		m_RightEffectCheckTwo = false;
		m_RightEffectCheckThree = false;
		m_RightEffectCheckFor = false;
		m_RightEffectCheckFive = false;
		m_RightEffectCheckSix = false;

		m_dLeftAnimRation = 0.0;
		m_bSecondChangeCheck = !m_bSecondChangeCheck;
		m_bFirstCheck = false;
		m_bLeftPosEndCheck = false;
		m_bChangeCheck = false;
		pBossAnimState->SetUp_State(CBoss_IdleState::Get_Instance());
		CBoss_IdleState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
		return;
	}

}

void CBoss_SwingAttackState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{
	if (FAILED(Set_MatrixBone()))
		return;

	if (false == m_bSecondChangeCheck)
	{
		if (FAILED(Left_HandAnim(TimeDelta)))
			return;
		if (FAILED(Right_HandAnim(TimeDelta)))
			return;
	}
	else
	{
		if (FAILED(Second_Left_HandAnim(TimeDelta)))
			return;
		if (FAILED(Second_Right_HandAnim(TimeDelta)))
			return;
	}

	m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
	m_pLeftHandModel->Update(TimeDelta);
	m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

	m_pRightHandModel->Set_Animation(m_iRightCurMotion);
	m_pRightHandModel->Update(TimeDelta);
	m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);

}

void CBoss_SwingAttackState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
	m_dLeftAnimRation = 0.0;
	m_bFirstCheck = false;
	m_bLeftPosEndCheck = false;
	m_bChangeCheck = false;

	m_LeftEffectCheckOne = false;
	m_LeftEffectCheckTwo = false;
	m_LeftEffectCheckThree = false;
	m_LeftEffectCheckFor = false;
	m_LeftEffectCheckFive = false;
	m_LeftEffectCheckSix = false;

	m_RightEffectCheckOne = false;
	m_RightEffectCheckTwo = false;
	m_RightEffectCheckThree = false;
	m_RightEffectCheckFor = false;
	m_RightEffectCheckFive = false;
	m_RightEffectCheckSix = false;
}

void CBoss_SwingAttackState::Set_Collision(CBoss_AnimState * pBossAnimState)
{
}

HRESULT CBoss_SwingAttackState::Left_HandAnim(_double TimeDelta)
{
	if (false == m_bChangeCheck)
	{
		m_dLeftAnimRation += TimeDelta * 0.8;
		if (false == m_bLeftPosSaveCheck)
		{
			XMStoreFloat4(&m_vLeftPos, m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
			_vector Dir = m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK);
			_vector Right = m_pLeftHandTrasform->Get_State(CTransform::STATE_RIGHT);
			XMVector3Normalize(Dir);
			XMVector3Normalize(Right);
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) += (Dir * 250));
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) -= (Right * 100));
			m_vLeftPos.y = m_vLeftPos.y - 12.f;

			m_bLeftPosSaveCheck = true;
		}

		_double RotX = 0.0 * (1.0 - m_dLeftAnimRation) + 45.0 * m_dLeftAnimRation;
		_double RotZ = 0.0 * (1.0 - m_dLeftAnimRation) + 90.0 * m_dLeftAnimRation;

		_vector vPos = XMVectorLerp(m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&m_vLeftPos), m_dLeftAnimRation);
		//m_pLeftHandTrasform->YawRollPitch_Set(_float3(0.f,0.f , -RotZ));
		m_pLeftHandTrasform->Turn(m_pLeftHandTrasform->Get_State(CTransform::STATE_RIGHT), TimeDelta * 0.5);
		m_pLeftHandTrasform->Turn(m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK), -TimeDelta * 0.5);
		m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);
		if (1.0 <= m_dLeftAnimRation)
		{
			m_dLeftAnimRation = 0.0;
			m_bLeftPosSaveCheck = false;
			m_bChangeCheck = true;
		}
	}
	if (true == m_bChangeCheck && false == m_bLeftPosEndCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);



		m_dLeftAnimRation += TimeDelta;

		if (false == m_bLeftPosSaveCheck)
		{
			XMStoreFloat4(&m_vLeftPos, m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
			_float3 Right;
			XMStoreFloat3(&Right, m_pLeftHandTrasform->Get_State(CTransform::STATE_RIGHT));
			XMVector3Normalize(XMLoadFloat3(&Right));
			Right.y = 0;
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) -= (XMLoadFloat3(&Right) * 400));
			m_bLeftPosSaveCheck = true;
		}

		_vector vPos = XMVectorLerp(m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&m_vLeftPos), 0.08);
		MODELDESC_CLIENT tModelDesc;
		ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));
		if (0.08 <= m_dLeftAnimRation && false == m_LeftEffectCheckOne)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->PlaySound(L"BossSwing", CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->Sound_Value(CSound_Manager::ID_BOSS_ATTCK, L"BossSwing", 1.f);
			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 14.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;
			m_LeftEffectCheckOne = true;
		}
		if (0.1 <= m_dLeftAnimRation && false == m_LeftEffectCheckTwo)
		{

			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 13.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;
			m_LeftEffectCheckTwo = true;
		}
		if (0.2 <= m_dLeftAnimRation && false == m_LeftEffectCheckThree)
		{
			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 12.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_LeftEffectCheckThree = true;
		}
		if (0.3 <= m_dLeftAnimRation && false == m_LeftEffectCheckFor)
		{
			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 11.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_LeftEffectCheckFor = true;
		}
		if (0.5 <= m_dLeftAnimRation && false == m_LeftEffectCheckFive)
		{
			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 10.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_LeftEffectCheckFive = true;
		}
		if (0.7 <= m_dLeftAnimRation && false == m_LeftEffectCheckSix)
		{
			tModelDesc.vPos = m_vLeftBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pLeftHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 9.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_LeftEffectCheckSix = true;
		}
		m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);
		if (2.0 <= m_dLeftAnimRation)
		{
			m_pLeftHandTrasform->YawRollPitch_Set(_float3(0.f, 0.f, 0.f));
			m_bLeftPosSaveCheck = false;
			m_bLeftPosEndCheck = true;
			m_dLeftAnimRation = 0.0;
		}
		Safe_Release(pGameInstance);
	}

	return S_OK;

}

HRESULT CBoss_SwingAttackState::Right_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(3.f, 0.f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector RightArmPos = m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, RightArmPos, 0.1);
	m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pRightHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_SwingAttackState::Second_Left_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(-3.f, 0.f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, LeftArmPos, 0.1);
	m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pLeftHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_SwingAttackState::Second_Right_HandAnim(_double TimeDelta)
{
	if (false == m_bChangeCheck)
	{
		m_dLeftAnimRation += TimeDelta * 0.8;
		if (false == m_bLeftPosSaveCheck)
		{
			XMStoreFloat4(&m_vLeftPos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
			_vector Dir = m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK);
			_vector Right = m_pRightHandTrasform->Get_State(CTransform::STATE_RIGHT);
			XMVector3Normalize(Dir);
			XMVector3Normalize(Right);
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) += (Dir * 250));
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) += (Right * 100));
			m_vLeftPos.y = m_vLeftPos.y - 12.f;

			m_bLeftPosSaveCheck = true;
		}

		_double RotX = 0.0 * (1.0 - m_dLeftAnimRation) + 45.0 * m_dLeftAnimRation;
		_double RotZ = 0.0 * (1.0 - m_dLeftAnimRation) + 90.0 * m_dLeftAnimRation;

		_vector vPos = XMVectorLerp(m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&m_vLeftPos), m_dLeftAnimRation);
		//m_pLeftHandTrasform->YawRollPitch_Set(_float3(0.f,0.f , -RotZ));
		m_pRightHandTrasform->Turn(m_pRightHandTrasform->Get_State(CTransform::STATE_RIGHT), TimeDelta * 0.5);
		m_pRightHandTrasform->Turn(m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK), TimeDelta * 0.5);
		m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);
		if (1.0 <= m_dLeftAnimRation)
		{
			m_dLeftAnimRation = 0.0;
			m_bLeftPosSaveCheck = false;
			m_bChangeCheck = true;
		}
	}
	if (true == m_bChangeCheck && false == m_bLeftPosEndCheck)
	{
		m_dLeftAnimRation += TimeDelta;

		if (false == m_bLeftPosSaveCheck)
		{
			XMStoreFloat4(&m_vLeftPos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
			_float3 Right;
			XMStoreFloat3(&Right, m_pRightHandTrasform->Get_State(CTransform::STATE_RIGHT));
			XMVector3Normalize(XMLoadFloat3(&Right));
			Right.y = 0;
			XMStoreFloat4(&m_vLeftPos, XMLoadFloat4(&m_vLeftPos) += (XMLoadFloat3(&Right) * 400));
			m_bLeftPosSaveCheck = true;
		}
		_vector vPos = XMVectorLerp(m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&m_vLeftPos), 0.08);
		m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		MODELDESC_CLIENT tModelDesc;
		ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));
		if (0.08 <= m_dLeftAnimRation && false == m_RightEffectCheckOne)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->PlaySound(L"BossSwing", CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->Sound_Value(CSound_Manager::ID_BOSS_ATTCK, L"BossSwing", 1.f);

			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 10.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 14.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;
			m_RightEffectCheckOne = true;
		}
		if (0.1 <= m_dLeftAnimRation && false == m_RightEffectCheckTwo)
		{
			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 9.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 13.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;
			m_RightEffectCheckTwo = true;
		}
		if (0.2 <= m_dLeftAnimRation && false == m_RightEffectCheckThree)
		{
			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 8.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 12.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;
			m_RightEffectCheckThree = true;
		}
		if (0.3 <= m_dLeftAnimRation && false == m_RightEffectCheckFor)
		{
			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 7.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 11.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_RightEffectCheckFor = true;
		}
		if (0.5 <= m_dLeftAnimRation && false == m_RightEffectCheckFive)
		{
			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 6.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 10.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_RightEffectCheckFive = true;
		}
		if (0.7 <= m_dLeftAnimRation && false == m_RightEffectCheckSix)
		{
			tModelDesc.vPos = m_vRightBonePos;
			tModelDesc.vPos.y -= 5.f;
			XMStoreFloat3(&tModelDesc.vRotaition, m_pRightHandTrasform->Get_State(CTransform::STATE_LOOK));
			tModelDesc.vRotaition.y = 0;
			tModelDesc.vScaile = 9.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Funch"), m_iMyLevel, L"Clone_TestMesh", &tModelDesc)))
				return E_FAIL;

			m_RightEffectCheckSix  = true;
		}

		if (2.0 <= m_dLeftAnimRation)
		{
			m_pRightHandTrasform->YawRollPitch_Set(_float3(0.f, 0.f, 0.f));
			m_bLeftPosSaveCheck = false;
			m_bLeftPosEndCheck = true;
			m_dLeftAnimRation = 0.0;
		}
		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CBoss_SwingAttackState::SetUp_TargetBoneMatrices(const char * pBoneName)
{
	m_LeftOffsetMatrix = m_pLeftHandModel->Get_OffsetMatrix(pBoneName);
	m_LeftPivotMatrix = m_pLeftHandModel->Get_PivotMatrix();
	m_pLeftCombinedMatrix = m_pLeftHandModel->Get_CombinedMatrixPtr(pBoneName);
	m_pLeftTargetWorldMatrix = m_pLeftHandTrasform->Get_WorldMatrixPtr();

	_matrix LeftMatrix;
	LeftMatrix = XMMatrixIdentity();
	LeftMatrix.r[3] = XMVectorSet(0.5f, 0.7f, -2.4, 1.f);
	XMStoreFloat4x4(&m_LeftMatWorld, LeftMatrix);

	m_RightOffsetMatrix = m_pRightHandModel->Get_OffsetMatrix(pBoneName);
	m_RightPivotMatrix = m_pRightHandModel->Get_PivotMatrix();
	m_pRightCombinedMatrix = m_pRightHandModel->Get_CombinedMatrixPtr(pBoneName);
	m_pRightTargetWorldMatrix = m_pRightHandTrasform->Get_WorldMatrixPtr();

	_matrix RightMatrix;
	RightMatrix = XMMatrixIdentity();
	RightMatrix.r[3] = XMVectorSet(0.39f, 0.35f, -1.f, 1.f);
	XMStoreFloat4x4(&m_RightMatWorld, RightMatrix);


	return S_OK;
}

HRESULT CBoss_SwingAttackState::Set_MatrixBone()
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

void CBoss_SwingAttackState::Free()
{
}
