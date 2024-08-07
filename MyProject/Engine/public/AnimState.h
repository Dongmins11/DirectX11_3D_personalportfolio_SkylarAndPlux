#pragma once

#include "Model.h"

BEGIN(Engine)

class CAnimState abstract : public CBase
{
public:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_JUMP, STATE_DOUBLEJUMP, STATE_JETPACK, STATE_ATTACK, STATE_ROPE, STATE_FALL, STATE_END };
protected:
	CAnimState();
	virtual ~CAnimState() = default;
public:
	virtual void Set_KeyInput(CModel* pModel,_int iKeyState)=0;
	virtual void Anim_Update(CModel* pModel, _double TimeDelta) = 0;
public:
	virtual void Free() override;
};

END
