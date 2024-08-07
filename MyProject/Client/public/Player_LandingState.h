#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_LandingState final :public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_LandingState)
private:
	CPlayer_LandingState();
	virtual ~CPlayer_LandingState() = default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_uint	m_iCurMotion = 0;
private:
	_bool	m_bLandingCheck = false;
	_bool	m_bJumpBlend = false;
	_bool	m_bChangeMotion = false;
private:
	_bool	m_bFirstCheck = false;
public:
	virtual void Free() override;

};

END