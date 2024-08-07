#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_DoubleJumpState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_DoubleJumpState)
private:
	CPlayer_DoubleJumpState();
	virtual ~CPlayer_DoubleJumpState() =default;
public:
	virtual _uint HandleInput(CPlayer_AnimState * pPlayerAnimState, const CRock::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;

public:
	virtual void Free() override;

};

END