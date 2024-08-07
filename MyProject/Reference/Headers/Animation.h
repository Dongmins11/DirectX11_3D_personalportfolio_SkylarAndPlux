#pragma once

#include "Base.h"

BEGIN(Engine)
class CModel;
class CAnimBone;
class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() =default;
public:
	HRESULT		NativeConstruct(_double	TickPerSecond,_double	Duration);
public:
	_bool		Get_isFinished() { return m_isFinished; }

public:
	void	Add_AnimBone(CAnimBone* pAnimBone) { m_vecAnimBone.push_back(pAnimBone); }

public:
	void	Update_AnimBoneTransformationMatrix(_double TimeDelta,_bool	ReturnBackCheck,_bool* pEndCheck);
	void	Update_NextAnimBoneTransformationMatrix(_double TimeDelta, CAnimation* pNextAnimation, CModel* pModel,_double BlenderTime);
private:
	vector<class CAnimBone*>		m_vecAnimBone;
private:
	_double			m_TimeAcc = 0.0;

	_double			m_TimeDelta = 0.0;

	_double			m_TickPerSecond = 0.0;

	_double			m_Duration = 0.0;

	_bool			m_isFinished = false;

	_uint			m_iCurrentKeyFrame = 0;

public:
	static CAnimation* Create(_double	TickPerSecond, _double	Duration);
	virtual void Free() override;
};

END