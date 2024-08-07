#include "stdafx.h"
#include "..\public\Player_AnimState.h"
#include "Rock.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
#include "Player_State.h"
#include "Player_IdleState.h"
#include "Player_AttackState.h"
#include "Player_WalkState.h"
#include "Player_JumpState.h"
#include "Player_LandingState.h"
#include "Player_FallingState.h"
#include "Player_DownAttack.h"
#include "Player_JetPackState.h"
#include "Player_HitState.h"
#include "Player_DeadState.h"
#include "Player_RopeState.h"
#include "Player_MageneTicState.h"


CPlayer_AnimState::CPlayer_AnimState()
{
}

HRESULT CPlayer_AnimState::NativeConstruct(CRock * pPlayer, CTransform * pTransform, CModel* pModel, CColider* pAABB,CColider* pSphere, CColider* pHandSphere, _uint iMyLevel)
{
	if (nullptr == pPlayer || nullptr == pTransform)
		return E_FAIL;

	m_pPlayer = pPlayer;
	m_pTransform = pTransform;
	m_pModel = pModel;
	m_pAABB = pAABB;
	m_pSphere = pSphere;
	m_pHandSphere = pHandSphere;
	m_iMyLevel = iMyLevel;

	CPlayer_IdleState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_AttackState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_WalkState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_JumpState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_LandingState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_FallingState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_DownAttack::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_JetPackState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB , m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_HitState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_DeadState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_RopeState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);
	CPlayer_MageneTicState::Get_Instance()->Set_StateInfo(m_pPlayer, m_pAABB, m_pTransform, m_pModel, m_pSphere, m_pHandSphere, m_iMyLevel);

	m_pAnimState = CPlayer_IdleState::Get_Instance();
	ZeroMemory(&m_tAnimDesc, sizeof(ANIMDESC));

	m_fMaxFallSpeed = 40.f;
	m_fGravityPower = 1.2f;
	m_dMoveValue = 3.0;
	m_Player_Rotation = _float3(0.f, 0.f, 0.f);

	return S_OK;

}
void CPlayer_AnimState::SetUp_State(CPlayer_State * pAnimState)
{
	m_pAnimState = pAnimState;
}

void CPlayer_AnimState::Update(_double TimeDelta)
{
	ZeroMemory(&m_tAnimDesc, sizeof(ANIMDESC));
	Key_SetUp(TimeDelta);

	if (FAILED(Rope_Anim()))
		return;

	m_pAnimState->Set_Collision(this);
	m_pAnimState->HandleInput(this, m_tAnimDesc);
	Move_State();
	m_pAnimState->Update(this,TimeDelta);
	if(m_bIsMoveCheck)
		Player_Move(TimeDelta);
}

