#include "stdafx.h"
#include "..\public\Player_DeadState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_WalkState.h"

IMPLEMENT_SINGLETON(CPlayer_DeadState)
CPlayer_DeadState::CPlayer_DeadState()
{
}

void CPlayer_DeadState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	pPlayerAnimState->Set_MoveCheck(false);
	m_iCurMotion = DEAD_ANIM;

	if (true == m_bStateChange)
	{
		m_bFirstCheck = false;
		m_pPlayer->Hp_Attack(1);
		m_bStateChange = false;
		m_pPlayer->Set_DeadCheck(false);
		m_pSphere->Set_CheckCollision(false);
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_DEAD);
		m_pTransform->Set_State(CTransform::STATE_POSITION, m_pPlayer->Get_SavePosition());
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}

}

void CPlayer_DeadState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	if (false == m_pPlayer->Get_DeadCheck())
	{
		m_pPlayer->Set_DeadCheck(true);
	}
	if (true == m_pPlayer->Get_ShutDownCheck() && false == m_bStateChange)
	{
		m_dTimeAcc += TimeDelta;
		if (1.0 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			m_bStateChange = true;
		}
	}


	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta);
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_DeadState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
}

void CPlayer_DeadState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_DeadState::Free()
{
}
