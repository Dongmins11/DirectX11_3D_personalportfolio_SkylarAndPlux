#pragma once

#include "Client_Defines.h"
#include "Client_BossAnimation.h"
#include "Base.h"
#include "BossCRT.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CColider;
END


BEGIN(Client)
class CBossCRT;
class CBoss_State;
class CBoss_AnimState final : public CBase
{
public:
	enum BOSS_STATE { BOSS_STATE_IDLE, BOSS_STATE_HANDATTACK, BOSS_STATE_GUNATTACK, BOSS_STATE_HANDTAKEDOWN, BOSS_STATE_HIT, BOSS_STATE_END };
public:
	typedef struct tagBossAnimDesc
	{
		_uint eType;
	}BOSSANIMDESC;

private:
	explicit CBoss_AnimState();
	virtual ~CBoss_AnimState() =default;
public:
	HRESULT NativeConstruct(CBossCRT* pBoss, CTransform* BossHeadTrans, CTransform* BossLeftHandTrans, CTransform* BossRightHandTrans,
							CModel* BossHeadModel, CModel* BossLeftHandModel, CModel* BossRightHandModel,
							CColider* HeadColider, CColider* LeftColider, CColider* RightColider,
							_uint iMyLevel);
public:
	void	Boss_Hit_Check();
public:
	void	SetUp_State(CBoss_State* pAnimState);
	void	Update(_double TimeDelta);
public:
	void	Set_BossHItCheck(_bool bCheck) { m_bHitCheck = bCheck; }
private:
	CBoss_State*	m_pBossState = nullptr;
	CBossCRT*		m_pBoss = nullptr;
private:
	CTransform*		m_pHeadTrasform = nullptr;
	CTransform*		m_pLeftHandTrasform = nullptr;
	CTransform*		m_pRightHandTrasform = nullptr;
private:
	CModel*			m_pHeadModel = nullptr;
	CModel*			m_pLeftHandModel = nullptr;
	CModel*			m_pRightHandModel = nullptr;
private:
	CColider*		m_pHeadCollider = nullptr;
	CColider*		m_pLeftHandCollider = nullptr;
	CColider*		m_pRightHandCollider = nullptr;
private:
	BOSSANIMDESC	m_tBossAnimDesc;
private:
	_bool			m_bHitCheck = false;
private:
	_uint		m_iMyLevel = 0;
public:
	static CBoss_AnimState* Create(CBossCRT* pBoss, CTransform* BossHeadTrans, CTransform* BossLeftHandTrans, CTransform* BossRightHandTrans
								, CModel* BossHeadModel, CModel* BossLeftHandModel, CModel* BossRightHandModel,
									CColider* HeadColider, CColider* LeftColider, CColider* RightColider,
									_uint iMyLevel);
	virtual void Free() override;
};
END