void CPlayer_AnimState::Key_SetUp(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_iKeyStack = 0;

	if (pGameInstance->Key_Presing('W'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'W';
		m_tAnimDesc.iOnceKey = 'W';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;

	}
	if (Key_Up('W'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'W';
		m_tAnimDesc.iOnceKey = 'W';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;

	}
	if (pGameInstance->Key_Presing('S'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'S';
		m_tAnimDesc.iOnceKey = 'S';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_BACK;
	}
	if (Key_Up('S'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'S';
		m_tAnimDesc.iOnceKey = 'S';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;

	}
	if (pGameInstance->Key_Presing('A'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'A';
		m_tAnimDesc.iOnceKey = 'A';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_LEFT;
	}
	if (Key_Up('A'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'A';
		m_tAnimDesc.iOnceKey = 'A';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;

	}
	if (pGameInstance->Key_Presing('D'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'D';
		m_tAnimDesc.iOnceKey = 'D';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_RIGHT;
	}
	if (Key_Up('D'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'D';
		m_tAnimDesc.iOnceKey = 'D';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_WALK;
		m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;

	}
	if (true == pGameInstance->Key_Down(VK_SPACE))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_SPACE;
		m_tAnimDesc.iOnceKey = VK_SPACE;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_DOWN;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_JUMP;
	}


	if (true == pGameInstance->Key_Presing(VK_SPACE))
	{
 		m_tAnimDesc.iKey[m_iKeyStack] = VK_END;
		m_tAnimDesc.iOnceKey = VK_END;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_JETPACK_SKY;
		m_fFlyTimeAcc += TimeDelta;
	}
	if (true == Key_Up(VK_SPACE))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_END;
        m_tAnimDesc.iOnceKey = VK_END;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_JETPACK_SKY;
	}


	if (pGameInstance->Key_Presing(VK_LSHIFT))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_LSHIFT;
		m_tAnimDesc.iOnceKey = VK_LSHIFT;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_PRESING;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_JETPACK;
	}
	if (true == Key_Up(VK_LSHIFT))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_LSHIFT;
		m_tAnimDesc.iOnceKey = VK_LSHIFT;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_UP;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_JETPACK;
	}


	if (pGameInstance->Key_Down(VK_LBUTTON))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_LBUTTON;
		m_tAnimDesc.iOnceKey = VK_LBUTTON;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_DOWN;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_ATTACK;
	}
	if (pGameInstance->Key_Down(VK_RBUTTON))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = VK_RBUTTON;
		m_tAnimDesc.iOnceKey = VK_RBUTTON;
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_DOWN;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_ATTACK;
	}
	//if (true == m_bRopeCollisionCheck)
	//{
	//	if (pGameInstance->Key_Down('F'))
	//	{
	//		m_tAnimDesc.iKey[m_iKeyStack] = 'F';
	//		m_tAnimDesc.iOnceKey = 'F';
	//		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_DOWN;
	//		++m_tAnimDesc.iKeyCount;
	//		++m_iKeyStack;
	//		m_ePlayerState = PLAYER_STATE_ATTACK;

	//		m_bRopeKeyCheck = true;
	//	}
	//	else
	//		m_bRopeKeyCheck = false;
	//}


	Safe_Release(pGameInstance);
}


void CPlayer_AnimState::Player_Move(_double TimeDelta)
{
	Set_Look(TimeDelta);
	m_pTransform->Go_Straight(TimeDelta * m_dMoveValue);
}

void CPlayer_AnimState::Move_State()
{
	for (_uint i = 0; i < m_tAnimDesc.iKeyCount; ++i)
	{
		if (m_tAnimDesc.iKey[i] == 'W' && m_tAnimDesc.eKeyState[i] == KEY_PRESING)
		{
			m_ePlayerLook = CRock::PLAYER_LOOK_FRONT;
			for (_uint j = 0; j < m_tAnimDesc.iKeyCount; ++j)
			{
				if (m_tAnimDesc.iKey[j] == 'D' && m_tAnimDesc.eKeyState[j] == KEY_PRESING)
				{
					m_ePlayerLook = CRock::PLAYER_LOOK_FRONTRIGHT;
					return;
				}
				else if (m_tAnimDesc.iKey[j] == 'A' && m_tAnimDesc.eKeyState[j] == KEY_PRESING)
				{
					m_ePlayerLook = CRock::PLAYER_LOOK_FRONTLEFT;
					return;
				}
			}
		}
		else if (m_tAnimDesc.iKey[i] == 'S' && m_tAnimDesc.eKeyState[i] == KEY_PRESING)
		{
			m_ePlayerLook = CRock::PLAYER_LOOK_BACK;
			for (_uint j = 0; j < m_tAnimDesc.iKeyCount; ++j)
			{
				if (m_tAnimDesc.iKey[j] == 'D' && m_tAnimDesc.eKeyState[j] == KEY_PRESING)
				{
					m_ePlayerLook = CRock::PLAYER_LOOK_BACKRIGHT;
					return;
				}
				else if (m_tAnimDesc.iKey[j] == 'A' && m_tAnimDesc.eKeyState[j] == KEY_PRESING)
				{
					m_ePlayerLook = CRock::PLAYER_LOOK_BACKLEFT;
					return;
				}
			}
		}
		else if (m_tAnimDesc.iKey[i] == 'D' && m_tAnimDesc.eKeyState[i] == KEY_PRESING)
		{
			m_ePlayerLook = CRock::PLAYER_LOOK_RIGHT;
			return;
		}
		else if (m_tAnimDesc.iKey[i] == 'A' && m_tAnimDesc.eKeyState[i] == KEY_PRESING)
		{
			m_ePlayerLook = CRock::PLAYER_LOOK_LEFT;
			return;
		}
	}
}

void CPlayer_AnimState::Set_Look(_double TimeDelta)
{
	_vector	Player_Look;

	if (true == m_bIsMoveCheck)
		XMStoreFloat3(&m_Player_Rotation, Player_Move_Direction(TimeDelta));
	else
		XMStoreFloat3(&m_Player_Rotation, m_pTransform->Get_State(CTransform::STATE_LOOK));
	m_Player_Rotation.y = 0.f;
	Player_Look = XMLoadFloat3(&m_Player_Rotation);

	m_pTransform->Set_MyLook(Player_Look);
}

_vector CPlayer_AnimState::Player_Move_Direction(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pCameraFly = pGameInstance->Get_GameObject(m_iMyLevel,L"Clone_Camera_Fly");
	if (nullptr == pCameraFly)
	{
		Safe_Release(pGameInstance);
		return XMVectorSet(0.f,0.f,0.f,0.f);
	}

	_matrix CamWorldMat = dynamic_cast<CCamera_Fly*>(pCameraFly)->Get_WorldMatrixCam();
	_vector	vCameraVector = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	switch (m_ePlayerLook)
	{
	case Client::CRock::PLAYER_LOOK_FRONT:
		vCameraVector = CamWorldMat.r[2];
		break;
	case Client::CRock::PLAYER_LOOK_FRONTLEFT:
		vCameraVector = CamWorldMat.r[2] - CamWorldMat.r[0];
		break;
	case Client::CRock::PLAYER_LOOK_FRONTRIGHT:
		vCameraVector = CamWorldMat.r[2] + CamWorldMat.r[0];
		break;
	case Client::CRock::PLAYER_LOOK_BACK:
		vCameraVector = -CamWorldMat.r[2];
		break;
	case Client::CRock::PLAYER_LOOK_BACKLEFT:
		vCameraVector = -CamWorldMat.r[2] - CamWorldMat.r[0];
		break;
	case Client::CRock::PLAYER_LOOK_BACKRIGHT:
		vCameraVector = -CamWorldMat.r[2] + CamWorldMat.r[0];
		break;
	case Client::CRock::PLAYER_LOOK_LEFT:
		vCameraVector = -CamWorldMat.r[0];
		break;
	case Client::CRock::PLAYER_LOOK_RIGHT:
		vCameraVector = CamWorldMat.r[0];
		break;
	}

	Safe_Release(pGameInstance);

	return XMVector3Normalize(vCameraVector);
}

void CPlayer_AnimState::Jump(_double TimeDelta)
{
	if (nullptr == m_pTransform)
		return;

	if (true == m_bJump)
	{
		m_fJumpTimeAcc += TimeDelta * 2;
		m_pTransform->Set_PosJump(m_vMyPos.y + (m_fJumpPower * m_fJumpTimeAcc - 0.5f *  9.8f * m_fJumpTimeAcc * m_fJumpTimeAcc));
		_float FlyValue = (m_fJumpPower * m_fJumpTimeAcc - 0.5f *  9.8f * m_fJumpTimeAcc * m_fJumpTimeAcc);

		if (FlyValue < m_fJumpValue || true == m_bDownAttack)
		{
			m_fJumpValue = 0.f;
			m_fCollSpeed = 0.f;
			m_fFallSpeedValue = 0.f;
			m_fJumpTimeAcc = 0.f;
			m_fDownAttackTimeAcc = 0.f;
			m_bDownAttack = false;
			m_bJump = false;
			m_bGravity = true;
		}
		else
		{
			m_tPlayerCollisionDesc->bCollisionY = false;
			m_bGravity = false;
		}

		m_fJumpValue = FlyValue;
	}
}

void CPlayer_AnimState::Gravity(_double TimeDelta)
{
	if (false == m_bMageneticCheck)
	{
		if (true == m_bGravity)
		{
			if (true == m_bDownAttackGravity)
			{
				m_fDownAttackTimeAcc += TimeDelta;
				if (0.1f <= m_fDownAttackTimeAcc)
					m_fCollSpeed += m_fGravityPower;
				else
					m_fCollSpeed += TimeDelta;
			}
			else
				m_fCollSpeed += m_fGravityPower;

			if (true == m_bFlyGravity)
				m_fFallSpeedValue += 0.01f;
			else
				m_fFallSpeedValue += TimeDelta*9.8f*1.f * m_fCollSpeed;

			if (m_fMaxFallSpeed <= m_fFallSpeedValue)
				m_fFallSpeedValue = m_fMaxFallSpeed;

			if (0.23 <= m_fFallSpeedValue)
			{
				m_bRealGravity = true;
			}
			else
				m_bRealGravity = false;

			m_pTransform->Set_PosGravity(-m_fFallSpeedValue * TimeDelta);
		}
	}
}


HRESULT CPlayer_AnimState::Collision_Check()
{
	if (nullptr == m_pAABB)
		return E_FAIL;

	if (false == m_bMageneticCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CColider* pCollider = (CColider*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Colider_Object", L"Com_AABB", 0);
		if (nullptr == pCollider)
			return E_FAIL;

		m_tPlayerCollisionDesc = m_pPlayer->Get_Player_Collision();

		if (true == m_tPlayerCollisionDesc->bMainCollision)
		{
			if (true == m_tPlayerCollisionDesc->bCollisionX)
			{
				_float3 vPos;
				XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
				vPos.x += m_tPlayerCollisionDesc->fDisX;
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&vPos));
				m_tPlayerCollisionDesc->bCollisionX = false;
			}
			if (true == m_tPlayerCollisionDesc->bCollisionY)
			{
				_float3 vPos;
				XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
				vPos.y += m_tPlayerCollisionDesc->fDisY;
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&vPos));

				m_fDownAttackTimeAcc = 0.f;
				m_bDownAttackGravity = false;
				m_bGravity = false;
				m_bJump = false;
				m_fJumpValue = 0.f;
				m_fCollSpeed = 0.f;
				m_fFallSpeedValue = 0.f;
				m_tPlayerCollisionDesc->bCollisionY = false;
			}

			if (true == m_tPlayerCollisionDesc->bCollisionZ)
			{
				_float3 vPos;
				XMStoreFloat3(&vPos, m_pTransform->Get_State(CTransform::STATE_POSITION));
				vPos.z += m_tPlayerCollisionDesc->fDisZ;
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&vPos));
				m_tPlayerCollisionDesc->bCollisionZ = false;
			}
			m_tPlayerCollisionDesc->bMainCollision = false;
		}
		else
			m_bGravity = true;

		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CPlayer_AnimState::BackUp_Pos()
{
	if (nullptr == m_pTransform)
		return E_FAIL;

	XMStoreFloat4(&m_vBackupPos, m_pTransform->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CPlayer_AnimState::Check_Hit(_bool CollisionCheck)
{
	if (nullptr == m_pSphere)
		return;

	if (true == CollisionCheck)
	{
		if (false == m_bHitAnim)
		{
			m_pAnimState->Reset_Anim(this);
			m_pAnimState = CPlayer_HitState::Get_Instance();
			CPlayer_HitState::Get_Instance()->HandleInput(this, m_tAnimDesc);
			m_bHitAnim = true;
		}
	}

}

HRESULT CPlayer_AnimState::Rope_Anim()
{
	//if (true == m_bRopeKeyCheck)
	//{
	//	m_pAnimState->Reset_Anim(this);
	//	m_pAnimState = CPlayer_RopeState::Get_Instance();
	//	CPlayer_RopeState::Get_Instance()->HandleInput(this, m_tAnimDesc);
	//}

	return S_OK;
}

void CPlayer_AnimState::Set_RopeObject(CRopeRobot * pRopeObject)
{
	if (nullptr == pRopeObject)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down('F'))
	{
		m_tAnimDesc.iKey[m_iKeyStack] = 'F';
		m_tAnimDesc.iOnceKey = 'F';
		m_tAnimDesc.eKeyState[m_iKeyStack] = KEY_DOWN;
		++m_tAnimDesc.iKeyCount;
		++m_iKeyStack;
		m_ePlayerState = PLAYER_STATE_ATTACK;
		m_bRopeKeyCheck = true;
		m_pAnimState->Reset_Anim(this);
		CPlayer_RopeState::Get_Instance()->Set_RopeSet(pRopeObject);
		m_pAnimState = CPlayer_RopeState::Get_Instance();
		CPlayer_RopeState::Get_Instance()->HandleInput(this, m_tAnimDesc);
	}

	Safe_Release(pGameInstance);
}

HRESULT CPlayer_AnimState::Dead_Anim()
{
	m_pAnimState->Reset_Anim(this);
	m_pAnimState = CPlayer_DeadState::Get_Instance();
	CPlayer_DeadState::Get_Instance()->HandleInput(this, m_tAnimDesc);

	return S_OK;
}

HRESULT CPlayer_AnimState::Magenetic_Change()
{
	if (false == CPlayer_MageneTicState::Get_Instance()->Get_Magenetic_Cur_Check())
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		m_pAnimState->Reset_Anim(this);
		m_pAnimState = CPlayer_MageneTicState::Get_Instance();
		CPlayer_MageneTicState::Get_Instance()->HandleInput(this, m_tAnimDesc);

		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CPlayer_AnimState::Set_Magenetic_Idle(_bool bCheck)
{
	if (nullptr == m_pAnimState)
		return E_FAIL;

	CPlayer_MageneTicState::Get_Instance()->Set_MageneticCheck(bCheck);

	return S_OK;
}

_bool CPlayer_AnimState::Key_Up(_int iKey)
{
	if (!(GetKeyState(iKey) & 0x8000) && true == m_bKeyState[iKey])
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}

	if ((GetKeyState(iKey) & 0x8000) && false == m_bKeyState[iKey])
	{
  		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return false;
	}
	return false;
}

CPlayer_AnimState * CPlayer_AnimState::Create(CRock * pPlayer, CTransform * pTransform, CModel* pModel, CColider* pAABB, CColider* pSphere, CColider* pHandSphere, _uint iMyLevel)
{
	CPlayer_AnimState* pInstance = new CPlayer_AnimState;
	if (FAILED(pInstance->NativeConstruct(pPlayer, pTransform, pModel, pAABB, pSphere, pHandSphere, iMyLevel)))
	{
		MSGBOX("Failed to Create AnimState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_AnimState::Free()
{
	CPlayer_MageneTicState::Destroy_Instance();
	CPlayer_RopeState::Destroy_Instance();
	CPlayer_HitState::Destroy_Instance();
	CPlayer_DeadState::Destroy_Instance();
	CPlayer_AttackState::Destroy_Instance();
	CPlayer_IdleState::Destroy_Instance();
	CPlayer_LandingState::Destroy_Instance();
	CPlayer_WalkState::Destroy_Instance();
	CPlayer_JumpState::Destroy_Instance();
	CPlayer_FallingState::Destroy_Instance();
	CPlayer_DownAttack::Destroy_Instance();
	CPlayer_JetPackState::Destroy_Instance();
}

