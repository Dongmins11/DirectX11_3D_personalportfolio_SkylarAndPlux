#include "stdafx.h"
#include "..\public\Player_DownAttack.h"
#include "Player_IdleState.h"
#include "Player_WalkState.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
IMPLEMENT_SINGLETON(CPlayer_DownAttack)
CPlayer_DownAttack::CPlayer_DownAttack()
{
}

void CPlayer_DownAttack::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	//바닥에닿을때까지 떨어져야함 어택스타트가
	
	//닿으면 루프시작 루프돌때 이동키누르면 이동 movement만들어야함
	
	//이동키안누를시 그자리에서 스탠딩

	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if (false == m_bDownStart && m_iCurMotion != DOWN_ATTACK_LOOP)
		{
			if (pPlayerAnimDesc.iKey[i] == VK_LBUTTON && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
			{
				pPlayerAnimState->Set_MoveCheck(false);
				pPlayerAnimState->Set_GravityPower(3.2f);
				pPlayerAnimState->Set_DownAttack(true);
				pPlayerAnimState->Set_DownAttackGravity(true);
				m_bEndAnim = false;
				m_bLoopEnd = false;
				m_bLandingAnim = false;
				m_iCurMotion = DOWN_ATTACK_START;
				m_bDownStart = true;
				break;
			}
		}
	}

	if (m_iCurMotion == DOWN_ATTACK_LOOP && true == m_bLoopEnd)
	{
		if (pPlayerAnimDesc.iOnceKey == 'W'||
			pPlayerAnimDesc.iOnceKey == 'S'||
			pPlayerAnimDesc.iOnceKey == 'A'||
			pPlayerAnimDesc.iOnceKey == 'D')
		{
			pPlayerAnimState->Set_MoveCheck(true);
			m_bEndAnim = false;
			m_bLoopEnd = false;
			m_bLandingAnim = false;
			m_iCurMotion = DOWN_ATTACK_WALK;
		}
		else
		{
			m_bEndAnim = false;
			m_bLoopEnd = false;
			m_bLandingAnim = false;
			m_iCurMotion = DOWN_ATTACK_LANDING;
		}
	}


	if (m_iCurMotion == DOWN_ATTACK_LANDING && true == m_bLandingAnim)
	{
		m_bDownStart = false;
		m_bEndAnim = false;
		m_bLoopEnd = false;
		m_bLandingAnim = false;
		pPlayerAnimState->Set_DownAttackGravity(false);
		pPlayerAnimState->Set_DownAttack(false);
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
	}
	if (m_iCurMotion == DOWN_ATTACK_WALK && true == m_bLandingAnim)
	{
		m_bDownStart = false;
		m_bEndAnim = false;
		m_bLoopEnd = false;
		m_bLandingAnim = false;
		pPlayerAnimState->Set_DownAttackGravity(false);
		pPlayerAnimState->Set_DownAttack(false);
		pPlayerAnimState->SetUp_State(CPlayer_WalkState::Get_Instance());
		CPlayer_WalkState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
	}
	

}

void CPlayer_DownAttack::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	m_pModel->Set_Animation(m_iCurMotion);

	if (m_iCurMotion == DOWN_ATTACK_START)
	{
		if (false == m_bEndAnim)
			m_pModel->Update(TimeDelta * 3, &m_bEndAnim, true);
		else if (false == pPlayerAnimState->Get_Gravity() || true == pPlayerAnimState->Get_CollisionY())
		{
			pPlayerAnimState->Set_DownAttack(false);
			pPlayerAnimState->Set_DownAttackGravity(false);
			m_iCurMotion = DOWN_ATTACK_LOOP;
		}
	}
	else if (m_iCurMotion == DOWN_ATTACK_LOOP)
	{
		 m_pModel->Update(TimeDelta, &m_bLoopEnd, true,10.0);
		 m_pHandSphere->Set_CheckCollision(false);
	}
	else if (m_iCurMotion == DOWN_ATTACK_LANDING)
		m_pModel->Update(TimeDelta * 2, &m_bLandingAnim, true);
	else if (m_iCurMotion == DOWN_ATTACK_WALK)
		m_pModel->Update(TimeDelta * 1.5, &m_bLandingAnim, true,20.0);

	m_pPlayer->Set_CurrentAnimation(DOWNATTACKMOTION);	//커런트애니메이션 충돌처리를위한녀석 밖에서확인을해서 분기를해야함
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_DownAttack::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_bDownStart = false;
	m_bEndAnim = false;
	m_bLoopEnd = false;
	m_bLandingAnim = false;
	pPlayerAnimState->Set_DownAttackGravity(false);
	pPlayerAnimState->Set_DownAttack(false);
}

void CPlayer_DownAttack::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(false);
}

void CPlayer_DownAttack::Free()
{
}


