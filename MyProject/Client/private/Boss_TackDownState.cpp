#include "stdafx.h"
#include "..\public\Boss_TackDownState.h"
#include "GameInstance.h"
#include "Boss_IdleState.h"
#include "Rock.h"
#include "Player_AnimState.h"
#include "Camera_Fly.h"

IMPLEMENT_SINGLETON(CBoss_TackDownState)
CBoss_TackDownState::CBoss_TackDownState()
{
}

void CBoss_TackDownState::HandleInput(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc)
{
	if (false == m_bFirstCheck)
	{
		CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
		CSound_Manager::Get_Instance()->PlaySound(L"BossEletricTwo", CSound_Manager::ID_BOSS_ATTCK);

		m_iLeftCurMotion = BOSS_IDLE_MOTION;
		m_iRightCurMotion = BOSS_IDLE_MOTION;
		m_bFirstCheck = true;
	}

	if (true == m_bIdleCheck)
	{
		m_bShakeCheck = false;
		m_bTimeCheck = false;
		m_bFirstCheck = false;
		m_bIdleCheck = false;
		m_bStateChange = !m_bStateChange;
		pBossAnimState->SetUp_State(CBoss_IdleState::Get_Instance());
		CBoss_IdleState::Get_Instance()->HandleInput(pBossAnimState, pBossAnimDesc);
		return;
	}

}

void CBoss_TackDownState::Update(CBoss_AnimState * pBossAnimState, const CBoss_AnimState::BOSSANIMDESC & pBossAnimDesc, _double TimeDelta)
{
	if (false == m_bStateChange)
	{
		if (FAILED(Second_Left_HandAnim(TimeDelta)))
			return;
		if (FAILED(Second_Right_HandAnim(TimeDelta)))
			return;
	}
	else
	{
		if (FAILED(Left_HandAnim(TimeDelta)))
			return;
		if (FAILED(Right_HandAnim(TimeDelta)))
			return;
	}
	if (TAKE_DOWN_ATTACK_LOOP_MOTION == m_iLeftCurMotion)
	{
		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta,&m_bLeftAnimEndCheck,true);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta,nullptr,true);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
	else if(TAKE_DOWN_ATTACK_MOTION == m_iLeftCurMotion)
	{

		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta,nullptr ,false);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta,nullptr, false);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
	else
	{
		m_pLeftHandModel->Set_Animation(m_iLeftCurMotion);
		m_pLeftHandModel->Update(TimeDelta);
		m_pLeftHandModel->Set_PreAnimation(m_iLeftCurMotion);

		m_pRightHandModel->Set_Animation(m_iRightCurMotion);
		m_pRightHandModel->Update(TimeDelta);
		m_pRightHandModel->Set_PreAnimation(m_iRightCurMotion);
	}
}

void CBoss_TackDownState::Reset_Anim(CBoss_AnimState * pBossAnimState)
{
	m_bShakeCheck = false;
	m_bTimeCheck = false;
	m_bFirstCheck = false;
	m_bIdleCheck = false;
}

void CBoss_TackDownState::Set_Collision(CBoss_AnimState * pBossAnimState)
{
}

HRESULT CBoss_TackDownState::Left_HandAnim(_double TimeDelta)
{

	if (false == m_bTimeCheck)
	{
		m_dTimeCheck += TimeDelta;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
		if (nullptr == pTargetTransform || nullptr == pPlayerObject)
		{
			Safe_Release(pGameInstance);
			MSGBOX("Failed To Get_Component Player Transform or PlayerObject");
			return E_FAIL;
			}

		if (0.8 >= m_dTimeCheck)
		{
			XMStoreFloat4(&m_vSourcePos, m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat4(&m_vDestPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
			
			if(CRock::PLAYER_LOOK_FRONT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_FRONTRIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_RIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if(CRock::PLAYER_LOOK_FRONTLEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_RIGHT)) * 10));
			else if (CRock::PLAYER_LOOK_LEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_RIGHT)) * 10));
			else if (CRock::PLAYER_LOOK_BACK == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_BACKLEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_BACKRIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));


			m_vDestPos.y = 35;
			_vector vPos = XMVectorLerp(XMLoadFloat4(&m_vSourcePos), XMLoadFloat4(&m_vDestPos), 0.05);
			m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);
		}
		if(0.5 <= m_dTimeCheck)
		{
			m_iLeftCurMotion = TAKE_DOWN_ATTACK_LOOP_MOTION;
			m_iRightCurMotion = TAKE_DOWN_ATTACK_LOOP_MOTION;
		}

		if (true == m_bLeftAnimEndCheck)
		{
			m_dTimeCheck = 0.0;
			m_bTimeCheck = true;
			m_bLeftAnimEndCheck = false;
			m_iLeftCurMotion = TAKE_DOWN_ATTACK_MOTION;
			m_iRightCurMotion = TAKE_DOWN_ATTACK_MOTION;
		}
		Safe_Release(pGameInstance);
	}

	if (true == m_bTimeCheck && false == m_bIdleCheck)
	{
		m_dTimeCheck += TimeDelta;

		_float4 vPos;
		XMStoreFloat4(&vPos,m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
		_float4 vDestPos = _float4(vPos.x, 8.5f, vPos.z, 1.f);

		_vector vMyPos = XMVectorLerp(XMLoadFloat4(&vPos), XMLoadFloat4(&vDestPos), 0.5);
		m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, vMyPos);

		if (0.15 <= m_dTimeCheck && false == m_bShakeCheck)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);


			CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
			if (nullptr == pCam)
			{
				MSGBOX("Failed to Finder : Boss Hit Cam in : pCam");
				Safe_Release(pGameInstance);
				return E_FAIL;
			}

			MODELDESC	tModelDesc;
			ZeroMemory(&tModelDesc, sizeof(MODELDESC));
			XMStoreFloat4(&tModelDesc.vPos,m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION));
			vPos.y = 7.5f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Boss"), LEVEL_BOSS_STAGE, L"Clone_Effect_Boss",&tModelDesc)))
				return E_FAIL;

			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->PlaySound(L"TakeDown", CSound_Manager::ID_BOSS_ATTCK);

			dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
			dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(6);

			Safe_Release(pGameInstance);

			m_bShakeCheck = true;
		}

		if (1.0 <= m_dTimeCheck)
		{
			m_dTimeCheck = 0.0;
			m_bIdleCheck = true;
		}
	}

	return S_OK;
}

