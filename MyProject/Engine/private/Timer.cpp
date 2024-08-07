#include "..\public\Timer.h"



CTimer::CTimer()
	:m_TimeDelta(0.0)
{
	ZeroMemory(&m_Current_Time, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Old_Time, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Origin_Time, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_Cpu_Tick, sizeof(LARGE_INTEGER));
}

HRESULT CTimer::NativeConstruct()
{
	QueryPerformanceCounter(&m_Current_Time);
	QueryPerformanceCounter(&m_Old_Time);
	QueryPerformanceCounter(&m_Origin_Time);
	QueryPerformanceFrequency(&m_Cpu_Tick);

	return S_OK;
}

_double CTimer::Compute_TimeDelta()
{
	QueryPerformanceCounter(&m_Current_Time);

	if (m_Current_Time.QuadPart - m_Old_Time.QuadPart > m_Cpu_Tick.QuadPart)
	{
		QueryPerformanceFrequency(&m_Cpu_Tick);
		m_Origin_Time = m_Current_Time;
	}

	m_TimeDelta = (_double)(m_Current_Time.QuadPart - m_Old_Time.QuadPart) / m_Cpu_Tick.QuadPart;

	m_Old_Time = m_Current_Time;

	return m_TimeDelta;
}

CTimer * CTimer::Create()
{
	CTimer* pInstance = new CTimer;
	if (pInstance->NativeConstruct())
	{
		MSGBOX("Failed to Create Timer");

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free()
{
}
