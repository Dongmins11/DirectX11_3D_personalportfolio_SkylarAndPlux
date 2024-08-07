#pragma once

#include "Boss_State.h"


BEGIN(Client)
class CBoss_TackDownState final : public CBoss_State
{
	DECLARE_SINGLETON(CBoss_TackDownState)
private:
	CBoss_TackDownState();
	virtual ~CBoss_TackDownState() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) override;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta)override;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState)override;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState)override;
private:
	HRESULT Left_HandAnim(_double TimeDelta);
	HRESULT Right_HandAnim(_double TimeDelta);
private:
	HRESULT Second_Left_HandAnim(_double TimeDelta);
	HRESULT Second_Right_HandAnim(_double TimeDelta);
private:
	_uint	m_iLeftCurMotion = 0;
	_uint	m_iRightCurMotion = 0;
private:
	_bool	m_bLeftAnimEndCheck = false;
	_bool	m_bRightAnimEndCheck = false;
private:
	_double m_dAnimRation = 0.0;
	_double m_dTimeCheck = 0.0;
private:
	_bool	m_bAnimCheck = false;
private:
	_bool	m_bFirstCheck = false;
private:
	_bool	m_bTimeCheck = false;
	_bool	m_bIdleCheck = false;
private:
	_bool	m_bShakeCheck = false;
private:
	_float4	m_vSourcePos;
	_float4	m_vDestPos;
private:
	_bool	m_bStateChange = false;
public:
	virtual void Free() override;

};

END
