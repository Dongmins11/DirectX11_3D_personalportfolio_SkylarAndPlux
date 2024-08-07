#include "stdafx.h"
#include "..\Public\NavigationTool.h"
#include "NavigationCube.h"
#include "GameInstance.h"
#include "MainFrm.h"
#include "MyForm2.h"
#include "NaviTool.h"

CNavigationTool::CNavigationTool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CNavigationTool::CNavigationTool(const CNavigationTool & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNavigationTool::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationTool::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	return S_OK;
}

_int CNavigationTool::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (1 == m_iDeadNumber)
		return 1;

	return 0;
}

_int CNavigationTool::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CNavigationTool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationTool::Finder_MapCube(wstring MapTag)
{

	auto Pair = find_if(m_MapCube.begin(), m_MapCube.end(), CTagFinderWstring(MapTag.c_str()));

	if (Pair == m_MapCube.end())
		return E_FAIL;

	for (_int i = 0; i < 3; ++i)
		Pair->second[i]->Set_DieLayer(1);
	
	Pair->second.clear();
	m_MapCube.erase(Pair);

	return S_OK;
}

HRESULT CNavigationTool::Release_ComponentLayer(_uint iIdex)
{
	if (FAILED(__super::Relese_Layer_Component(L"Clone_Navi0",iIdex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationTool::SetUp_Component_Tool(_uint iLevel, const _tchar * PrototypeTag, const _tchar * LayerTag, void * pArg, const _tchar* CubeTag, wstring MapTag,_uint iPropertyIndex)
{

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	CVIBuffer_Navigation*	m_pVIBufferCom = nullptr;
	if (FAILED(__super::SetUp_Component(iLevel, PrototypeTag, LayerTag, (CComponent**)&m_pVIBufferCom, pArg)))
		return E_FAIL;

	const _ulong*	pIndices = (_ulong*)m_pVIBufferCom->Get_Indices();
	const _float3* pVerticesPos = m_pVIBufferCom->Get_VerticesPos();
	m_pVIBufferCom->Set_Property(iPropertyIndex);

	for (_int i = 0; i < 3; ++i)
	{
		CNavigationCube*						m_pNavigationCube;
		if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_NavigationCube"), LEVEL_STATIC, CubeTag, nullptr, (CGameObject**)&m_pNavigationCube)))
			return E_FAIL;

		const _float3 VertexPos = pVerticesPos[pIndices[i]];

		auto Pair = find_if(m_MapCube.begin(), m_MapCube.end(), CTagFinderWstring(MapTag.c_str()));
	
		if (Pair == m_MapCube.end())
		{
			m_pNavigationCube->Set_Pos(_float4(VertexPos.x, VertexPos.y, VertexPos.z, 1.f));
			m_vecCube.push_back(m_pNavigationCube);
			m_MapCube.emplace(MapTag, m_vecCube);
			m_vecCube.clear();
		}
		else
		{
			m_pNavigationCube->Set_Pos(_float4(VertexPos.x, VertexPos.y, VertexPos.z, 1.f));
			Pair->second.push_back(m_pNavigationCube);
		}
	}

	m_vVIBufferCom.push_back(m_pVIBufferCom);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNavigationTool::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationTool::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (m_vVIBufferCom.empty())
	{
		Safe_Release(pGameInstance);
		return S_OK;
	}
	auto Begin= m_vVIBufferCom.begin();
	auto End = m_vVIBufferCom.end();

	for (; Begin != End; ++Begin)
	{
		(*Begin)->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		(*Begin)->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		(*Begin)->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		if(false ==(*Begin)->Get_ColorCheck())
			(*Begin)->Render(0);
		else 
			(*Begin)->Render(1);
	}


	Safe_Release(pGameInstance);
	return S_OK;
}


CNavigationTool * CNavigationTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNavigationTool*	pInstance = new CNavigationTool(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CNavigationTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNavigationTool::Clone(void* pArg, _uint iMyLevelIndex)
{
	CNavigationTool*	pInstance = new CNavigationTool(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CNavigationTool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigationTool::Free()
{
	__super::Free();

	for (auto& iter : m_vVIBufferCom)
	{
		Safe_Release(iter);
	}
	m_vVIBufferCom.clear();
	vector<CVIBuffer_Navigation*>().swap(m_vVIBufferCom);

	for (auto& Pair : m_MapCube)
	{
		for (auto& iter : Pair.second)
			iter->Set_DieLayer(1);

		Pair.second.clear();
		vector<CNavigationCube*>().swap(Pair.second);
	}
	m_MapCube.clear();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);


}
