#include "stdafx.h"
#include "..\public\Player_JumpState.h"
#include "Player_IdleState.h"
#include "Player_FallingState.h"
#include "Player_LandingState.h"
#include "Player_DownAttack.h"
#include "Player_JetPackState.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPlayer_JumpState)

CPlayer_JumpState::CPlayer_JumpState()
	:m_iJumpCount(0)
{
}

void CPlayer_JumpState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	//착지일때시점을알아야한다 ->충돌했을때 bool값을 가져온다 y충돌

	//점프시간대랑 애니메이션시간대를 조절할수있어야한다 ->이건 상수로 나눠봐야할듯 레이티오값을찾자

	//점프중에 애니메이션변경은 내려찍기 이단점프 제트팩 3개로나뉘어진다 ->분기문나눠할듯

	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if (2 > m_iJumpCount)
		{
			if (pPlayerAnimDesc.iKey[i] == VK_SPACE && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
			{
				if (0 == m_iJumpCount)
				{
					if (LEVEL_BOSS_STAGE != m_iLevelIndex)
						pPlayerAnimState->Set_MoveValue(1.2);
					else
						pPlayerAnimState->Set_MoveValue(2.0);

					CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
					CSound_Manager::Get_Instance()->PlaySound(L"Jump", CSound_Manager::ID_PLAYER_JUMP);

					pPlayerAnimState->Set_JumpState(true);
					pPlayerAnimState->Set_JumpPower(12.f);
					pPlayerAnimState->Set_JumpTimeAcc(0.f);
					pPlayerAnimState->Set_JumpValue(0.f);
					pPlayerAnimState->Set_FlyTimeAcc(0.f);
					pPlayerAnimState->Set_MyBackPos(m_pTransform->Get_State(CTransform::STATE_POSITION));
					++m_iJumpCount;
					m_bFallandSky = false;
					m_bJumpCheck = false;
					m_iCurMotion = JUMP;
					break;
				}
				if (1 == m_iJumpCount && DOUBLEJUMP != m_iCurMotion)
				{
					if (LEVEL_BOSS_STAGE != m_iLevelIndex)
						pPlayerAnimState->Set_MoveValue(1.2);
					else
						pPlayerAnimState->Set_MoveValue(2.0);

					CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
					CSound_Manager::Get_Instance()->PlaySound(L"DoubleJump", CSound_Manager::ID_PLAYER_JUMP);

					pPlayerAnimState->Set_JumpState(true);
					pPlayerAnimState->Set_JumpPower(10.f);
					pPlayerAnimState->Set_JumpTimeAcc(0.f);
					pPlayerAnimState->Set_JumpValue(0.f);
					pPlayerAnimState->Set_FlyTimeAcc(0.f);
					pPlayerAnimState->Set_MyBackPos(m_pTransform->Get_State(CTransform::STATE_POSITION));
					++m_iJumpCount;
					m_bFallandSky = false;
					m_bJumpCheck = false;
					m_iCurMotion = DOUBLEJUMP;
					break;
				}

			}
		}
		if (pPlayerAnimDesc.iKey[i] == VK_END && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_PRESING)
		{
			if (0.3f <= pPlayerAnimState->Get_TimeDelta() && true == m_bJumpCheck && true == m_pJetPack_Ui->Get_JetPackPowerCheck())
			{	
				if (false == m_bJetPackSoundCheck)
				{
					CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
					CSound_Manager::Get_Instance()->PlayBGM(L"JetPackLoop", 1.f, CSound_Manager::ID_PLAYER_JUMP);

					m_bJetPackSoundCheck = true;
				}

				m_pJetPack_Ui->Set_JetPackUsing(true);
				pPlayerAnimState->Set_FlyTimeAcc(0.f);
				pPlayerAnimState->Set_CollSpeed(0.f);
				pPlayerAnimState->Set_MoveValue(0.8);
				m_bFallandSky = false;
				m_iCurMotion = JETPACK_SKY;
			}
			if (pPlayerAnimDesc.iKey[i] == VK_LBUTTON && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
			{
				m_fFlyTimeAcc = 0.f;
				m_iJumpCount = 0;
				m_bJetPackSoundCheck = false;
				m_bFallandSky = false;
				m_bJumpCheck = false;
				pPlayerAnimState->Set_MoveCheck(false);
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
				pPlayerAnimState->SetUp_State(CPlayer_DownAttack::Get_Instance());
				CPlayer_DownAttack::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
				return;
			}
			for (_uint j = 0; j < pPlayerAnimDesc.iKeyCount; ++j)
			{
				if (pPlayerAnimDesc.iKey[j] == 'W' && pPlayerAnimDesc.eKeyState[j] == CPlayer_AnimState::KEY_PRESING ||
					pPlayerAnimDesc.iKey[j] == 'S' && pPlayerAnimDesc.eKeyState[j] == CPlayer_AnimState::KEY_PRESING ||
					pPlayerAnimDesc.iKey[j] == 'A' && pPlayerAnimDesc.eKeyState[j] == CPlayer_AnimState::KEY_PRESING ||
					pPlayerAnimDesc.iKey[j] == 'D' && pPlayerAnimDesc.eKeyState[j] == CPlayer_AnimState::KEY_PRESING)
				{
					pPlayerAnimState->Set_MoveCheck(true);//여기 아까와도같은녀석이네..... 이것만고치면끝 고치고 히트시간대를보자
					break;
				}
				else
				{
					pPlayerAnimState->Set_MoveCheck(false);
				}
			}
		}
		if (pPlayerAnimDesc.iKey[i] == VK_END && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_UP && JETPACK_SKY == m_iCurMotion)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
			pPlayerAnimState->Set_FlyTimeAcc(0.f);
			pPlayerAnimState->Set_CollSpeed(0.f);
			pPlayerAnimState->Set_FlyGravity(false);
			m_pJetPack_Ui->Set_JetPackUsing(false);
			m_bJetPackSoundCheck = false;
			m_bFallandSky = true;
			m_iCurMotion = FALLING;
			break;
		}
		if (pPlayerAnimDesc.iKey[i] == VK_LBUTTON && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
		{
			m_fFlyTimeAcc = 0.f;
			m_iJumpCount = 0;
			m_bJetPackSoundCheck = false;
			m_bFallandSky = false;
			m_bJumpCheck = false;
			pPlayerAnimState->Set_MoveCheck(false);
			pPlayerAnimState->Set_FlyGravity(false);
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
			pPlayerAnimState->SetUp_State(CPlayer_DownAttack::Get_Instance());
			CPlayer_DownAttack::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			return;
		}
	}

	if (false == pPlayerAnimState->Get_Gravity() && m_iCurMotion == FALLING ||
		false == pPlayerAnimState->Get_Gravity() && m_iCurMotion == JETPACK_SKY)
	{
		m_bJetPackSoundCheck = false;
		m_bFallandSky = false;
		m_bJumpCheck = false;
		m_iJumpCount = 0;
		m_pJetPack_Ui->Set_JetPackUsing(false);
		pPlayerAnimState->Set_MoveCheck(false);
		pPlayerAnimState->Set_FlyGravity(false);
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_JUMP);
		pPlayerAnimState->SetUp_State(CPlayer_LandingState::Get_Instance());
		CPlayer_LandingState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}



}

