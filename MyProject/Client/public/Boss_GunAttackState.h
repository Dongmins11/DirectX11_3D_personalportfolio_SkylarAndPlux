#pragma once

#include "Boss_State.h"


BEGIN(Client)
class CBoss_GunAttackState final : public CBoss_State
{
	DECLARE_SINGLETON(CBoss_GunAttackState)
private:
	CBoss_GunAttackState();
	virtual ~CBoss_GunAttackState() = default;
public:
	virtual void HandleInput(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc) override;
	virtual void Update(CBoss_AnimState* pBossAnimState, const CBoss_AnimState::BOSSANIMDESC& pBossAnimDesc, _double TimeDelta)override;
	virtual void Reset_Anim(CBoss_AnimState* pBossAnimState)override;
	virtual void Set_Collision(CBoss_AnimState* pBossAnimState)override;
public:
	_float4x4	Get_LeftMat() { return m_LeftMatWorld; }
	_float4x4	Get_RightMat() { return m_RightMatWorld; }
private:
	HRESULT Left_HandAnim(_double TimeDelta);
	HRESULT Right_HandAnim(_double TimeDelta);
	HRESULT SetUp_TargetBoneMatrices(const char* pBoneName);
private:
	HRESULT Set_MatrixBone();
private:
	_uint	m_iLeftCurMotion = 0;
	_uint	m_iRightCurMotion = 0;
private:
	_bool	m_bLeftAnimEndCheck = false;
	_bool	m_bRightAnimEndCheck = false;
private:
	_double m_dAnimRation = 0;
private:
	_bool	m_bLoopCheck = false;
	_bool	m_bFirstCheck = false;
private:
	_double m_dAttackTimeAcc = 0.0;
private:
	_float4	m_vLeftBonePos;
	_float4	m_vRightBonePos;
private:
	_bool	m_bGunSoundCheck = false;
	_bool	m_bGunSoundCheckTwo = false;
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
public:
	virtual void Free() override;

};

END
