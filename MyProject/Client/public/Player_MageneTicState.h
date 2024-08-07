#pragma once

#include "Player_State.h"


BEGIN(Client)

class CPlayer_MageneTicState final :public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_MageneTicState)
private:
	CPlayer_MageneTicState();
	virtual ~CPlayer_MageneTicState() = default;
public:
	void	Set_IdleTime(_double TimeAcc) { m_TimeAcc = TimeAcc; }
	_double	Get_IdleTime() { return m_TimeAcc; }
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
public:
	void	Set_MageneticCheck(_bool bCheck) { m_bMageneticCheck = bCheck; }
	_bool	Get_MageneticCheck() { return m_bMageneticCheck; }
public:
	void	Set_Magenetic_Cur_Check(_bool bCheck) { m_bCullCheck = bCheck; }
	_bool	Get_Magenetic_Cur_Check() { return m_bCullCheck; }
private:
	_uint		m_iCurMotion = 0;
	_double		m_TimeAcc = 0.0;
	_bool		m_bMageneticCheck = false;
	_bool		m_bCullCheck = false;
	_bool		m_bPlayerUpCheck = false;
public:
	virtual void Free() override;

};

END