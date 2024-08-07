#include "stdafx.h"
#include "..\public\Effect_FireCrt.h"
#include "GameInstance.h"
#include "Rock.h"

CEffect_FireCrt::CEffect_FireCrt(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_FireCrt::CEffect_FireCrt(const CEffect_FireCrt & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_FireCrt::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_FireCrt::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

		if (FAILED(SetUp_Component_Buffer()))
			return E_FAIL;
	}

	return S_OK;
}

_int CEffect_FireCrt::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	return 0;
}

_int CEffect_FireCrt::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (true == m_bDeadCheck)
		return 1;

	if (FAILED(Move_Effect(TimeDelta)))
		return -1;

	if (1 == Effect_ChangeTwo(TimeDelta))
		return 1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return E_FAIL;


	return 0;
}

HRESULT CEffect_FireCrt::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (true == m_bEffectRenderCheck)
	{
		if (FAILED(SetUp_ConstantTable_Buffer()))
			return E_FAIL;
	}

	return S_OK;
}

void CEffect_FireCrt::BillBord_Object()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pTransformCom->Set_MyLook(pGameInstance->Get_CamPosition() - m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	Safe_Release(pGameInstance);
}

_int CEffect_FireCrt::Effect_Change(_double TimeDelta)
{
	m_dTimeAccFilp += TimeDelta * 2;

	if (0.01 <= m_dTimeAccFilp)
	{
		m_dTimeAccFilp = 0.0;
		++m_fU;
	}
	if (8 <= m_fU)
	{
		m_fU = 1.f;
		++m_fV;
	}
	m_fScale += TimeDelta * 10;
	m_pTransformCom->Set_Scailing(m_fScale);

	if (8 <= m_fV)
	{
		m_fV = 1.f;
		return 1;
	}




	return 0;
}

_int CEffect_FireCrt::Effect_ChangeTwo(_double TimeDelta)
{
	m_dTimeAccFilp += TimeDelta * 2;

	if (0.03 <= m_dTimeAccFilp)
	{
		m_dTimeAccFilp = 0.0;
		++m_fU;
	}
	if (5 <= m_fU)
	{
		m_fU = 1.f;
		++m_fV;
	}
	if (3 <= m_fV)
	{
		m_fV = 1.f;
	}

	return 0;
}

HRESULT CEffect_FireCrt::Move_Effect(_double TimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Boss_CRT", L"Com_Transform", 0, 0);
	if (nullptr == pTargetTransform)
	{
		MSGBOX("Failed Finder Transform Effect_FireCrt");
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	Safe_Release(pGameInstance);
	_float4 vPos;
	_vector Dir = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	_vector Right = pTargetTransform->Get_State(CTransform::STATE_RIGHT);
	_float3 vDir;
	XMStoreFloat3(&vDir, Dir);
	XMVector3Normalize(Dir);
	vDir.y = 0;
	XMVector3Normalize(Right);


	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) -= (Dir * 0.5));
	XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) -= (Right * 0.6));
	vPos.y -= 14.f;
	m_pLeftUpTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	m_pLeftUpTransformCom->Set_MyLook(-XMLoadFloat3(&vDir));
	m_pLeftUpTransformCom->Rotation_Turn(m_pLeftUpTransformCom->Get_State(CTransform::STATE_LOOK), 30.f);

	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) -= (Dir * 0.5));
	XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) += (Right * 0.6));
	vPos.y -= 14.f;
	m_pRightUpTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	m_pRightUpTransformCom->Set_MyLook(-XMLoadFloat3(&vDir));
	m_pRightUpTransformCom->Rotation_Turn(m_pRightUpTransformCom->Get_State(CTransform::STATE_LOOK), 330.f);


	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&vPos, XMLoadFloat4(&vPos) += (Dir * 0.55));
	vPos.y -= 14.f;
	m_pLeftDownTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	m_pLeftDownTransformCom->Set_MyLook(-XMLoadFloat3(&vDir));
	m_pLeftDownTransformCom->Rotation_Turn(m_pLeftDownTransformCom->Get_State(CTransform::STATE_RIGHT), 30.f);


	return S_OK;
}

HRESULT CEffect_FireCrt::SetUp_Component_Load()
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

HRESULT CEffect_FireCrt::SetUp_Component()
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

