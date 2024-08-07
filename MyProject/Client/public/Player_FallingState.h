#pragma once

#include "Player_State.h"

BEGIN(Client)

class CPlayer_FallingState final :public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_FallingState)
private:
	CPlayer_FallingState();
	virtual ~CPlayer_FallingState() =default;

public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
public:
	virtual void Free() override;


};

END