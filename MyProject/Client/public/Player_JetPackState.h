#pragma once

#include "Player_State.h"

BEGIN(Client)
class CPlayer_JetPackState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_JetPackState)
private:
	CPlayer_JetPackState();
	virtual ~CPlayer_JetPackState() = default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_uint	m_iCurMotion = 0;
	_bool	m_bEndingAnim = false;
	_bool	m_LandingAnim = false;
	_bool	m_bStartAnim = false;
	_bool	m_bLoopAnim = false;
	_bool	m_bWaitAnim = false;
private:
	_bool	m_bFirstEffectCheck = false;
private:
	CGameObject*	m_pEffectObject = nullptr;
private:
	_double	m_StartTimeAcc = 0.f;
	_uint	m_iJumpCount = 0;
public:
	virtual void Free() override;

};

END