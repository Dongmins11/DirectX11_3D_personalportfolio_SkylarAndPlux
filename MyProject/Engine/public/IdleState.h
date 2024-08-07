#pragma once

#include "AnimState.h"

BEGIN(Engine)
class CIdleState final : public CAnimState
{
private:
	CIdleState();
	virtual ~CIdleState() =default;
public:
	virtual void Set_KeyInput(CModel* pModel, _int iKeyState) override;
	virtual void Anim_Update(CModel* pModel, _double TimeDelta) override;;

public:
	static	CIdleState*	Create();
	virtual void Free() override;
};

END