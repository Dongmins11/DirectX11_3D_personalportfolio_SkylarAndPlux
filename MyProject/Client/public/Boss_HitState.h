#pragma once

#include "Boss_State.h"


BEGIN(Client)
class CBoss_HitState final : public CBoss_State
{
	DECLARE_SINGLETON(CBoss_HitState)
private:
	CBoss_HitState();
	virtual ~CBoss_HitState() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) override;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta)override;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState)override;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState)override;
private:
	HRESULT Left_HandAnim(_double TimeDelta);
	HRESULT Right_HandAnim(_double TimeDelta);
	HRESULT Head_Animation(_double TimeDelta);
	HRESULT Head_Hit(_double TimeDelta);
	HRESULT Hit_Motion(_double TimeDelta);
private:
	HRESULT Cam_In(_double TimeDelta);
	HRESULT Cam_Out();
private:
	HRESULT Gravity(_double TimeDelta);
private:
	_uint	m_iLeftCurMotion = 0;
	_uint	m_iRightCurMotion = 0;
private:
	_bool	m_bLeftAnimEndCheck = false;
	_bool	m_bRightAnimEndCheck = false;
private:
	_bool	m_bLerpCheck = false;
private:
	_double m_dTimeLerpAcc = 0.0;
	_float	m_fRotZ = 0.f;
private:
	_double m_dAnimRation = 0;
private:
	_bool	m_bJump = false;
	_bool	m_bFristCheck = false;
	_double m_dJumpValue = 0.0;
	_double m_dJumpPower = 0.0;
	_double	m_dJumpTimeAcc = 0.0;
private:
	_float	m_fBackUp_BossY = 0.f;
	_float	m_fMyPosY = 0.f;
	_float4 m_vBackUpPos;
private:
	_bool	m_bGravityCheck = false;
private:
	_bool	m_bNextAnim = false;
private:
	_uint	m_iCount = 0;
	_float	m_fCollSpeed = 0.f;
	_float	m_fGravityPower = 0.f;
	_double	m_fGravityTimeAcc = 0.f;
	_double m_dMaxCallSpeed = 0.0;
private:
	_double m_dNextMotionCheckTime = 0.0;
private:
	_uint					m_iCurrntIndex = 0;
	_uint					m_iCurMotion = 0;
	_double					m_dTimeAcc = 0.0;
	_double					m_dCollisionTimeAcc = 0.0;
	_bool					m_bCheckCollision = false;
private:
	_double					m_vTimeAcc = 0.0;
	_float3					m_vBackDirection;
private:
	_float					m_vStartValue = 0.f;
	_float					m_vEndValue = 0.f;
	_float					m_vBlendValue = 0.f;
	_uint					m_iScaleIndex = 0;
	_double					m_dBlendTime = 0.f;
	_bool					m_bMotionCheck = false;
private:
	_uint					m_iHitIndex = 0;

public:
	virtual void Free() override;

};

END
