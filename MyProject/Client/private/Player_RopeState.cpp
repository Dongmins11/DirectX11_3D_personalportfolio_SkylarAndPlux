#include "stdafx.h"
#include "..\public\Player_RopeState.h"
#include "GameInstance.h"
#include "Player_JumpState.h"
#include "RopeRobot.h"

IMPLEMENT_SINGLETON(CPlayer_RopeState)
CPlayer_RopeState::CPlayer_RopeState()
{

}

void CPlayer_RopeState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{
	pPlayerAnimState->Set_MoveCheck(false);

	if (false == m_RealFirstCheck)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_SWINGNODE);
		CSound_Manager::Get_Instance()->PlaySound(L"SwingNode", CSound_Manager::ID_PLAYER_SWINGNODE);
		m_RealFirstCheck = true;
	}


	for (_uint i = 0; i < pPlayerAnimDesc.iKeyCount; ++i)
	{
		if (pPlayerAnimDesc.iKey[i] == 'F' && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
		{
			m_RotationCheck = false;
			m_iCurMotion = ROPE_FRONT;
			break;
		}
		if (pPlayerAnimDesc.iKey[i] == VK_SPACE && pPlayerAnimDesc.eKeyState[i] == CPlayer_AnimState::KEY_DOWN)
		{
			if (nullptr != m_pRopeRobot)
			{
				m_vDistance = _float4(0.f, 0.f, 0.f, 0.f);
				m_pRopeRobot->Set_RenderCheck(false);
				m_pRopeRobot->Set_RotationCheck(false);
			}
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_PLAYER_SWINGNODE);
			m_RealFirstCheck = false;
			m_RenderCheck = false;
			m_RotationCheck = false;
			pPlayerAnimState->SetUp_State(CPlayer_JumpState::Get_Instance());
			CPlayer_JumpState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
			return;
		}
	}
	if (nullptr != m_pRopeRobot)
	{
		if (m_iCurMotion == ROPE_FRONT && true == m_RotationCheck)
		{
			m_iCurMotion = ROPE_BACK;
			m_RotationCheck = false;
			if (0 > m_vDistance.z)
			{
				m_FirstCheck = true;
				m_pRopeRobot->Set_RotationCheck(true);
			}
			else
			{
				m_FirstCheck = false;
				m_pRopeRobot->Set_RotationCheck(false);
			}
			return;
		}
		if (m_iCurMotion == ROPE_BACK && true == m_RotationCheck)
		{
			m_iCurMotion = ROPE_FRONT;
			m_RotationCheck = false;
			if (true == m_FirstCheck)
				m_pRopeRobot->Set_RotationCheck(false);
			else
				m_pRopeRobot->Set_RotationCheck(true);
			return;
		}


	}
}

void CPlayer_RopeState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
	if (nullptr == m_pRopeRobot)
		return;

	pPlayerAnimState->Set_Gravity(false);


	if (false == m_RenderCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iLevelIndex, TEXT("Clone_RopeRobot"), TEXT("Com_SphereTransform"), 0, 0);
		if (nullptr == pTargetTransform)
		{
			Safe_Release(pGameInstance);
			return;
		}
		Safe_Release(pGameInstance);

		_float3	Look;
		XMStoreFloat3(&Look, m_pTransform->Get_State(CTransform::STATE_LOOK));
		Look.y = 0.f;
		pTargetTransform->Set_MyLook(XMVector3Normalize(XMLoadFloat3(&Look)));


		XMStoreFloat4(&m_vDistance, m_pPlayer->Get_Player_ColiderMat().r[3] - m_pRopeRobot->Get_RopeWolrdMatrix().r[3]);
		m_vDistance.x = 0;
		m_RenderCheck = true;
	}
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pRopeRobot->Get_RopeWolrdMatrix()));
	
	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta,&m_RotationCheck,true);
	m_pModel->Set_PreAnimation(m_iCurMotion);
}

void CPlayer_RopeState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_RotationCheck = false;
	m_RealFirstCheck = false;
	if (nullptr != m_pRopeRobot)
	{
		m_RenderCheck = false;
		m_vDistance = _float4(0.f, 0.f, 0.f, 0.f);
		m_pRopeRobot->Set_RenderCheck(false);
		m_pRopeRobot->Set_RotationCheck(false);
	}
}

void CPlayer_RopeState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(false);
}

void CPlayer_RopeState::Set_RopeSet(CRopeRobot * pRopeRobot)
{
	if (nullptr == pRopeRobot)
		return;

	if (nullptr != m_pRopeRobot)
	{
		m_pRopeRobot->Set_RenderCheck(false);
		Safe_Release(m_pRopeRobot);
	}
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iLevelIndex, L"Clone_Player");
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iLevelIndex, TEXT("RopeRobot0.fbx"), TEXT("Com_SphereTransform"), 0, 0);
	if (nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return;
	}

	Safe_Release(pGameInstance);

	m_pRopeRobot = pRopeRobot;
	Safe_AddRef(m_pRopeRobot);

	XMStoreFloat4(&m_vDistance, m_pPlayer->Get_Player_ColiderMat().r[3] - m_pRopeRobot->Get_RopeWolrdMatrix().r[3]);
	m_vDistance.x = 0;
	_float3	Look;
	XMStoreFloat3(&Look, m_pTransform->Get_State(CTransform::STATE_LOOK));
	Look.y = 0.f;
	pTargetTransform->Set_MyLook(XMVector3Normalize(XMLoadFloat3(&Look)));
	m_pRopeRobot->Set_Rotation(0.0);
	m_pRopeRobot->Set_RenderCheck(true);
	m_RenderCheck = true; 

	if (0 > m_vDistance.z)
	{
		m_FirstCheck = true;
		m_pRopeRobot->Set_RotationCheck(false);
	}
	else
	{
		m_FirstCheck = false;
		m_pRopeRobot->Set_RotationCheck(true);
	}
}

void CPlayer_RopeState::Free()
{
	Safe_Release(m_pRopeRobot);
}
