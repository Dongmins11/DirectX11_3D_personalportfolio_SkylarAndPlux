#pragma once

#include "AnimState.h"

BEGIN(Engine)
class CWalkState final : public CAnimState
{
private:
	CWalkState();
	virtual ~CWalkState() =default;
public:
	virtual void Set_KeyInput(CModel* pModel, _int iKeyState)override;
	virtual void Anim_Update(CModel* pModel, _double TimeDelta)override;
public:
	static	CWalkState* Create();
	virtual void Free() override;
};

END