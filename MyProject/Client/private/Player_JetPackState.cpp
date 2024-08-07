#include "stdafx.h"
#include "..\public\Player_JetPackState.h"
#include "Player_JumpState.h"
#include "Player_IdleState.h"
#include "Player_LandingState.h"
#include "Effect_JetPack.h"
IMPLEMENT_SINGLETON(CPlayer_JetPackState)
CPlayer_JetPackState::CPlayer_JetPackState()
{
}

void CPlayer_JetPackState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	if (false == m_bFirstEffectCheck)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JETPACK);
		CSound_Manager::Get_Instance()->PlayBGM(L"ChargeJumpReady",1.f,CSound_Manager::ID_PLAYER_JETPACK);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iLevelIndex, TEXT("Prototype_GameObject_Effect_JetPack"), m_iLevelIndex, L"Clone_Effect_JetPack",nullptr,(CGameObject**)&m_pEffectObject)))
			return;

		Safe_Release(pGameInstance);
		m_bFirstEffectCheck = true;
	}

	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if (1 > m_iJumpCount)
		{
			if (pPlayerAnimDesc.iKey[i] == VK_LSHIFT && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && JETPACK_READY != m_iCurMotion && JETPACK_WAIT != m_iCurMotion)
			{
				m_iCurMotion = JETPACK_READY;
				break;
			}
			else if (pPlayerAnimDesc.iKey[i] == VK_LSHIFT && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && JETPACK_READY == m_iCurMotion)
			{
				m_iCurMotion = JETPACK_WAIT;
				break;
			}
		}
		if (JETPACK_WAIT == m_iCurMotion)
		{
			if (pPlayerAnimDesc.iKey[i] == VK_SPACE && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JETPACK);
				CSound_Manager::Get_Instance()->PlaySound(L"ChargeJumpStart", CSound_Manager::ID_PLAYER_JETPACK);

				++m_iJumpCount;
				m_iCurMotion = JETPACK_START;
				if (nullptr != m_pEffectObject)
				{
					dynamic_cast<CEffect_JetPack*>(m_pEffectObject)->Set_DeadCheck(true);
					Safe_Release(m_pEffectObject);
				}
				break;
			}
		}
		if (pPlayerAnimDesc.iKey[i] == 'W' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && m_iCurMotion == JETPACK_LOOP ||
			pPlayerAnimDesc.iKey[i] == 'S' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && m_iCurMotion == JETPACK_LOOP ||
			pPlayerAnimDesc.iKey[i] == 'A' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && m_iCurMotion == JETPACK_LOOP ||
			pPlayerAnimDesc.iKey[i] == 'D' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING && m_iCurMotion == JETPACK_LOOP)
		{
			pPlayerAnimState->Set_MoveCheck(true);//여기 아까와도같은녀석이네..... 이것만고치면끝 고치고 히트시간대를보자
			break;
		}
	}


	if (false == pPlayerAnimState->Get_Gravity() && m_iCurMotion == FALLING)
	{
		m_bFirstEffectCheck = false;
		m_bStartAnim = false;
		m_bLoopAnim = false;
		m_iJumpCount = 0;
		pPlayerAnimState->Set_FlyGravity(false);
		pPlayerAnimState->SetUp_State(CPlayer_LandingState::Get_Instance());
		CPlayer_LandingState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}



}

void CPlayer_JetPackState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{

	if (false == pPlayerAnimState->Get_JumpCheck() && true == pPlayerAnimState->Get_Gravity() && m_iCurMotion == JETPACK_LOOP)
	{
		m_iCurMotion = FALLING;
	}

	m_pModel->Set_Animation(m_iCurMotion);

	if (JETPACK_START == m_iCurMotion)
	{
		m_pModel->Update(TimeDelta * 0.5, &m_bStartAnim, false);
		m_StartTimeAcc += TimeDelta;
		if (0.02 <= m_StartTimeAcc)
		{
			pPlayerAnimState->Set_JumpState(true);
			pPlayerAnimState->Set_JumpPower(20.f);
			pPlayerAnimState->Set_JumpTimeAcc(0.f);
			pPlayerAnimState->Set_JumpValue(0.f);
			pPlayerAnimState->Set_FlyTimeAcc(0.f);
			pPlayerAnimState->Set_MyBackPos(m_pTransform->Get_State(CTransform::STATE_POSITION));
			m_StartTimeAcc = 0.0;
			m_bStartAnim = false;
			m_iCurMotion = JETPACK_LOOP;
		}
	}
	if (JETPACK_LOOP == m_iCurMotion)
		m_pModel->Update(TimeDelta, &m_bLoopAnim, false);
	else
		m_pModel->Update(TimeDelta, &m_bWaitAnim, false);

	m_pModel->Set_PreAnimation(m_iCurMotion);

}

void CPlayer_JetPackState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_bFirstEffectCheck = false;
	m_bWaitAnim = false;
	m_bStartAnim = false;
	m_bLoopAnim = false;
	m_iJumpCount = 0;
	m_StartTimeAcc = 0.0;
	pPlayerAnimState->Set_FlyGravity(false);
	pPlayerAnimState->Set_JumpTimeAcc(0.f);
	pPlayerAnimState->Set_JumpValue(0.f);
	pPlayerAnimState->Set_FlyTimeAcc(0.f);
}

void CPlayer_JetPackState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_JetPackState::Free()
{
}


