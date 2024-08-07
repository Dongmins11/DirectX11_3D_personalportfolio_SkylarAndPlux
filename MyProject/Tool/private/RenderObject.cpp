#include "stdafx.h"
#include "..\Public\RenderObject.h"

#include "GameInstance.h"

CRenderObject::CRenderObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_bIsClone(false)
{
}

CRenderObject::CRenderObject(const CRenderObject & rhs)
	: CGameObject(rhs)
	, m_bIsClone(true)
{
}

HRESULT CRenderObject::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderObject::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;


	m_pModelDesc = new MODELDESC;
	ZeroMemory(m_pModelDesc, sizeof(MODELDESC));
	if (FAILED(SetUp_Component()))
		return E_FAIL;


	return S_OK;
}

_int CRenderObject::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;


	return 0;
}

_int CRenderObject::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);
	
	return 0;
}

HRESULT CRenderObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetUp_ConstantTable();

	if(nullptr != m_pVIBufferCom)
		Safe_Release(m_pVIBufferCom);

	return S_OK;
}

HRESULT CRenderObject::Set_Pos()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc->vPos));

	return S_OK;
}

HRESULT CRenderObject::Set_Scaile()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Scailing(m_pModelDesc->vScaile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderObject::Set_ScaileXYZ()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ScailingXYZ(m_pModelDesc->vScaileXYZ)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderObject::Set_Rotation()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Rotation(m_pModelDesc->vRotaition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderObject::Set_ModelDesc(MODELDESC * pModelDesc)
{
	if (nullptr == pModelDesc)
		return E_FAIL;
	MODELDESC tModelDesc;
	ZeroMemory(&tModelDesc, sizeof(MODELDESC));

	tModelDesc = *pModelDesc;
	*m_pModelDesc = tModelDesc;
	
	return S_OK;
}

HRESULT CRenderObject::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderObject::SetUp_ConstantTable()
{
	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc->wstrPath.c_str(), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (nullptr != m_pVIBufferCom)
	{
		Set_Pos();
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		Safe_Release(pGameInstance);

		m_pVIBufferCom->Render(0);
	}


	return S_OK;
}

CRenderObject * CRenderObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRenderObject*	pInstance = new CRenderObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRenderObject::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRenderObject*	pInstance = new CRenderObject(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderObject::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		Safe_Delete(m_pModelDesc);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);


}
