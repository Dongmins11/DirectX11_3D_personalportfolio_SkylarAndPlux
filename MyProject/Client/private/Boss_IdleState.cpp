#include "stdafx.h"
#include "..\public\Boss_IdleState.h"
#include "GameInstance.h"
#include "Boss_SwingAttackState.h"
#include "Boss_GunAttackState.h"
#include "Boss_TackDownState.h"
#include "Boss_GunAttackState.h"
#include "Boss_HitState.h"

IMPLEMENT_SINGLETON(CBoss_IdleState)
CBoss_IdleState::CBoss_IdleState()
	:m_iRandomIndex(0)
{
}

void CBoss_IdleState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	m_iLeftCurMotion = BOSS_IDLE_MOTION;
	m_iRightCurMotion = BOSS_IDLE_MOTION;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down(VK_NUMPAD1))
		m_iRandomIndex = 1;
	else if (pGameInstance->Key_Down(VK_NUMPAD2))
		m_iRandomIndex = 2;
	else if (pGameInstance->Key_Down(VK_NUMPAD3))
		m_iRandomIndex = 3;
	else if (pGameInstance->Key_Down(VK_NUMPAD4))
		m_iRandomIndex = 4;

	Safe_Release(pGameInstance);

	if (true == m_bAnimChangeCheck)
	{
		switch (m_iRandomIndex)
		{
		case 1:
			m_bAnimChangeCheck = false;
			pBossAnimState->SetUp_State(CBoss_SwingAttackState::Get_Instance());
			CBoss_SwingAttackState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
			break;
		case 2:
			m_bAnimChangeCheck = false;
			pBossAnimState->SetUp_State(CBoss_TackDownState::Get_Instance());
			CBoss_TackDownState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
			break;
		case 3:
			m_bAnimChangeCheck = false;
			m_iRandomIndex = 0;
			pBossAnimState->SetUp_State(CBoss_GunAttackState::Get_Instance());
			CBoss_GunAttackState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
			break;
		case 4:
			m_pBoss->Set_HitCheck(true);
			pBossAnimState->SetUp_State(CBoss_HitState::Get_Instance());
			CBoss_HitState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
			break;
		}
		return;
	}
}

void CBoss_IdleState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{
	m_pBoss->Set_FaceChange(false);

	m_dRandomTime += TimeDelta;
	if (4.0 <= m_dRandomTime)
	{
		m_bAnimChangeCheck = true;
		m_dRandomTime = 0.0;
		++m_iRandomIndex;
		//m_iRandomIndex = 3;
		//m_iRandomIndex = rand() % 3 +1;
	}

	if (FAILED(Left_HandAnim(TimeDelta)))
		return;
	if (FAILED(Right_HandAnim(TimeDelta)))
		return;

	m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
	m_pLeftHandModel->Update(TimeDelta);
	m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

	m_pRightHandModel->Set_Animation(m_iRightCurMotion);
	m_pRightHandModel->Update(TimeDelta);
	m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
}

void CBoss_IdleState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
}

void CBoss_IdleState::Set_Collision(CBoss_AnimState * pBossAnimState)
{
}

HRESULT CBoss_IdleState::Left_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance,XMVectorSet(-3.f, -0.5f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(LeftArmPos, CrtPos, 0.05);
	m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pLeftHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_IdleState::Right_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(3.f, -0.5f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector RightArmPos = m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(RightArmPos, CrtPos, 0.05);
	m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pRightHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

void CBoss_IdleState::Free()
{
}
