#include "..\public\Layer.h"
#include "GameObject.h"


CLayer::CLayer()
	:m_iDeadNumber(1)
{
}

CComponent * CLayer::Get_Components(const _tchar * pComponentTag, _uint iIndex, _uint iLayerIndex)
{
	auto iter = m_listGameObject.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Components(pComponentTag, iLayerIndex);
}

_uint CLayer::Get_LayerSize()
{
	return m_listGameObject.size();
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_listGameObject.emplace_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	_int	iProgressID = 0;

	if (m_listGameObject.empty())
		return iProgressID;

	auto iter = m_listGameObject.begin();
	for (; iter != m_listGameObject.end(); ++iter)
	{
		if (nullptr != *iter)
		{
			iProgressID = (*iter)->Tick(TimeDelta);
			if (m_iDeadNumber == iProgressID)
			{
				(*iter)->Release_Component();
				Safe_Release(*iter);
				iter = m_listGameObject.erase(iter);

				if (iter == m_listGameObject.end())
					return iProgressID;
				if (m_listGameObject.empty())
					return iProgressID;
			}
			if (0 > iProgressID)
				return iProgressID;
		}
	}
	return iProgressID;
}

_int CLayer::LateTick(_double TimeDelta)
{
	_int	iProgressID = 0;

	if (m_listGameObject.empty())
		return iProgressID;

	auto iter = m_listGameObject.begin();
	for (; iter != m_listGameObject.end(); ++iter)
	{
		if (nullptr != *iter)
		{
			iProgressID = (*iter)->LateTick(TimeDelta);
			if (m_iDeadNumber == iProgressID)
			{
				(*iter)->Release_Component();
				Safe_Release(*iter);
				iter = m_listGameObject.erase(iter);

				if (iter == m_listGameObject.end())
					return iProgressID;
				if (m_listGameObject.empty())
					return iProgressID;
			}
			if (0 > iProgressID)
				return iProgressID;
		}
	}
	return iProgressID;
}

CGameObject * CLayer::Finder_LayerGameObject()
{
	if (m_listGameObject.empty())
		return nullptr;

	return m_listGameObject.back();
}

HRESULT CLayer::Release_Layer(_uint iIndex)
{
	auto iter = m_listGameObject.begin();
	
	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	(*iter)->Release_Component();
	Safe_Release(*iter);
	m_listGameObject.erase(iter++);

	return S_OK;
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer;

	return pInstance;
}

void CLayer::Free()
{
	for (auto& iter : m_listGameObject)
		Safe_Release(iter);

	m_listGameObject.clear();
}

