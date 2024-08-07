#pragma once

#include "Base.h"
#include "Level.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() =default;
public:
	HRESULT	Open_Level(_uint iCurrurntLevelIndex, CLevel* pNewLevel);
	_int	Tick(_double TimeDelta);
	HRESULT Render();
private:
	CLevel*	m_pLevel;
	_uint	m_iCurruntLevel;

public:
	virtual void Free() override;
};

END