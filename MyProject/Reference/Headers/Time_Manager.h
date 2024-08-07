#pragma once

#include "Base.h"

BEGIN(Engine)

class CTime_Manager final : public CBase
{
	DECLARE_SINGLETON(CTime_Manager);
private:
	CTime_Manager();
	virtual ~CTime_Manager() =default;

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_double Compute_TimeDleta(const _tchar* pTimerTag);
private:
	class CTimer*	Find_Timer(const _tchar* pTimerTag);
private:
	map<const _tchar*, class CTimer*>	m_mapTimer;
	typedef map<const _tchar*, class CTimer*>	TIMERS;

public:
	virtual void Free() override;
};

END