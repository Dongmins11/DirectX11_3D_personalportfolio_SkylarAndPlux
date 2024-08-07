#include "..\public\Key_Manager.h"


IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
}

_bool CKey_Manager::Key_Up(_int Key)
{
	if (!(GetKeyState(Key) & 0x8000) && true == m_bKeyState[Key])
	{
		m_bKeyState[Key] = !m_bKeyState[Key];
		return true;
	}

	if ((GetKeyState(Key) & 0x8000) && false == m_bKeyState[Key])
	{
     	m_bKeyState[Key] = !m_bKeyState[Key];
		return false;
	}
	return false;
}

_bool CKey_Manager::Key_Down(_int Key)
{
	if (!m_bKeyState[Key] && (GetAsyncKeyState(Key) & 0x8000))
	{
		m_bKeyState[Key] = !m_bKeyState[Key];
		return true;
	}

	if (m_bKeyState[Key] && !(GetAsyncKeyState(Key) & 0x8000))
		m_bKeyState[Key] = !m_bKeyState[Key];

	return false;
}

_bool CKey_Manager::Key_Presing(_int Key)
{
	if (GetAsyncKeyState(Key) & 0x8000)
		return true;

	return false;
}

void CKey_Manager::Free()
{
}

