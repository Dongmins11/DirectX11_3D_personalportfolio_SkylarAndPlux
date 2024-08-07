#include "stdafx.h"
#include "..\Public\Arrow.h"

#include "GameInstance.h"

CArrow::CArrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_bIsClone(false)
{
}

CArrow::CArrow(const CArrow & rhs)
	: CGameObject(rhs)
	, m_bIsClone(true)
{
}

HRESULT CArrow::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArrow::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scailing(5.f);

	return S_OK;
}

_int CArrow::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_fRotY += 1;

	if (m_fRotY > 360)
		m_fRotY = 0;

	m_vRotation = _float3(0.f, m_fRotY, 0.f);

	return 0;
}

_int CArrow::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	m_pTransformCom->Set_Rotation(m_vRotation);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CArrow::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(3);

	return S_OK;
}

HRESULT CArrow::Set_Pos(_float4 vPos)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y + 5, vPos.z, vPos.w));

	return S_OK;
}

HRESULT CArrow::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Arrow"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CArrow::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, 0);

	Safe_Release(pGameInstance);
	return S_OK;
}

CArrow * CArrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CArrow*	pInstance = new CArrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArrow::Clone(void* pArg, _uint iMyLevelIndex)
{
	CArrow*	pInstance = new CArrow(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CFloor_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArrow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);


}
