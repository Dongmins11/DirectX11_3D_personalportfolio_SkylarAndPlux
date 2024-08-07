#include "stdafx.h"
#include "..\public\Player_AttackState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_WalkState.h"


IMPLEMENT_SINGLETON(CPlayer_AttackState)
CPlayer_AttackState::CPlayer_AttackState()
{
}

void CPlayer_AttackState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	if (pPlayerAnimDesc.iOnceKey == VK_LBUTTON)
	{
		pPlayerAnimState->Set_MoveCheck(false);
		if (false == m_bIsChangeMotion)
		{
			if (0 == m_iAttackCount)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				CSound_Manager::Get_Instance()->PlaySound(L"AttackFast", CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				++m_iAttackCount;
				m_dAttackCollisionCheckTime = 0.0;
				m_iCurMotion = ATTACK_ONE;
			}
			else if (1 == m_iAttackCount)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				CSound_Manager::Get_Instance()->PlaySound(L"AttackFast", CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				++m_iAttackCount;
				m_dAttackCollisionCheckTime = 0.0;
				m_iCurMotion = ATTACK_TWO;
			}
			else if (2 == m_iAttackCount)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				CSound_Manager::Get_Instance()->PlaySound(L"AttackFast", CSound_Manager::ID_PLAYER_ATTACK_PUNCH);
				m_iAttackCount = 0;
				m_dAttackCollisionCheckTime = 0.0;
				m_iCurMotion = ATTACK_THREE;
			}
			m_bIsChangeMotion = true;
		}
	}
	else if (pPlayerAnimDesc.iOnceKey == VK_RBUTTON)
	{
		if (false == m_bIsChangeMotion)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_ATTACK_SWING);
			CSound_Manager::Get_Instance()->PlaySound(L"SwingAttack", CSound_Manager::ID_PLAYER_ATTACK_SWING);

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iLevelIndex, TEXT("Prototype_GameObject_Effect_SwingAttack"), m_iLevelIndex, L"Clone_Effect_SwingAttack")))
				return;
			Safe_Release(pGameInstance);

			m_iCurMotion = ATTACK_SWING;

			m_bIsChangeMotion = true;
		}
	}
	else if ('W' == pPlayerAnimDesc.iOnceKey ||
		'S' == pPlayerAnimDesc.iOnceKey ||
		'A' == pPlayerAnimDesc.iOnceKey ||
		'D' == pPlayerAnimDesc.iOnceKey)
	{
		if (true == m_bIsIdleChange)
		{
			m_dAttackCollisionCheckTime = 0.0;
			pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		}
	}

	if (pPlayerAnimDesc.iKeyCount == 0 && true == m_bIsIdleChange)
	{
		m_dAttackCollisionCheckTime = 0.0;
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
	}

}

void CPlayer_AttackState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{

	m_pModel->Set_Animation(m_iCurMotion);

	if (ATTACK_ONE == m_iCurMotion)
	{
		m_dAttackCollisionCheckTime += TimeDelta;
		
		if (0.3 <= m_dAttackCollisionCheckTime && 0.5 >= m_dAttackCollisionCheckTime)
		{
			m_pHandSphere->Set_CheckCollision(false);
			m_dAttackCollisionCheckTime = 0.0;
		}
		else
		{
			m_pHandSphere->Set_CheckCollision(true);
		}
	}
	if (ATTACK_TWO == m_iCurMotion)
	{
		m_dAttackCollisionCheckTime += TimeDelta;

		if (0.4 <= m_dAttackCollisionCheckTime && 0.6 >= m_dAttackCollisionCheckTime)
		{
			m_pHandSphere->Set_CheckCollision(false);
			m_dAttackCollisionCheckTime = 0.0;
		}
		else
		{
			m_pHandSphere->Set_CheckCollision(true);
		}
	}
	if (ATTACK_THREE == m_iCurMotion)
	{
		m_dAttackCollisionCheckTime += TimeDelta;

		if (0.3 <= m_dAttackCollisionCheckTime && 0.6 >= m_dAttackCollisionCheckTime)
		{
			m_pHandSphere->Set_CheckCollision(false);
			m_dAttackCollisionCheckTime = 0.0;
		}
		else
		{
			m_pHandSphere->Set_CheckCollision(true);
		}
	}
	if (ATTACK_SWING == m_iCurMotion)
	{
		m_pHandSphere->Set_CheckCollision(false);
	}



	if (0.6 > CPlayer_IdleState::Get_Instance()->Get_IdleTime())
	{
		m_bIsOnceChage = true;

		if (m_iCurMotion == ATTACK_SWING)
			m_pModel->Update(TimeDelta * 2, &m_bIsCheck, m_bIsOnceChage);
		else
			m_pModel->Update(TimeDelta, &m_bIsCheck, m_bIsOnceChage);
	}
	else
	{
		m_bIsOnceChage = false;
		m_iAttackCount = 0;
		CPlayer_IdleState::Get_Instance()->Set_IdleTime(0.0);

		if (m_iCurMotion == ATTACK_SWING)
			m_pModel->Update(TimeDelta * 3, &m_bIsCheck, m_bIsOnceChage);
		else
			m_pModel->Update(TimeDelta, &m_bIsCheck, m_bIsOnceChage);

	}

	
	m_pPlayer->Set_CurrentAnimation(ATTACKMOTION);
	m_pModel->Set_PreAnimation(m_iCurMotion);

	if (true == m_bIsCheck)
	{
		m_bIsChangeMotion = false;
		m_bIsCheck = false;
		m_bIsIdleChange = true;
	}
	else
		m_bIsIdleChange = false;

}

void CPlayer_AttackState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_dAttackCollisionCheckTime = 0.0;
}

void CPlayer_AttackState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
}

void CPlayer_AttackState::Free()
{
}


