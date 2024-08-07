#pragma once

#include "Player_State.h"


BEGIN(Client)

class CPlayer_IdleState final :public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_IdleState)
private:
	CPlayer_IdleState();
	virtual ~CPlayer_IdleState() = default;
public:
	void	Set_IdleTime(_double TimeAcc) { m_TimeAcc = TimeAcc; }
	_double	Get_IdleTime() { return m_TimeAcc; }
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;

private:
	_uint		m_iCurMotion = 0;
	_double		m_TimeAcc = 0.0;
public:
	virtual void Free() override;

};

END