#include "stdafx.h"
#include "..\Public\Effect_Rect.h"
#include "GameInstance.h"

CEffect_Rect::CEffect_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEffect_Rect::CEffect_Rect(const CEffect_Rect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Rect::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Rect::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		ZeroMemory(&m_tRectInst, sizeof(CVIBuffer_RectInstance::RECTINST));
		memcpy(&m_tRectInst, pArg, sizeof(CVIBuffer_RectInstance::RECTINST));

		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}
	else
		return E_FAIL;

	return S_OK;
}

_int CEffect_Rect::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;


	if (true == m_bDeadCheck)
		return 1;

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CEffect_Rect::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pVIBufferCom->Update(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CEffect_Rect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render(1)))
		return E_FAIL;
	//m_tRectInst.m_iPassIndex)))

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CEffect_Rect::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_tRectInst.m_vPosition));
	XMStoreFloat4(&m_tRectInst.m_vPosition,XMVector3TransformCoord(XMLoadFloat4(&m_tRectInst.m_vPosition), m_pTransformCom->Get_WorldMatrixInverse()));

	if (0 == m_tRectInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance_CountThree"), TEXT("Com_RectInstance"), (CComponent**)&m_pVIBufferCom, &m_tRectInst)))
			return E_FAIL;
	}

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CEffect_Rect::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, m_tRectInst.m_iType);

	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_Rect * CEffect_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_Rect*	pInstance = new CEffect_Rect(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Rect::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_Rect*	pInstance = new CEffect_Rect(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Rect::Free()
{
	__super::Free();
	Safe_Release(m_pSphere);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
