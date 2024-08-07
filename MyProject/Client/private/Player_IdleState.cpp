#include "stdafx.h"
#include "..\public\Player_IdleState.h"
#include "Player_WalkState.h"
#include "Player_JumpState.h"
#include "Player_AttackState.h"
#include "Player_JetPackState.h"
#include "Player_LandingState.h"

IMPLEMENT_SINGLETON(CPlayer_IdleState)
CPlayer_IdleState::CPlayer_IdleState()
{
}

void CPlayer_IdleState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if ('W' == pPlayerAnimDesc.iKey[i] ||
			'S' == pPlayerAnimDesc.iKey[i] ||
			'A' == pPlayerAnimDesc.iKey[i] ||
			'D' == pPlayerAnimDesc.iKey[i])
		{
			pPlayerAnimState->SetUp_State(CPlayer_WalkState::Get_Instance());
			CPlayer_WalkState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
		if (VK_SPACE == pPlayerAnimDesc.iKey[i])
		{
			pPlayerAnimState->SetUp_State(CPlayer_JumpState::Get_Instance());
			CPlayer_JumpState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
		if (VK_LBUTTON == pPlayerAnimDesc.iKey[i])
		{
			pPlayerAnimState->SetUp_State(CPlayer_AttackState::Get_Instance());
			CPlayer_AttackState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
		if (VK_RBUTTON == pPlayerAnimDesc.iKey[i])
		{
			pPlayerAnimState->SetUp_State(CPlayer_AttackState::Get_Instance());
			CPlayer_AttackState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
		if (VK_LSHIFT == pPlayerAnimDesc.iKey[i])
		{
			pPlayerAnimState->SetUp_State(CPlayer_JetPackState::Get_Instance());
			CPlayer_JetPackState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
	}

	pPlayerAnimState->Set_MageneTicCheck(false);

	if (FALLING == m_iCurMotion && false == pPlayerAnimState->Get_Gravity())
	{
		m_iCurMotion = IDLE_ANIM;
		pPlayerAnimState->Set_MoveCheck(false);
		pPlayerAnimState->Set_FlyGravity(false);
		pPlayerAnimState->SetUp_State(CPlayer_LandingState::Get_Instance());
		CPlayer_LandingState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}


}

void CPlayer_IdleState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	if (true == pPlayerAnimState->Get_Gravity() && 1.f <= pPlayerAnimState->Get_GravirySpeed())
	{
		m_iCurMotion = FALLING;
	}
	else
		m_iCurMotion = IDLE_ANIM;

	m_TimeAcc += TimeDelta;

	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta);
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_IdleState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	pPlayerAnimState->Set_MageneTicCheck(false);
}

void CPlayer_IdleState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pSphere->Set_CheckCollision(true);
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_IdleState::Free()
{

}