HRESULT CBoss_TackDownState::Right_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(3.f, 0.f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector RightArmPos = m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, RightArmPos, 0.1);
	m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pRightHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_TackDownState::Second_Left_HandAnim(_double TimeDelta)
{
	XMStoreFloat4(&m_vDistance, XMVectorSet(-3.f, 0.f, 0.f, 1.f));
	_vector CrtPos = XMVector3TransformCoord(XMLoadFloat4(&m_vDistance), m_pHeadTrasform->Get_WorldMatrix());
	_vector LeftArmPos = m_pLeftHandTrasform->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = XMVectorLerp(CrtPos, LeftArmPos, 0.1);
	m_pLeftHandTrasform->Set_State(CTransform::STATE_POSITION, MyPos);

	m_pLeftHandTrasform->Set_MyLook(-m_pHeadTrasform->Get_State(CTransform::STATE_LOOK));

	return S_OK;
}

HRESULT CBoss_TackDownState::Second_Right_HandAnim(_double TimeDelta)
{
	if (false == m_bTimeCheck)
	{
		m_dTimeCheck += TimeDelta;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
		if (nullptr == pTargetTransform || nullptr == pPlayerObject)
		{
			Safe_Release(pGameInstance);
			MSGBOX("Failed To Get_Component Player Transform or PlayerObject");
			return E_FAIL;
		}



		if (0.8 >= m_dTimeCheck)
		{
			XMStoreFloat4(&m_vSourcePos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat4(&m_vDestPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));

			if (CRock::PLAYER_LOOK_FRONT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_FRONTRIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_RIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_FRONTLEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_RIGHT)) * 10));
			else if (CRock::PLAYER_LOOK_LEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) += (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_RIGHT)) * 10));
			else if (CRock::PLAYER_LOOK_BACK == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_BACKLEFT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));
			else if (CRock::PLAYER_LOOK_BACKRIGHT == dynamic_cast<CRock*>(pPlayerObject)->Get_PlayerLook())
				XMStoreFloat4(&m_vDestPos, XMLoadFloat4(&m_vDestPos) -= (XMVector4Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK)) * 10));

			m_vDestPos.y = 35;
			_vector vPos = XMVectorLerp(XMLoadFloat4(&m_vSourcePos), XMLoadFloat4(&m_vDestPos), 0.05);
			m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, vPos);
		}

		if (0.5 <= m_dTimeCheck)
		{
			m_iLeftCurMotion = TAKE_DOWN_ATTACK_LOOP_MOTION;
			m_iRightCurMotion = TAKE_DOWN_ATTACK_LOOP_MOTION;
		}

		if (true == m_bLeftAnimEndCheck)
		{
			m_dTimeCheck = 0.0;
			m_bTimeCheck = true;
			m_bLeftAnimEndCheck = false;
			m_iLeftCurMotion = TAKE_DOWN_ATTACK_MOTION;
			m_iRightCurMotion = TAKE_DOWN_ATTACK_MOTION;
		}
		Safe_Release(pGameInstance);
	}

	if (true == m_bTimeCheck && false == m_bIdleCheck)
	{
		m_dTimeCheck += TimeDelta;

		_float4 vPos;
		XMStoreFloat4(&vPos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
		_float4 vDestPos = _float4(vPos.x, 8.5f, vPos.z, 1.f);

		_vector vMyPos = XMVectorLerp(XMLoadFloat4(&vPos), XMLoadFloat4(&vDestPos), 0.5);
		m_pRightHandTrasform->Set_State(CTransform::STATE_POSITION, vMyPos);

		if (0.15 <= m_dTimeCheck && false == m_bShakeCheck)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);


			CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
			if (nullptr == pCam)
			{
				MSGBOX("Failed to Finder : Boss Hit Cam in : pCam");
				Safe_Release(pGameInstance);
				return E_FAIL;
			}

			MODELDESC	tModelDesc;
			ZeroMemory(&tModelDesc, sizeof(MODELDESC));
			XMStoreFloat4(&tModelDesc.vPos, m_pRightHandTrasform->Get_State(CTransform::STATE_POSITION));
			vPos.y = 7.0f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Boss"), LEVEL_BOSS_STAGE, L"Clone_Effect_Boss", &tModelDesc)))
				return E_FAIL;

			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_BOSS_ATTCK);
			CSound_Manager::Get_Instance()->PlaySound(L"TakeDown", CSound_Manager::ID_BOSS_ATTCK);

			dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
			dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(6);

			Safe_Release(pGameInstance);

			m_bShakeCheck = true;
		}

		if (1.0 <= m_dTimeCheck)
		{
			m_dTimeCheck = 0.0;
			m_bIdleCheck = true;
		}
	}

	return S_OK;
}

void CBoss_TackDownState::Free()
{
}
