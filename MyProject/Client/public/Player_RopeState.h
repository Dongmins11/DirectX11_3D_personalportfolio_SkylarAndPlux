#include "Player_State.h"
#include "RopeRobot.h"
BEGIN(Client)

class CPlayer_RopeState final : public CPlayer_State
{
	DECLARE_SINGLETON(CPlayer_RopeState)
private:
	CPlayer_RopeState();
	virtual ~CPlayer_RopeState() = default;
public:
	virtual void HandleInput(CPlayer_AnimState * pPlayerAnimState, const CPlayer_AnimState::ANIMDESC & pPlayerAnimDesc) override;
	virtual void Update(CPlayer_AnimState * pPlayerAnimState, _double TimeDelta) override;
	virtual void Reset_Anim(CPlayer_AnimState* pPlayerAnimState)override;
	virtual void Set_Collision(CPlayer_AnimState* pPlayerAnimState) override;
	void	Set_RopeSet(CRopeRobot* pRopeRobot);
private:
	CRopeRobot*	m_pRopeRobot = nullptr;
	_uint		m_iCurMotion = 0;
	_float4		m_vDistance;
	_float4x4	Player_HandMatrix;
private:
	_bool		m_RenderCheck = false;
	_bool		m_FirstCheck = false;
private:
	_bool		m_RealFirstCheck = false;

private:
	_bool		m_RotationCheck = false;
public:
	virtual void Free() override;
};

END