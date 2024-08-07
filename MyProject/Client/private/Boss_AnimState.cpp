#include "stdafx.h"
#include "..\public\Boss_AnimState.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
#include "Boss_State.h"
#include "Boss_IdleState.h"
#include "Boss_SwingAttackState.h"
#include "Boss_TackDownState.h"
#include "Boss_GunAttackState.h"
#include "Boss_HitState.h"
#include "Boss_BackUpState.h"

CBoss_AnimState::CBoss_AnimState()
{
}

HRESULT CBoss_AnimState::NativeConstruct(CBossCRT * pBoss, CTransform * BossHeadTrans, CTransform * BossLeftHandTrans, CTransform * BossRightHandTrans, CModel * BossHeadModel, CModel * BossLeftHandModel, CModel * BossRightHandModel, CColider * HeadColider, CColider * LeftColider, CColider * RightColider, _uint iMyLevel)
{
	if (nullptr == pBoss || nullptr == BossHeadTrans || nullptr == BossHeadModel)
		return E_FAIL;

	m_pBoss = pBoss;

	m_pHeadTrasform = BossHeadTrans;
	m_pLeftHandTrasform = BossLeftHandTrans;
	m_pRightHandTrasform = BossRightHandTrans;

	m_pHeadModel = BossHeadModel;
	m_pLeftHandModel = BossLeftHandModel;
	m_pRightHandModel = BossRightHandModel;

	m_pHeadCollider = HeadColider;
	m_pLeftHandCollider = LeftColider;
	m_pRightHandCollider = RightColider;

	m_iMyLevel = iMyLevel;

	CBoss_IdleState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	CBoss_SwingAttackState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	CBoss_TackDownState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	CBoss_GunAttackState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	CBoss_HitState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	CBoss_BackUpState::Get_Instance()->Set_StateInfo(m_pBoss, m_pHeadTrasform, m_pLeftHandTrasform, m_pRightHandTrasform, m_pHeadModel, m_pLeftHandModel, m_pRightHandModel,
		m_pHeadCollider, m_pLeftHandCollider, m_pRightHandCollider, m_iMyLevel);

	m_pBossState = CBoss_IdleState::Get_Instance();

	ZeroMemory(&m_tBossAnimDesc, sizeof(BOSSANIMDESC));

	return S_OK;
}

void CBoss_AnimState::Boss_Hit_Check()
{
	if (false == m_bHitCheck)
	{
		m_pBossState->Reset_Anim(this);
		m_pBossState = CBoss_HitState::Get_Instance();
		CBoss_HitState::Get_Instance()->HandleInput(this, m_tBossAnimDesc);
		m_bHitCheck = true;
	}
}

void CBoss_AnimState::SetUp_State(CBoss_State * pAnimState)
{
	m_pBossState = pAnimState;
}

void CBoss_AnimState::Update(_double TimeDelta)
{
	m_pBossState->Set_Collision(this);
	m_pBossState->HandleInput(this, m_tBossAnimDesc);
	m_pBossState->Update(this, m_tBossAnimDesc, TimeDelta);
}

CBoss_AnimState* CBoss_AnimState::Create(CBossCRT * pBoss, CTransform * BossHeadTrans, CTransform * BossLeftHandTrans, CTransform * BossRightHandTrans, CModel * BossHeadModel, CModel * BossLeftHandModel, CModel * BossRightHandModel, CColider * HeadColider, CColider * LeftColider, CColider * RightColider, _uint iMyLevel)
{
	CBoss_AnimState* pInstance = new CBoss_AnimState;

	if (FAILED(pInstance->NativeConstruct(pBoss, BossHeadTrans, BossLeftHandTrans, BossRightHandTrans,
											BossHeadModel, BossLeftHandModel, BossRightHandModel,
											HeadColider, LeftColider, RightColider, iMyLevel)))
	{
		MSGBOX("Failed to Create : CBoss_AnimState");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_AnimState::Free()
{
	CBoss_IdleState::Destroy_Instance();
	CBoss_SwingAttackState::Destroy_Instance();
	CBoss_TackDownState::Destroy_Instance();
	CBoss_GunAttackState::Destroy_Instance();
	CBoss_HitState::Destroy_Instance();
	CBoss_BackUpState::Destroy_Instance();
}

