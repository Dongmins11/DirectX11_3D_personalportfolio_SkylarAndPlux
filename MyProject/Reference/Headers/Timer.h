#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() =default;
public:
	HRESULT NativeConstruct();
	_double Compute_TimeDelta();
private:
	LARGE_INTEGER		m_Current_Time;
	LARGE_INTEGER		m_Old_Time;
	LARGE_INTEGER		m_Origin_Time;
	LARGE_INTEGER		m_Cpu_Tick;
	_double				m_TimeDelta;
public:
	static CTimer* Create();
	virtual void Free() override;
};


END
