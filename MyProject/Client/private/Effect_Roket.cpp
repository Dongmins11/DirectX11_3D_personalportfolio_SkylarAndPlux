#include "stdafx.h"
#include "..\public\Effect_Roket.h"
#include "GameInstance.h"
#include "Rock.h"

CEffect_Roket::CEffect_Roket(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_Roket::CEffect_Roket(const CEffect_Roket & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Roket::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Roket::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(121.4f, 8.0f, 143.84f, 1.f));
	}

	if (LEVEL_STAGE1 == m_iMyLevel)
		m_pTransformCom->Set_Scailing(2.f);
	else
		m_pTransformCom->Set_Scailing(6.f);

	return S_OK;
}

_int CEffect_Roket::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;
	
	Compute_ViewZ(m_pTransformCom);

	BillBord_Object();

	if (FAILED(Move_Effect(TimeDelta)))
		return -1;

	if (1 == Effect_Change(TimeDelta))
		return 1;

	return 0;
}

_int CEffect_Roket::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
		return E_FAIL;


	return 0;
}

HRESULT CEffect_Roket::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(18)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Roket::BillBord_Object()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pTransformCom->Set_MyLook(pGameInstance->Get_CamPosition() - m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	Safe_Release(pGameInstance);
}

_int CEffect_Roket::Effect_Change(_double TimeDelta)
{
	m_dTimeAccFilp += TimeDelta * 2;

	if (0.03 <= m_dTimeAccFilp)
	{
		m_dTimeAccFilp = 0.0;
		++m_fU;
	}
	if (8 <= m_fU)
	{
		m_fU = 1.f;
		++m_fV;
	}
	if (8 <= m_fV)
	{
		m_fV = 1.f;
		return 1;
	}

	return 0;
}

HRESULT CEffect_Roket::Move_Effect(_double TimeDelta)
{
	_float4 vPos;
	XMStoreFloat4(&vPos,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	vPos.y += TimeDelta * 2;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&vPos));

	return S_OK;
}

HRESULT CEffect_Roket::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Roket::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_BossEffect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Roket::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pModelCom->Set_RawValue("g_U", &m_fU, sizeof(_float));
	m_pModelCom->Set_RawValue("g_V", &m_fV, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pModelCom, 1);

	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_Roket * CEffect_Roket::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_Roket* pInstance = new CEffect_Roket(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_Roket");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Roket::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_Roket* pInstance = new CEffect_Roket(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_Roket");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Roket::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);


	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