HRESULT CEffect_FireCrt::SetUp_ConstantTable()
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

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pModelCom, 4);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CEffect_FireCrt::SetUp_Component_Buffer()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_LeftUpVIBuffer"), (CComponent**)&m_pLeftUpBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_RightUpVIBuffer"), (CComponent**)&m_pRightUpBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_LeftDownVIBuffer"), (CComponent**)&m_pLeftDownBuffer)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_RightDownVIBuffer"), (CComponent**)&m_pRightDownBuffer)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftUpTransform"), (CComponent**)&m_pLeftUpTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pLeftUpTransformCom->Set_Rotation(_float3(0.f, 0.f, 120.f));
	m_pLeftUpTransformCom->Set_ScailingXYZTwo(_float3(4.f, 9.f, 4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftDownTransform"), (CComponent**)&m_pLeftDownTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pLeftDownTransformCom->Set_Rotation(_float3(0.f, 0.f, 180.f));
	m_pLeftDownTransformCom->Set_ScailingXYZTwo(_float3(4.f, 9.f, 4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightUpTransform"), (CComponent**)&m_pRightUpTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pRightUpTransformCom->Set_Rotation(_float3(0.f, 0.f, 120.f));
	m_pRightUpTransformCom->Set_ScailingXYZTwo(_float3(4.f, 9.f, 4.f));
	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightDownTransform"), (CComponent**)&m_pRightDownTransformCom, &TransformDesc)))
		return E_FAIL;
	m_pRightDownTransformCom->Set_Rotation(_float3(0.f, 0.f, 180.f));


	return S_OK;
}

HRESULT CEffect_FireCrt::SetUp_ConstantTable_Buffer()
{
	if (true == m_bEffectRenderCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		m_pLeftUpBuffer->Set_RawValue("g_WorldMatrix", &m_pLeftUpTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pLeftUpBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pLeftUpBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		m_pLeftUpBuffer->Set_RawValue("g_U", &m_fU, sizeof(_float));
		m_pLeftUpBuffer->Set_RawValue("g_V", &m_fV, sizeof(_float));

		m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pLeftUpBuffer, 5);

		if (FAILED(m_pLeftUpBuffer->Render(23)))
			return E_FAIL;

		m_pLeftDownBuffer->Set_RawValue("g_WorldMatrix", &m_pLeftDownTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pLeftDownBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pLeftDownBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		m_pLeftDownBuffer->Set_RawValue("g_U", &m_fU, sizeof(_float));
		m_pLeftDownBuffer->Set_RawValue("g_V", &m_fV, sizeof(_float));

		m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pLeftDownBuffer, 5);

		if (FAILED(m_pLeftDownBuffer->Render(23)))
			return E_FAIL;


		m_pRightUpBuffer->Set_RawValue("g_WorldMatrix", &m_pRightUpTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pRightUpBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pRightUpBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		m_pRightUpBuffer->Set_RawValue("g_U", &m_fU, sizeof(_float));
		m_pRightUpBuffer->Set_RawValue("g_V", &m_fV, sizeof(_float));

		m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pRightUpBuffer, 5);

		if (FAILED(m_pRightUpBuffer->Render(23)))
			return E_FAIL;

		m_pRightDownBuffer->Set_RawValue("g_WorldMatrix", &m_pRightDownTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
		m_pRightDownBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		m_pRightDownBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		m_pRightDownBuffer->Set_RawValue("g_U", &m_fU, sizeof(_float));
		m_pRightDownBuffer->Set_RawValue("g_V", &m_fV, sizeof(_float));

		m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pRightDownBuffer, 5);

		if (FAILED(m_pRightDownBuffer->Render(23)))
			return E_FAIL;
		Safe_Release(pGameInstance);
	}
	return S_OK;
}

CEffect_FireCrt * CEffect_FireCrt::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_FireCrt* pInstance = new CEffect_FireCrt(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_FireCrt");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_FireCrt::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_FireCrt* pInstance = new CEffect_FireCrt(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_FireCrt");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FireCrt::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pLeftDownBuffer);
	Safe_Release(m_pLeftDownTransformCom);

	Safe_Release(m_pRightDownBuffer);
	Safe_Release(m_pRightDownTransformCom);

	Safe_Release(m_pRightUpBuffer);
	Safe_Release(m_pRightUpTransformCom);

	Safe_Release(m_pLeftUpBuffer);
	Safe_Release(m_pLeftUpTransformCom);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
