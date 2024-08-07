#include "stdafx.h"
#include "..\public\Player_MageneTicState.h"
#include "Player_IdleState.h"
#include "GravityMesh.h"

IMPLEMENT_SINGLETON(CPlayer_MageneTicState)
CPlayer_MageneTicState::CPlayer_MageneTicState()
{
}

void CPlayer_MageneTicState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc)
{

	if (true == m_bMageneticCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject* pTargetObject = pGameInstance->Get_GameObject(m_iLevelIndex, L"GravityMesh0.fbx");
		dynamic_cast<CGravityMesh*>(pTargetObject)->Set_MoveCheck(false);
		Safe_Release(pGameInstance);

		m_bCullCheck = false;
		m_bMageneticCheck = false;
		pPlayerAnimState->Set_MoveCheck(false);
		pPlayerAnimState->SetUp_State(CPlayer_IdleState::Get_Instance());
		CPlayer_IdleState::Get_Instance()->HandleInput(pPlayerAnimState, pPlayerAnimDesc);
		return;
	}
	else
	{
		m_iCurMotion = MAGENETIC;
	}

	m_bCullCheck = true;
}

void CPlayer_MageneTicState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{

	if (false == m_bPlayerUpCheck)
	{
		m_pTransform->Set_PosUp(1.f);
		//올라갈것처리
	}

	pPlayerAnimState->Set_MageneTicCheck(true);
	m_pModel->Set_Animation(m_iCurMotion);
	m_pModel->Update(TimeDelta);
	m_pModel->Set_PreAnimation(m_iCurMotion);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pTargetObject = pGameInstance->Get_GameObject(m_iLevelIndex, L"GravityMesh0.fbx");
	CColider*	pTargetAABB = (CColider*)pGameInstance->Get_Components(m_iLevelIndex, L"GravityMesh0.fbx", L"Com_AABB", 0, 0);
	if (nullptr == pTargetObject && nullptr == pTargetAABB)
	{
		Safe_Release(pGameInstance);
		return;
	}

	if (true == m_pAABB->Collision_AABB(pTargetAABB))
	{
		m_bPlayerUpCheck = true;
		dynamic_cast<CGravityMesh*>(pTargetObject)->Set_MoveCheck(true);//후이동할것처리
	}
	else
		dynamic_cast<CGravityMesh*>(pTargetObject)->Set_MoveCheck(false);

	Safe_Release(pGameInstance);


}

void CPlayer_MageneTicState::Reset_Anim(CPlayer_AnimState * pPlayerAnimState)
{
	m_bMageneticCheck = false;
	m_bCullCheck = false;
	pPlayerAnimState->Set_MageneTicCheck(false);
}

void CPlayer_MageneTicState::Set_Collision(CPlayer_AnimState * pPlayerAnimState)
{
	m_pHandSphere->Set_CheckCollision(true);
}

void CPlayer_MageneTicState::Free()
{

}