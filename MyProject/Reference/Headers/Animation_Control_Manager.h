#pragma once

#include "Base.h"

BEGIN(Engine)
class CModel;
class CAnimation_Control_Manager final : public CBase
{
	DECLARE_SINGLETON(CAnimation_Control_Manager)
public:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_JUMP, STATE_DOUBLEJUMP, STATE_JETPACK, STATE_ATTACK,STATE_DOWNATTCK, STATE_ROPE, STATE_FALL, STATE_END };
private:
	CAnimation_Control_Manager();
	virtual ~CAnimation_Control_Manager() = default;
public:
	HRESULT		Set_Key_Check(_int InputKey,CModel* pModel);
	HRESULT		Update_Player_Key_Contral(CModel* pModel,_double TimeDelta);
	HRESULT		AniMation_Check(CModel* pModel, _double TimeDelta);
private:
	HRESULT		Anim_Idle(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_Walk(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_Jump(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_DoubleJump(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_JetPack(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_Attack(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_DownAttack(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_Rope(CModel* pModel, _double TimeDelta);
	HRESULT		Anim_Fall(CModel* pModel, _double TimeDelta);
private:
	class CKey_Manager* pKey_Manager = nullptr;
	class CInput_Device* pInput_Device = nullptr;
private:
	STATE				m_ePreState = STATE_END;
	STATE				m_eCurState = STATE_END;
	_double				m_dTimeDelta = 0.0;
	_byte				m_iKey = 0;
	_uint				m_iAttackCount = 0;
public:
	virtual void Free() override;
};

END