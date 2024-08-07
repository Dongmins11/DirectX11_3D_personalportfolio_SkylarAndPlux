#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_JumpState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_JumpState)
private:
	CPlayer_JumpState();
	virtual ~CPlayer_JumpState() =default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
	void	Skylar_FlyJetPack(CPlayer_AnimState * pPlayerAnimState,_double TimeDelta);
private:
	_uint	m_iJumpCount = 0;
	_uint	m_iCurMotion = 0;
	_float	m_fFlyTimeAcc = 0.f;
private:
	_bool	m_bJumpCheck = false;
	_bool	m_bJumpBlend = false;
	_bool	m_bFallandSky = false;
private:
	_bool	m_bJetPackSoundCheck = false;
public:
	virtual void Free() override;
};

END