#include "stdafx.h"
#include "..\public\Boss_HitState.h"
#include "Boss_BackUpState.h"
#include "Boss_IdleState.h"
#include "Client_AnimationDefine.h"
#include "Rock.h"
#include "Camera_Fly.h"

IMPLEMENT_SINGLETON(CBoss_HitState)
CBoss_HitState::CBoss_HitState()
	:m_dBlendTime(5.f)
	,m_iHitIndex(0)
	,m_iCurMotion(1)
	,m_vStartValue(9.f)
	,m_vBlendValue(9.f)
	,m_vEndValue(10.f)
{
}

void CBoss_HitState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	m_iLeftCurMotion = BOSS_IDLE_MOTION;
	m_iRightCurMotion = BOSS_IDLE_MOTION;

	if (false == m_bFristCheck)
	{
		m_pBoss->Set_Effect_RenderCheck(true);
		XMStoreFloat4(&m_vBackUpPos, m_pHeadTrasform->Get_State(CTransform::STATE_POSITION));
		m_fMyPosY = m_vBackUpPos.y;
		m_pBoss->Set_HitCheck(true);
		m_bFristCheck = true;
	}

	if (true == m_bNextAnim)
	{
		m_bJump = false;
		m_bLerpCheck = false;
		m_bNextAnim = false;
		m_bFristCheck = false;

		if (FAILED(Cam_Out()))
			return;
		pBossAnimState->Set_BossHItCheck(false);
		pBossAnimState->SetUp_State(CBoss_BackUpState::Get_Instance());
		CBoss_BackUpState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
		return;
	}


}

void CBoss_HitState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{
	m_pBoss->Set_FaceChange(true);



	if (FAILED(Cam_In(TimeDelta)))
		return;


	if (FAILED(Left_HandAnim(TimeDelta)))
		return;

	if (FAILED(Head_Animation(TimeDelta)))
		return;

	if (FAILED(Gravity(TimeDelta)))
		return;

	if (FAILED(Head_Hit(TimeDelta)))
		return;

	m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
	m_pLeftHandModel->Update(TimeDelta, nullptr, false);
	m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

	m_pRightHandModel->Set_Animation(m_iRightCurMotion);
	m_pRightHandModel->Update(TimeDelta, nullptr, false);
	m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
}

void CBoss_HitState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
}

void CBoss_HitState::Set_Collision(CBoss_AnimState * pBossAnimState)
{
}

HRESULT CBoss_HitState::Left_HandAnim(_double TimeDelta)
{
	if (false == m_bLerpCheck)
	{
		m_dAnimRation += TimeDelta;
		_float4 vLeftDestPos;
		XMStoreFloat4(&vLeftDestPos, m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
		vLeftDestPos.y += 30;

		_float4 vRightDestPos;
		XMStoreFloat4(&vRightDestPos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
		vRightDestPos.y += 30;

		_vector LerpLeftPosition = XMVectorLerp(m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&vLeftDestPos), m_dAnimRation);
		_vector LerpRightPosition = XMVectorLerp(m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&vRightDestPos), m_dAnimRation);

		m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, LerpLeftPosition);
		m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, LerpRightPosition);

		if (1.0 <= m_dAnimRation)
		{
			m_bLerpCheck = true;
			m_dAnimRation = 0.0;
		}
	}


	return S_OK;
}

HRESULT CBoss_HitState::Right_HandAnim(_double TimeDelta)
{
	return S_OK;
}

HRESULT CBoss_HitState::Head_Animation(_double TimeDelta)
{
	if (false == m_bJump)
	{
		m_dJumpTimeAcc += TimeDelta * 3;

		m_pHeadTrasform->Set_PosJump(m_fMyPosY + (10 * m_dJumpTimeAcc - 0.5f * 9.8f * m_dJumpTimeAcc *m_dJumpTimeAcc));
		m_pHeadTrasform->Go_Backward(-TimeDelta * 4);

		m_pHeadTrasform->Turn(m_pHeadTrasform->Get_State(CTransform::STATE_LOOK), TimeDelta * 2.5);
		_float FlyValue = (10 * m_dJumpTimeAcc - 0.5f *  9.8f * m_dJumpTimeAcc * m_dJumpTimeAcc);

		if (FlyValue < m_fBackUp_BossY)
		{
			m_fBackUp_BossY = 0.f;
			m_fMyPosY = 0.f;
			m_dJumpTimeAcc = 0.0;
			m_bJump = true;
			m_bGravityCheck = true;
		}
		else
		{
			m_fBackUp_BossY = FlyValue;
		}
	}
	return S_OK;
}

