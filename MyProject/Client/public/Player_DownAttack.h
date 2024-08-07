#pragma once

#include "Player_State.h"

BEGIN(Client)
class CPlayer_DownAttack final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_DownAttack)
private:
	CPlayer_DownAttack();
	virtual ~CPlayer_DownAttack() =default;
public:

	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_bool	m_bEndAnim = false;
	_bool	m_bLoopEnd = false;
	_bool	m_bLandingAnim = false;
	_bool	m_bDownStart = false;
public:
	virtual void Free() override;

};

END