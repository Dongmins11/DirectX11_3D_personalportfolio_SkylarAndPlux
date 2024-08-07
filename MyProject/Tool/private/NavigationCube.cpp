#include "stdafx.h"
#include "..\Public\NavigationCube.h"

#include "GameInstance.h"

#include "MainFrm.h"
#include "MyForm2.h"
#include "NaviTool.h"
#include "Floor_Tool.h"



CNavigationCube::CNavigationCube(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CNavigationCube::CNavigationCube(const CNavigationCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNavigationCube::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationCube::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scailing(1.5f);

	return S_OK;
}

_int CNavigationCube::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (1 == m_iDeadNumber)
		return 1;

	return 0;
}

_int CNavigationCube::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CNavigationCube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if(false == m_bIsColorCheck)
		m_pVIBufferCom->Render(4);
	else
		m_pVIBufferCom->Render(5);
	return S_OK;
}

HRESULT CNavigationCube::Set_Pos(_float4 vPos)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f));

	return S_OK;
}

HRESULT CNavigationCube::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationCube::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CNavigationCube * CNavigationCube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNavigationCube*	pInstance = new CNavigationCube(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CNavigationCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNavigationCube::Clone(void* pArg, _uint iMyLevelIndex)
{
	CNavigationCube*	pInstance = new CNavigationCube(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CNavigationCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigationCube::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);


}
