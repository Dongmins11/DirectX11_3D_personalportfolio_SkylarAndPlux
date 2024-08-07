#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_AttackState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_AttackState)
private:
	CPlayer_AttackState();
	virtual ~CPlayer_AttackState() =default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_uint	m_iAttackCount = 0;
	_uint	m_iTimeDelta = 0;
	_bool	m_bIsCheck = false;
	_bool	m_bIsChangeMotion = false;
	_bool	m_bIsIdleChange = false;
	_bool	m_bIsOnceChage = false;
private:
	_double m_dAttackCollisionCheckTime = 0.0;

public:
	virtual void Free() override;
};

END