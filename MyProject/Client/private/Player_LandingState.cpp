#include "stdafx.h"
#include "..\public\Player_LandingState.h"
#include "GameInstance.h"
#include "Player_IdleState.h"
#include "Player_WalkState.h"
#include "Player_FallingState.h"


IMPLEMENT_SINGLETON(CPlayer_LandingState)
CPlayer_LandingState::CPlayer_LandingState()
{
}



void CPlayer_LandingState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	if (false == m_bFirstCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CVIBuffer_PointInstance::POINTINST tPointInst;
		ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
		tPointInst.m_iType = 6;
		tPointInst.m_iSize = 5;
		tPointInst.m_iTextureIndex = 2;
		tPointInst.m_iPassIndex = 4;

		if (LEVEL_BOSS_STAGE != m_iLevelIndex)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_WALK);
			CSound_Manager::Get_Instance()->PlaySound(L"Landing", CSound_Manager::ID_PLAYER_WALK);
		}
		else
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_WALK);
			CSound_Manager::Get_Instance()->PlaySound(L"BossWalk", CSound_Manager::ID_PLAYER_WALK);
		}
		XMStoreFloat4(&tPointInst.m_vPosition, m_pTransform->Get_State(CTransform::STATE_POSITION));
		tPointInst.m_vPosition.y += 1.f;
		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iLevelIndex, TEXT("Prototype_GameObject_Effect_Point"), m_iLevelIndex, L"Clone_PetPointEffect", &tPointInst)))
			return;

		Safe_Release(pGameInstance);

		m_bFirstCheck = true;
	}
	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if (pPlayerAnimDesc.iKey[i] == 'W' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			pPlayerAnimDesc.iKey[i] == 'S' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			pPlayerAnimDesc.iKey[i] == 'A' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING ||
			pPlayerAnimDesc.iKey[i] == 'D' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING)
		{
			pPlayerAnimState->Set_MoveCheck(true);
			pPlayerAnimState->Set_MoveValue(1.0);
			m_iCurMotion = MOVE_LANDING;
		}
	}

	if (pPlayerAnimDesc.iKeyCount == 0 && m_iCurMotion != MOVE_LANDING)
	{
		pPlayerAnimState->Set_MoveCheck(false);
		m_iCurMotion = STOP_LANDING;
	}


	if (m_iCurMotion == MOVE_LANDING && m_bLandingCheck == true && pPlayerAnimDesc.iKeyCount != 0)
	{
		m_bFirstCheck = false;
		m_bLandingCheck = false;
		m_iCurMotion = STOP_LANDING;
		pPlayerAnimState->SetUp_State(CPlayer_WalkState::Get_Instance());
		CPlayer_WalkState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}
	else if (m_iCurMotion == STOP_LANDING && m_bLandingCheck == true)
	{
		m_bFirstCheck = false;
		m_bLandingCheck = false;
		m_iCurMotion = STOP_LANDING;
		pPlayerAnimState->Set_MoveCheck(false);
 		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}
	else if (m_iCurMotion == MOVE_LANDING && m_bLandingCheck == true && pPlayerAnimDesc.iKeyCount == 0)
	{
		m_bFirstCheck = false;
		m_bLandingCheck = false;
		m_iCurMotion = STOP_LANDING;
		pPlayerAnimState->Set_MoveCheck(false);
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}

}

void CPlayer_LandingState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	m_pModel->Set_Animation(m_iCurMotion);

	if (m_iCurMotion == MOVE_LANDING)
		m_pModel->Update(TimeDelta * 1.2, &m_bLandingCheck, true,24.0);
	if (m_iCurMotion == STOP_LANDING)
		m_pModel->Update(TimeDelta * 1.7, &m_bLandingCheck, true);

	m_pModel->Set_PreAnimation(m_iCurMotion);

}

void CPlayer_LandingState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	pPlayerAnimState->Set_MoveCheck(false);
	m_bLandingCheck = false;
	m_bFirstCheck = false;
}

void CPlayer_LandingState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_LandingState::Free()
{
}
