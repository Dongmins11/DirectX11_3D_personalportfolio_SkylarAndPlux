#include "Player_State.h"

BEGIN(Client)

class CPlayer_DeadState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_DeadState)
private:
	CPlayer_DeadState();
	virtual ~CPlayer_DeadState() = default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
private:
	_uint		m_iCurMotion = 0;
	_double		m_dTimeAcc = 0.0;
	_bool		m_bStateChange = false;
private:
	_bool		m_bFirstCheck = false;
public:
	virtual void Free() override;
};

END