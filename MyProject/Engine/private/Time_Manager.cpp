#include "..\public\Time_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTime_Manager)
CTime_Manager::CTime_Manager()
{
}

HRESULT CTime_Manager::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr != Find_Timer(pTimerTag))
		return E_FAIL;

	CTimer* pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimer.emplace(pTimerTag, pTimer);
	//�����
	//��ο�
	//hdr
	//�Ÿ��������
	//�޷�
	//ī�޶�䰪���̿��� �䰪���� 
	//ī�޶��ǹ�
	//�ν��Ͻ����ι�ġ
	//����ĳ��Ʈ
	return S_OK;
}

_double CTime_Manager::Compute_TimeDleta(const _tchar * pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return 0.0;

	return pTimer->Compute_TimeDelta();
}

CTimer * CTime_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTagFinder(pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void CTime_Manager::Free()
{
	for (auto& Pair : m_mapTimer)
		Safe_Release(Pair.second);

	m_mapTimer.clear();
}
