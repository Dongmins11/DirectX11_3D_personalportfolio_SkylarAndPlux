#include "stdafx.h"
#include "..\public\Player_FallingState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_LandingState.h"

IMPLEMENT_SINGLETON(CPlayer_FallingState)
CPlayer_FallingState::CPlayer_FallingState()
{
}

void CPlayer_FallingState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{

	if (false == pPlayerAnimState->Get_Gravity() && false == pPlayerAnimState->Get_JumpState())
	{
		pPlayerAnimState->SetUp_State(CPlayer_LandingState::Get_Instance());
		CPlayer_LandingState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
	}

	m_iCurMotion = FALLING;

}

void CPlayer_FallingState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta);
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_FallingState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
}

void CPlayer_FallingState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_FallingState::Free()
{
}
