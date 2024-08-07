#include "stdafx.h"
#include "..\Public\MapObject.h"
#include "GameInstance.h"

CMapObject::CMapObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMapObject::CMapObject(const CMapObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMapObject::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
	}
	else
		return E_FAIL;

	return S_OK;
}

_int CMapObject::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;
	//m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CMapObject::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CMapObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(0);

//#ifdef _DEBUG
//	m_pNavigationCom->Render(0);
//#endif // _DEBUG

	return S_OK;
}

HRESULT CMapObject::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 7.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMapObject::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CMapObject * CMapObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMapObject*	pInstance = new CMapObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMapObject::Clone(void* pArg, _uint iMyLevelIndex)
{
	CMapObject*	pInstance = new CMapObject(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Delete(m_pModelDesc_Client);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);


}
