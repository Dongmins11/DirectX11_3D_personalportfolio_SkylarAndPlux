#include "..\public\Object_Component_Manager.h"
#include "Component.h"


CObject_Component_Manager::CObject_Component_Manager()
{
}

HRESULT CObject_Component_Manager::Add_Component_Layer(CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	m_ListComponentLayer.emplace_back(pComponent);
	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CObject_Component_Manager::Get_Component(_uint iIndex)
{
	if (0 > iIndex || iIndex >= m_ListComponentLayer.size())
		return nullptr;

	auto iter = m_ListComponentLayer.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

HRESULT CObject_Component_Manager::Relese_ComponentLayer(_uint iIndex)
{
	if (0 > iIndex || iIndex >= m_ListComponentLayer.size())
		return E_FAIL;;

	auto iter = m_ListComponentLayer.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	Safe_Release(*iter);
	iter = m_ListComponentLayer.erase(iter);

	return S_OK;
}

CObject_Component_Manager * CObject_Component_Manager::Create()
{
	CObject_Component_Manager*	pInstance = new CObject_Component_Manager;

	return pInstance;
}

void CObject_Component_Manager::Free()
{
	for (auto& iter : m_ListComponentLayer)
		Safe_Release(iter);

	m_ListComponentLayer.clear();
}

