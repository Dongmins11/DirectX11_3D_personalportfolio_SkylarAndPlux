#include "stdafx.h"
#include "..\public\Boss_State.h"
#include "Boss_AnimState.h"
#include "BossCRT.h"
#include "GameInstance.h"

CBoss_State::CBoss_State()
{
}

void CBoss_State::Set_StateInfo(CBossCRT * pBoss, CTransform * BossHeadTrans, CTransform * BossLeftHandTrans, CTransform * BossRightHandTrans, CModel * BossHeadModel, CModel * BossLeftHandModel, CModel * BossRightHandModel, CColider * HeadColider, CColider * LeftColider, CColider * RightColider, _uint iMyLevel)
{
	if (nullptr == pBoss || nullptr == BossHeadTrans || nullptr == BossHeadModel)
	{
		MSGBOX("Failed to Set Boss State");
		return;
	}

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

}

void CBoss_State::Free()
{
}

