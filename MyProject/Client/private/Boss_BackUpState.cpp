#include "stdafx.h"
#include "..\public\Boss_BackUpState.h"
#include "GameINstance.h"
#include "Boss_IdleState.h"


IMPLEMENT_SINGLETON(CBoss_BackUpState)
CBoss_BackUpState::CBoss_BackUpState()
{
}

void CBoss_BackUpState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	m_iLeftCurMotion = BOSS_IDLE_MOTION;
	m_iRightCurMotion = BOSS_IDLE_MOTION;

	if (true == m_bEndFSM)
	{
		m_pBoss->Set_Effect_RenderCheck(false);
		m_bLerpCheck = false;
		m_bEndFSM = false;
		m_pBoss->Set_HitCheck(false);
		pBossAnimState->SetUp_State(CBoss_IdleState::Get_Instance());
		CBoss_IdleState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
		return;
	}

}

void CBoss_BackUpState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{
	if (FAILED(Head_Animation(TimeDelta)))
		return;

	if (FAILED(Left_HandAnim(TimeDelta)))
		return;

	if (FAILED(Right_HandAnim(TimeDelta)))
		return;

	if (FAILED(Boss_Look(TimeDelta)))
		return;

	m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
	m_pLeftHandModel->Update(TimeDelta, nullptr, false);
	m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

	m_pRightHandModel->Set_Animation(m_iRightCurMotion);
	m_pRightHandModel->Update(TimeDelta, nullptr, false);
	m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);




}

void CBoss_BackUpState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
}

void CBoss_BackUpState::Set_Collision(CBoss_AnimState * pBossAnimState)
{
}

HRESULT CBoss_BackUpState::Left_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(-3.f, -0.5f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(LeftArmPos, CrtPos, 0.1);
	m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pLeftHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_BackUpState::Right_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(3.f, -0.5f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector RightArmPos = m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(RightArmPos, CrtPos, 0.1);
	m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pRightHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_BackUpState::Head_Animation(_double TimeDelta)
{
	if (false == m_bLerpCheck)
	{
		m_dAnimRation += TimeDelta * 2.5;


		_vector vSourcePos = XMVectorSet(131.4f, 30.0f, 143.84f, 1.f);
		_vector vDestPos = m_pHeadTrasform->Get_State(CTransform::STATE_POSITION);
	
		_vector MyPos = XMVectorLerp(vDestPos, vSourcePos, m_dAnimRation);

		m_pHeadTrasform->Set_State(CTransform::STATE_POSITION, MyPos);
		m_pHeadTrasform->Turn(m_pHeadTrasform->Get_State(CTransform::STATE_LOOK), -TimeDelta * 2.7);
		if (1.0 <= m_dAnimRation)
		{
			m_bLerpCheck = true;
			m_dAnimRation = 0.0;
		}
	}



	return S_OK;
}

HRESULT CBoss_BackUpState::Boss_Look(_double TimeDelta)
{
	if (true == m_bLerpCheck)
	{
		m_dAnimRation += TimeDelta;

		CGameInstance * pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);

		_vector BossLook = pTargetTransform->Get_State(CTransform::STATE_POSITION) - m_pHeadTrasform->Get_State(CTransform::STATE_POSITION);
		_float3 vLook;
		XMStoreFloat3(&vLook, BossLook);
		vLook.y = 0.f;
		_vector vHeadLook = XMVectorLerp(m_pHeadTrasform->Get_State(CTransform::STATE_LOOK), -XMLoadFloat3(&vLook), 0.3);

		m_pHeadTrasform->Set_MyLook(vHeadLook);

		Safe_Release(pGameInstance);
		if (1.0 <= m_dAnimRation)
		{
			m_dAnimRation = 0.0;
			m_bEndFSM = true;
		}

	}
	return S_OK;
}

void CBoss_BackUpState::Free()
{
}
