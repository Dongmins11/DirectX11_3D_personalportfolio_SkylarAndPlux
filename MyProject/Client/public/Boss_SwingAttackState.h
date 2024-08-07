#pragma once

#include "Boss_State.h"


BEGIN(Client)
class CBoss_SwingAttackState final : public CBoss_State
{
	DECLARE_SINGLETON(CBoss_SwingAttackState)
private:
	CBoss_SwingAttackState();
	virtual ~CBoss_SwingAttackState() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) override;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta)override;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState)override;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState)override;
private:
	HRESULT Left_HandAnim(_double TimeDelta);
	HRESULT Right_HandAnim(_double TimeDelta);
private:
	HRESULT Second_Left_HandAnim(_double TimeDelta);
	HRESULT Second_Right_HandAnim(_double TimeDelta);
private:
	HRESULT SetUp_TargetBoneMatrices(const char* pBoneName);
private:
	HRESULT Set_MatrixBone();
private:
	_uint	m_iLeftCurMotion = 0;
	_uint	m_iRightCurMotion = 0;
private:
	_bool	m_bLeftPosSaveCheck = false;
	_bool	m_bRightPosSaveCheck = false;
private:
	_bool	m_bLeftPosEndCheck = false;
	_bool	m_bRightPosEndCheck = false;
private:
	_bool	m_bLeftAnimEndCheck = false;
	_bool	m_bRightAnimEndCheck = false;
private:
	_double m_dLeftAnimRation = 0;
	_double m_dRightAnimRation = 0;
private:
	_bool	m_bChangeCheck = false;
private:
	_double m_dChangeTime = 0;
private:
	_bool	m_bSecondChangeCheck = false;
private:
	_bool	m_bFirstCheck = false;
private:
	_bool	m_LeftEffectCheckOne = false;
	_bool	m_LeftEffectCheckTwo = false;
	_bool	m_LeftEffectCheckThree = false;
	_bool	m_LeftEffectCheckFor = false;
	_bool	m_LeftEffectCheckFive = false;
	_bool	m_LeftEffectCheckSix = false;
private:
	_bool	m_RightEffectCheckOne = false;
	_bool	m_RightEffectCheckTwo = false;
	_bool	m_RightEffectCheckThree = false;
	_bool	m_RightEffectCheckFor = false;
	_bool	m_RightEffectCheckFive = false;
	_bool	m_RightEffectCheckSix = false;
private:
	_float4x4				m_LeftOffsetMatrix;
	_float4x4				m_LeftPivotMatrix;
	const _float4x4*		m_pLeftCombinedMatrix = nullptr;
	const _float4x4*		m_pLeftTargetWorldMatrix;
	_float4x4				m_LeftMatWorld;
private:
	_float4x4				m_RightOffsetMatrix;
	_float4x4				m_RightPivotMatrix;
	const _float4x4*		m_pRightCombinedMatrix = nullptr;
	const _float4x4*		m_pRightTargetWorldMatrix;
	_float4x4				m_RightMatWorld;
private:
	_float4	m_vLeftBonePos;
	_float4	m_vRightBonePos;
private:
	_float4	m_vLeftPos;
public:
	virtual void Free() override;

};

END
