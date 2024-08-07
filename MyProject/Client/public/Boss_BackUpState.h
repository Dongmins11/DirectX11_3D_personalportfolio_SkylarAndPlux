#pragma once

#include "Boss_State.h"


BEGIN(Client)
class CBoss_BackUpState final : public CBoss_State
{
	DECLARE_SINGLETON(CBoss_BackUpState)
private:
	CBoss_BackUpState();
	virtual ~CBoss_BackUpState() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) override;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta)override;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState)override;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState)override;
private:
	HRESULT Left_HandAnim(_double TimeDelta);
	HRESULT Right_HandAnim(_double TimeDelta);
	HRESULT Head_Animation(_double TimeDelta);
	HRESULT Boss_Look(_double TimeDelta);
private:
	_uint	m_iLeftCurMotion = 0;
	_uint	m_iRightCurMotion = 0;
private:
	_bool	m_bLeftAnimEndCheck = false;
	_bool	m_bRightAnimEndCheck = false;
private:
	_bool	m_bEndFSM = false;
private:
	_bool	m_bBackUpPosChange = false;
private:
	_double m_dAnimRation = 0;
private:
	_bool	m_bLerpCheck = false;
public:
	virtual void Free() override;

};

END
