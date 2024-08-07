#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Client_BossAnimation.h"
#include "Boss_AnimState.h"
#include "BossCRT.h"
#include "GameInstance.h"
#include "Sound_Manager.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CColider;
END


BEGIN(Client)
class CBossCRT;
class CBoss_State abstract : public CBase
{
public:
	CBoss_State();
	virtual ~CBoss_State() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) = 0;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta) = 0;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState) = 0;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState) = 0;
public:
	void Set_StateInfo(CBossCRT* pBoss, CTransform* BossHeadTrans, CTransform* BossLeftHandTrans, CTransform* BossRightHandTrans,
						CModel* BossHeadModel, CModel* BossLeftHandModel, CModel* BossRightHandModel,
						CColider* HeadColider, CColider* LeftColider, CColider* RightColider,
						_uint iMyLevel);
protected:
	CBoss_State*	m_pBossState = nullptr;
	CBossCRT*		m_pBoss = nullptr;
protected:
	CTransform*		m_pHeadTrasform = nullptr;
	CTransform*		m_pLeftHandTrasform = nullptr;
	CTransform*		m_pRightHandTrasform = nullptr;
protected:
	CModel*			m_pHeadModel = nullptr;
	CModel*			m_pLeftHandModel = nullptr;
	CModel*			m_pRightHandModel = nullptr;
protected:
	CColider*		m_pHeadCollider = nullptr;
	CColider*		m_pLeftHandCollider = nullptr;
	CColider*		m_pRightHandCollider = nullptr;
protected:
	_uint		m_iMyLevel = 0;
	_float4		m_vDistance;
public:
	virtual void Free() override;
};

END
