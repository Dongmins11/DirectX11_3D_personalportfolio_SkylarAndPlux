#pragma once

#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager();
	virtual ~CKey_Manager() =default;
public:
	_bool	Key_Up(_int	Key);
	_bool	Key_Down(_int Key);
	_bool	Key_Presing(_int Key);
private:
	bool	m_bKeyState[0xFF];
public:
	virtual void Free() override;
};

END