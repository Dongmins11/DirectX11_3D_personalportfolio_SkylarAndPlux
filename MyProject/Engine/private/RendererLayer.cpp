#include "..\public\RendererLayer.h"
#include "GameObject.h"


CRendererLayer::CRendererLayer()
{
}

HRESULT CRendererLayer::Add_RenderGroup(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_listRendererLayer.emplace_back(pGameObject);
	Safe_AddRef(pGameObject);
	return S_OK;
}

HRESULT CRendererLayer::Render()
{
	for (auto& iter : m_listRendererLayer)
	{
		if (nullptr != iter)
		{
			if (FAILED(iter->Render()))
				return E_FAIL;
		}
		Safe_Release(iter);
	}
	m_listRendererLayer.clear();

	return S_OK;
}

HRESULT CRendererLayer::Sort_Render()
{
	m_listRendererLayer.sort([&](CGameObject* pSour, CGameObject* pDest) { return pSour->Get_ViewZ() > pDest->Get_ViewZ(); });

	for (auto& iter : m_listRendererLayer)
	{
		if (nullptr != iter)
		{
			if (FAILED(iter->Render()))
				return E_FAIL;
		}
		Safe_Release(iter);
	}
	m_listRendererLayer.clear();

	return S_OK;
}

CRendererLayer * CRendererLayer::Create()
{
	CRendererLayer* pInstance = new CRendererLayer;

	return pInstance;
}

void CRendererLayer::Free()
{
	for (auto& iter : m_listRendererLayer)
		Safe_Release(iter);
	m_listRendererLayer.clear();
}

