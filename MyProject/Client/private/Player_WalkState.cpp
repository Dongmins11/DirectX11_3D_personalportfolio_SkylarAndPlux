#include "stdafx.h"
#include "..\public\Player_WalkState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_AttackState.h"
#include "Player_JumpState.h"
#include "Player_LandingState.h"

IMPLEMENT_SINGLETON(CPlayer_WalkState)
CPlayer_WalkState::CPlayer_WalkState()
{
}

void CPlayer_WalkState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if ('W' == pPlayerAnimDesc.iKey[i] && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			'S' == pPlayerAnimDesc.iKey[i] && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			'A' == pPlayerAnimDesc.iKey[i] && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			'D' == pPlayerAnimDesc.iKey[i] && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING)
		{
			pPlayerAnimState->Set_MoveCheck(true);
			if (LEVEL_BOSS_STAGE != m_iLevelIndex)
			{
				pPlayerAnimState->Set_MoveValue(1.2);
			}
			else
			{
				pPlayerAnimState->Set_MoveValue(2.2);
			}
			if(FALLING != m_iCurMotion)
				m_iCurMotion = WALK_ANIM;
		}
		else if (VK_LBUTTON == pPlayerAnimDesc.iKey[i])
		{
			m_iCurMotion = WALK_ANIM;
			pPlayerAnimState->Set_MoveCheck(false);
			m_bIsFallingCheck = false;
			pPlayerAnimState->SetUp_State(CPlayer_AttackState::Get_Instance());
			CPlayer_AttackState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
		else if (VK_SPACE == pPlayerAnimDesc.iKey[i])
		{
			m_iCurMotion = WALK_ANIM;
			pPlayerAnimState->Set_MoveCheck(false);
			m_bIsFallingCheck = false;
			pPlayerAnimState->SetUp_State(CPlayer_JumpState::Get_Instance());
			CPlayer_JumpState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			break;
		}
	}
		if (0 == pPlayerAnimDesc.iKeyCount)
		{
			m_iCurMotion = WALK_ANIM;
			pPlayerAnimState->Set_MoveCheck(false);
			m_bIsFallingCheck = false;
			pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		}


		if (true == m_bIsFallingCheck && false == pPlayerAnimState->Get_Gravity())
		{
			m_iCurMotion = WALK_ANIM;
			m_bIsFallingCheck = false;
			pPlayerAnimState->Set_MoveCheck(false);
			pPlayerAnimState->Set_FlyGravity(false);
			pPlayerAnimState->SetUp_State(CPlayer_LandingState::Get_Instance());
			CPlayer_LandingState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			return;
		}
}

void CPlayer_WalkState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	if (true == pPlayerAnimState->Get_Gravity() && 1.f <= pPlayerAnimState->Get_GravirySpeed() && false == m_bIsFallingCheck)
	{
		m_iCurMotion = FALLING;
		m_bIsFallingCheck = true;
	}
	if (FALLING != m_iCurMotion)
	{
		m_dWalkTimeAcc += TimeDelta;

		if (LEVEL_BOSS_STAGE != m_iLevelIndex)
		{
			if (0.7 <= m_dWalkTimeAcc)
			{
				m_dWalkTimeAcc = 0.0;
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_WALK);
				CSound_Manager::Get_Instance()->PlaySound(L"StageWalk", CSound_Manager::ID_PLAYER_WALK);
			}
		}
		else
		{
			if (0.5 <= m_dWalkTimeAcc)
			{
				m_dWalkTimeAcc = 0.0;
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_WALK);
				CSound_Manager::Get_Instance()->PlaySound(L"BossWalk", CSound_Manager::ID_PLAYER_WALK);
			}
		}
	}

	m_pModel->Set_Animation(m_iCurMotion);
	
	if(m_iLevelIndex == LEVEL_BOSS_STAGE)
		m_pModel->Update(TimeDelta * 2.0);
	else
		m_pModel->Update(TimeDelta * 1.3);
	
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_WalkState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_dWalkTimeAcc = 0.0;
	m_iCurMotion = WALK_ANIM;
	m_bIsFallingCheck = false;
}

void CPlayer_WalkState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_WalkState::Free()
{
}