HRESULT CBoss_HitState::Head_Hit(_double TimeDelta)
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

	if (false == m_pHeadCollider->Get_CheckCollision() && pTargetSphere->Collision_SphereAABB(m_pHeadCollider))
	{
		if (false == m_bCheckCollision)
		{
			m_iScaleIndex = 0;
			m_dBlendTime = 5.f;
			m_vBlendValue = 9.f;
			m_vStartValue = 9.f;
			m_vEndValue = 10.f;
			m_iScaleIndex = 0;
			m_vTimeAcc = 0.0;
			m_pHeadCollider->Set_CheckCollision(true);
			m_pBoss->Hp_Down(8);
			m_bCheckCollision = true;
			m_bMotionCheck = true;
		}
		++m_iHitIndex;
	}


	if (true == m_bCheckCollision)
	{
		m_dCollisionTimeAcc += TimeDelta;

		if (FAILED(Hit_Motion(TimeDelta)))
			return E_FAIL;
		if (ATTACKMOTION == dynamic_cast<CRock*>(pPlayerObject)->Get_CurrentAnimation())
		{
			if (0.5f <= m_dCollisionTimeAcc)
			{
				m_dCollisionTimeAcc = 0.0;
				m_pHeadCollider->Set_CheckCollision(false);
				m_bCheckCollision = false;
			}
		}
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBoss_HitState::Hit_Motion(_double TimeDelta)
{
	if (true == m_bMotionCheck)
	{
		m_vTimeAcc += TimeDelta * m_dBlendTime;

		_double dSize = m_vStartValue * (1.0 - m_vTimeAcc) + m_vEndValue * m_vTimeAcc;
		//m_pTransformCom->Set_ScailingXYZ(_float3((_float)dSize, 1.f, (_float)dSize));
		m_pHeadTrasform->Set_Scailing((_float)dSize);
		if (1.0 <= m_vTimeAcc)
		{
			_uint iScaleIndex = m_iScaleIndex % 2;


			if (0 == iScaleIndex)
			{
				m_vStartValue = 10.f;
				m_vEndValue = m_vBlendValue;

			}
			else
			{
				m_vStartValue = m_vEndValue;
				m_vEndValue = 10.f;
			}

			m_vBlendValue += 0.48f;
			m_dBlendTime += 6.f;
			m_vTimeAcc = 0.0;
			++m_iScaleIndex;


			if (m_iScaleIndex == 5)
			{
				m_dBlendTime = 5.f;
				m_vBlendValue = 9.f;
				m_vStartValue = 9.f;
				m_vEndValue = 10.f;
				m_iScaleIndex = 0;
				m_bMotionCheck = false;
			}
		}
	}

	return S_OK;
}

HRESULT CBoss_HitState::Cam_In(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
	if (nullptr == pCam)
	{
		MSGBOX("Failed to Finder : Boss Hit Cam in : pCam");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	dynamic_cast<CCamera_Fly*>(pCam)->Set_DistanceCheck(true);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBoss_HitState::Cam_Out()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
	if (nullptr == pCam)
	{
		MSGBOX("Failed to Finder : Boss Hit Cam in : pCam");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	dynamic_cast<CCamera_Fly*>(pCam)->Set_DistanceCheck(false);


	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CBoss_HitState::Gravity(_double TimeDelta)
{

	if (true == m_bGravityCheck)
	{
		m_iCount +=2;
		m_fCollSpeed += TimeDelta * 9.8f * 1.f * m_iCount;
		m_dMaxCallSpeed = 40.f;
		if (m_dMaxCallSpeed <= m_fCollSpeed)
			m_fCollSpeed = m_dMaxCallSpeed;

		m_pHeadTrasform->Set_PosGravity(-m_fCollSpeed * TimeDelta);
		m_pHeadTrasform->Go_Backward(-TimeDelta * 4);
		_float4 vHeadPos;
		XMStoreFloat4(&vHeadPos, m_pHeadTrasform->Get_State(CTransform::STATE_POSITION));
		if (12.f >= vHeadPos.y)
		{
			m_fCollSpeed = 0;
			m_iCount = 0;
			m_bGravityCheck = false;
		}
	}
	if (true == m_bJump && false == m_bGravityCheck && false == m_bNextAnim)
	{
		m_dNextMotionCheckTime += TimeDelta;
		if(7.0 <= m_dNextMotionCheckTime)
		{
			m_dNextMotionCheckTime = 0.0;
			m_bNextAnim = true;
		}
	}
	return S_OK;
}

void CBoss_HitState::Free()
{
}
