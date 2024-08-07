#include "stdafx.h"
#include "..\public\Player_DoubleJumpState.h"
#include "Player_JumpState.h"
#include "Player_IdleState.h"


IMPLEMENT_SINGLETON(CPlayer_DoubleJumpState)

CPlayer_DoubleJumpState::CPlayer_DoubleJumpState()
{
}

_uint CPlayer_DoubleJumpState::HandleInput(CPlayer_AnimState * pPlayerAnimState, const CRock::ANIMDESC & pPlayerAnimDesc)
{



	return DOUBLEJUMP;
}

void CPlayer_DoubleJumpState::Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta)
{
}

void CPlayer_DoubleJumpState::Free()
{
}
