#include "..\public\GameObject_Manager.h"

#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{
}

CComponent * CGameObject_Manager::Get_Components(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex,_uint iLayerIndex)
{
	if (iLevelIndex >= m_iNumLevel)
		return nullptr;

	CLayer*	pLayer = Finder_Layer(iLevelIndex,pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Components(pComponentTag, iIndex, iLayerIndex);
}

CGameObject * CGameObject_Manager::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevel)
		return nullptr;

	CLayer*	pLayer = Finder_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Finder_LayerGameObject();
}

_uint CGameObject_Manager::Get_GameObjectSize(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevel)
		return -1;

	CLayer*	pLayer = Finder_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return -1;

	return pLayer->Get_LayerSize();
}

HRESULT CGameObject_Manager::Clear_Object(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevel)
		return E_FAIL;

	for (auto& Pair : m_mapLayer[iLevelIndex])
		Safe_Release(Pair.second);

	m_mapLayer[iLevelIndex].clear();

	for (auto& Pair : m_mapPrototype[iLevelIndex])
		Safe_Release(Pair.second);

	m_mapPrototype[iLevelIndex].clear();

	return S_OK;
}

HRESULT CGameObject_Manager::Release_GameObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevel)
		return E_FAIL;

	for (auto& Pair : m_mapLayer[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_mapLayer[iLevelIndex].clear();

	return S_OK;
}

HRESULT CGameObject_Manager::Release_GameObject_Layer(_uint iNumLevel, const _tchar * pLayerTag, _uint iIndex)
{
	if (iNumLevel >= m_iNumLevel)
		return E_FAIL;

	CLayer*	pLayer = Finder_Layer(iNumLevel, pLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	pLayer->Release_Layer(iIndex);
	return S_OK;
}

HRESULT CGameObject_Manager::Reserve_GameObjectManager(_uint iLevelIndex)
{
	if (nullptr != m_mapPrototype ||
		nullptr != m_mapLayer)
		return E_FAIL;

	m_mapPrototype = new map<const _tchar*, CGameObject*>[iLevelIndex];

	m_mapLayer = new map<const _tchar*, CLayer*>[iLevelIndex];

	m_iNumLevel = iLevelIndex;

	return S_OK;
}

HRESULT CGameObject_Manager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (iLevelIndex >= m_iNumLevel ||
		nullptr == m_mapPrototype ||
		nullptr == pGameObject ||
		nullptr != Finder_GameObject(iLevelIndex,pPrototypeTag))
		return E_FAIL;

	m_mapPrototype[iLevelIndex].emplace(pPrototypeTag, pGameObject);

	return S_OK;
}

HRESULT CGameObject_Manager::Add_ObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, _uint iLayerLevelIndex, const _tchar * pLayerTag, void * pArg, CGameObject ** ppOutGameObject)
{
	if (iLevelIndex >= m_iNumLevel ||
		iLayerLevelIndex >= m_iNumLevel ||
		nullptr == m_mapPrototype ||
		nullptr == m_mapLayer)
		return E_FAIL;

	CGameObject* pGameObject = Finder_GameObject(iLevelIndex, pPrototypeTag);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Finder_Layer(iLayerLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		CGameObject*	pClone = pGameObject->Clone(pArg, iLayerLevelIndex);
		if (nullptr == pClone)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pClone)))
			return E_FAIL;
		if (nullptr != ppOutGameObject)
		{
			*ppOutGameObject = pClone;
			Safe_AddRef(pClone);
		}
		m_mapLayer[iLayerLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		CGameObject*	pClone = pGameObject->Clone(pArg, iLayerLevelIndex);
		if (nullptr == pClone)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pClone)))
			return E_FAIL;
		if (nullptr != ppOutGameObject)
		{
			*ppOutGameObject = pClone;
			Safe_AddRef(pClone);
		}
	}

	return S_OK;
}

_int CGameObject_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_mapPrototype ||
		nullptr == m_mapLayer)
		return -1;

	_int	iProGressID = 0;

	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		auto iter_begin = m_mapLayer[i].begin();

		for (; iter_begin != m_mapLayer[i].end(); ++iter_begin)
		{
			if (nullptr != iter_begin->second)
				iProGressID = iter_begin->second->Tick(TimeDelta);

			if (m_mapLayer[i].empty())
				break;
		}
	}
	return iProGressID;
}

_int CGameObject_Manager::LateTick(_double TimeDelta)
{
	if (nullptr == m_mapPrototype ||
		nullptr == m_mapLayer)
		return -1;

	_int	iProGressID = 0;

	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		auto iter_begin = m_mapLayer[i].begin();

		for (; iter_begin != m_mapLayer[i].end(); ++iter_begin)
		{
			if (nullptr != iter_begin->second)
				iProGressID = iter_begin->second->LateTick(TimeDelta);

			if (m_mapLayer[i].empty())
				break;
		}
	}

	return iProGressID;
}

CGameObject * CGameObject_Manager::Finder_GameObject(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto Pair = find_if(m_mapPrototype[iLevelIndex].begin(), m_mapPrototype[iLevelIndex].end(), CTagFinder(pPrototypeTag));
	
	if (Pair == m_mapPrototype[iLevelIndex].end())
		return nullptr;
	
	return Pair->second;
}

CLayer * CGameObject_Manager::Finder_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto Pair = find_if(m_mapLayer[iLevelIndex].begin(), m_mapLayer[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (Pair == m_mapLayer[iLevelIndex].end())
		return nullptr;

	return Pair->second;
}

void CGameObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_mapLayer[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapLayer[i].clear();
	}
	Safe_Delete_Array(m_mapLayer);


	for (_uint i= 0; i < m_iNumLevel; ++i)
	{
		for (auto& Pair : m_mapPrototype[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapPrototype[i].clear();
	}
	Safe_Delete_Array(m_mapPrototype);

}
