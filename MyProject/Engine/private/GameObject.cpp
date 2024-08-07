#include "..\public\GameObject.h"
#include "Object_Component_Manager.h"
#include "GameInstance.h"
#include "Component.h"


CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:m_pDevice(pDevice)
	,m_pDeviceContext(pDeviceContext)
	,m_bIsClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	:m_pDevice(rhs.m_pDevice)
	,m_pDeviceContext(rhs.m_pDeviceContext)
	, m_bIsClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CComponent * CGameObject::Get_Components(const _tchar * pComponentTag, _uint iLayerIndex)
{
	CObject_Component_Manager* pComponentLayer =  Finder_Components(pComponentTag);

	if (nullptr == pComponentLayer)
		return nullptr;

	return pComponentLayer->Get_Component(iLayerIndex);
}

HRESULT CGameObject::Native_Construct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Native_Construct(void * pArg, _uint iMyLevelIndex)
{
	m_iMyLevel = iMyLevelIndex;
	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}


HRESULT CGameObject::Release_Component()
{
	for (auto& Pair : m_mapComponents)
		Safe_Release(Pair.second);

	m_mapComponents.clear();

	return S_OK;
}

HRESULT CGameObject::Relese_Layer_Component(const _tchar* pLayerTag,_uint iIndex)
{
	CObject_Component_Manager*	pComponentLayer = Finder_Components(pLayerTag);
	if (nullptr == pComponentLayer)
		return E_FAIL;

	pComponentLayer->Relese_ComponentLayer(iIndex);
	
	return S_OK;
}

void CGameObject::Compute_ViewZ(CTransform * pTransform)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	m_fViewZ = XMVectorGetX(XMVector4Length(pTransform->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));
	Safe_Release(pGameInstance);
}

HRESULT CGameObject::SetUp_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentsTag, CComponent ** ppComponentOut, void * pArg)
{
	//if (nullptr != Finder_Components(pComponentsTag))
		//return E_FAIL;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*		pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	CObject_Component_Manager*	pComponentLayer = Finder_Components(pComponentsTag);

	if (nullptr == pComponentLayer)
	{
		CObject_Component_Manager*	pComponentLayer = CObject_Component_Manager::Create();
		pComponentLayer->Add_Component_Layer(pComponent);

		m_mapComponents.emplace(pComponentsTag, pComponentLayer);
	}
	else
		pComponentLayer->Add_Component_Layer(pComponent);

	*ppComponentOut = pComponent;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CGameObject::Get_ComponentSize(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentsTag)
{
	CObject_Component_Manager*	pComponentLayer = Finder_Components(pComponentsTag);

	if (nullptr == pComponentLayer)
		return -1;
	else
		return 0;

	return 0;
}

CObject_Component_Manager * CGameObject::Finder_Components(const _tchar * pComponentsTag)
{

	auto Pair = find_if(m_mapComponents.begin(), m_mapComponents.end(), CTagFinder(pComponentsTag));

	if (Pair == m_mapComponents.end())
		return nullptr;

	return Pair->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponents)
		Safe_Release(Pair.second);

	m_mapComponents.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

}
