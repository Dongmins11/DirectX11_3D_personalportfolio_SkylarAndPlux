#include "..\public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
	:m_pLevel(nullptr)
	,m_iCurruntLevel(0)
{
}

HRESULT CLevel_Manager::Open_Level(_uint iCurrurntLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == pNewLevel)
		return E_FAIL;

	if (nullptr != m_pLevel)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Clear_LevelResource(m_iCurruntLevel);
		Safe_Release(pGameInstance);
	}

	Safe_Release(m_pLevel);

	m_pLevel = pNewLevel;

	m_iCurruntLevel = iCurrurntLevelIndex;

	return S_OK;
}

_int CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pLevel)
		return -1;

	if (0 > m_pLevel->Tick(TimeDelta))
		return -1;

	return _int();
}

HRESULT CLevel_Manager::Render()
{	
	if (nullptr == m_pLevel)
		return E_FAIL;

	if (FAILED(m_pLevel->Render()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pLevel);
}
