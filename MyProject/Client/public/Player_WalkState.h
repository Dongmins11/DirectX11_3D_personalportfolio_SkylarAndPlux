#pragma once

#include "Player_State.h"

BEGIN(Client)
class CPlayer_WalkState final :public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_WalkState)
private:
	CPlayer_WalkState();
	virtual ~CPlayer_WalkState() =default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState) override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_uint		m_iCurMotion = 0;
	_bool		m_bIsFallingCheck = false;
private:
	_double		m_dWalkTimeAcc = 0.0;
public:
	virtual void Free() override;
};

END