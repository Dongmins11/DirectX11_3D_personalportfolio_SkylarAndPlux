#include "stdafx.h"
#include "..\Public\Collector_Point.h"
#include "GameInstance.h"

CCollector_Point::CCollector_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CCollector_Point::CCollector_Point(const CCollector_Point & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCollector_Point::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollector_Point::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		ZeroMemory(&m_tPointInst, sizeof(CVIBuffer_PointInstance_Clone::POINTINST));
		memcpy(&m_tPointInst, pArg, sizeof(CVIBuffer_PointInstance_Clone::POINTINST));

		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}

_int CCollector_Point::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_dTimeAcc += TimeDelta * 0.5;
	m_dScale = 0.5f * (1.0 - m_dTimeAcc) + 0.05f * m_dTimeAcc;

	if (1.0 <= m_dTimeAcc)
	{
		m_dTimeAcc = 0.0;
		return 1;
	}

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CCollector_Point::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pVIBufferCom->Update(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CCollector_Point::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(2)))
		return E_FAIL;

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);


#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CCollector_Point::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_tPointInst.m_vPosition));
	XMStoreFloat4(&m_tPointInst.m_vPosition, XMVector3TransformCoord(XMLoadFloat4(&m_tPointInst.m_vPosition), m_pTransformCom->Get_WorldMatrixInverse()));

	if (3 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Clone_Ten"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}
	else if (4 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Two"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}
	else if (5 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Three"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}
	else if (6 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_For"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}
	else if (7 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Five"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}
	else if (8 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Six"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;
	}


	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect_Png"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollector_Point::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pVIBufferCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_vector));
	m_pVIBufferCom->Set_RawValue("g_Size", &m_dScale, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 0);

	Safe_Release(pGameInstance);

	return S_OK;
}

CCollector_Point * CCollector_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCollector_Point*	pInstance = new CCollector_Point(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CCollector_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCollector_Point::Clone(void* pArg, _uint iMyLevelIndex)
{
	CCollector_Point*	pInstance = new CCollector_Point(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CCollector_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollector_Point::Free()
{
	__super::Free();

	Safe_Release(m_pSphere);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