void CPlayer_JumpState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{

	if (true == m_bJumpCheck && m_iCurMotion == JUMP ||
		true == pPlayerAnimState->Get_Gravity() && true == m_bJumpCheck && m_iCurMotion == DOUBLEJUMP ||
		true == pPlayerAnimState->Get_Gravity() && true == m_bJumpCheck && m_iCurMotion == JETPACK_SKY)
	{
		if (m_iCurMotion == JETPACK_SKY)
		{
			pPlayerAnimState->Set_FlyGravity(true);
			m_iCurMotion = JETPACK_SKY;
		}
		else if (m_iCurMotion == JUMP)
		{
			pPlayerAnimState->Set_FlyGravity(false);
			m_iCurMotion = FALLING;
		}
		else
		{
			pPlayerAnimState->Set_FlyGravity(false);
			m_iCurMotion = FALLING;
		}
	}



   	m_pModel->Set_Animation(m_iCurMotion);

	if (m_iCurMotion == FALLING && false == m_bFallandSky)
		m_pModel->Update(TimeDelta * 0.0005, &m_bJumpCheck, true, 1.0);
	else if (m_iCurMotion == FALLING && true == m_bFallandSky)
		m_pModel->Update(TimeDelta, &m_bJumpCheck, true);
	else if (m_iCurMotion == JETPACK_SKY)
		m_pModel->Update(TimeDelta);
	else if (m_iCurMotion == JUMP)
		m_pModel->Update(TimeDelta * 0.8 , &m_bJumpCheck, true);
	else
		m_pModel->Update(TimeDelta, &m_bJumpCheck, true);

	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_JumpState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_bJetPackSoundCheck = false;
	m_bFallandSky = false;
	m_bJumpCheck = false;
	m_iJumpCount = 0;
	pPlayerAnimState->Set_MoveCheck(false);
	pPlayerAnimState->Set_FlyGravity(false);
	pPlayerAnimState->Set_JumpTimeAcc(0.f);
	pPlayerAnimState->Set_JumpValue(0.f);
	pPlayerAnimState->Set_FlyTimeAcc(0.f);


}

void CPlayer_JumpState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_JumpState::Skylar_FlyJetPack(CPlayer_AnimState* pPlayerAnimState, _double TimeDelta)
{
	if (true == pPlayerAnimState->Get_Gravity() && m_iCurMotion == JETPACK_SKY)
	{
		pPlayerAnimState->Set_GravityPower(0.5f);
	}
}

void CPlayer_JumpState::Free()
{
}
