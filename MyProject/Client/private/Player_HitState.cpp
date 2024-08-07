#include "stdafx.h"
#include "..\public\Player_HitState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_WalkState.h"


IMPLEMENT_SINGLETON(CPlayer_HitState)
CPlayer_HitState::CPlayer_HitState()
{
}

void CPlayer_HitState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	/*if (0 >= m_pPlayer->Get_Hp())
	{
		pPlayerAnimState->SetUp_State(CPlayer_DeadState::Get_Instance());
		CPlayer_DeadState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}*/
	if (true == m_bAnimCheck)
	{
		m_pPlayer->Hp_Attack(1);
		m_bAnimCheck = false;
		pPlayerAnimState->Set_DownAttackGravity(false);
		pPlayerAnimState->Set_DownAttack(false);
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}
	else
	{
		pPlayerAnimState->Set_MoveCheck(false);
		m_iCurMotion = HIT_ANIM;
	}

	pPlayerAnimState->Set_GravityPower(1.2f);
	pPlayerAnimState->Set_DownAttack(true);
	pPlayerAnimState->Set_DownAttackGravity(true);

}

void CPlayer_HitState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta* 1.5,&m_bAnimCheck);
	m_pModel->Set_PreAnimation(m_iCurMotion);


}

void CPlayer_HitState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_bAnimCheck = false;
}

void CPlayer_HitState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_HitState::Free()
{
}
