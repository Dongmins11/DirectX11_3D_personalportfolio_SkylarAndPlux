#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_HitState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_HitState)
private:
	CPlayer_HitState();
	virtual ~CPlayer_HitState() =default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_bool		m_bAnimCheck = false;
public:
	virtual void Free() override;
};

END

