#include "stdafx.h"
#include "..\public\NpcSphere.h"
#include "GameInstance.h"


CNpcSphere::CNpcSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CNpcSphere::CNpcSphere(const CNpcSphere & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNpcSphere::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcSphere::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component_Load()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
	}
	else
	{

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_Scailing(10.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(60.4f, 60.0f, 143.84f, 1.f));

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		MODELDESC_CLIENT tModel_Desc;
		ZeroMemory(&tModel_Desc, sizeof(MODELDESC_CLIENT));
		XMStoreFloat4(&tModel_Desc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_BossNpc"), m_iMyLevel, L"Clone_BossNpc", &tModel_Desc)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	m_fAlpha = 0.7f;

	return S_OK;
}

_int CNpcSphere::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_fTimeDelta += (_float)(TimeDelta * 0.5);
	if (50 <= m_fTimeDelta)
		m_fTimeDelta = 0;

	if (FAILED(Sphere_Dead(TimeDelta)))
		return -1;


	if (true == m_bDeadCheck)
		return 1;

	return 0;
}

_int CNpcSphere::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CNpcSphere::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(14)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcSphere::Sphere_Dead(_double TimeDelta)
{
	if (true == m_bBossEndCheck)
	{
		m_dDeadTime += TimeDelta * 0.5;

		m_fAlpha = 0.7f * (1.0 - m_dDeadTime) + 0.01f * m_dDeadTime;

		if (1.0 <= m_dDeadTime)
		{
			m_dDeadTime = 0.0;
			m_bDeadCheck = true;
		}
	}
	return S_OK;
}

HRESULT CNpcSphere::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpcSphere::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcSphere0.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpcSphere::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pModelCom->Set_RawValue("g_WaterTime", &m_fTimeDelta, sizeof(_float));


	m_pModelCom->Set_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));
	

	Safe_Release(pGameInstance);

	return S_OK;
}

CNpcSphere * CNpcSphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNpcSphere* pInstance = new CNpcSphere(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CNpcSphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNpcSphere::Clone(void* pArg, _uint iMyLevelIndex)
{
	CNpcSphere* pInstance = new CNpcSphere(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CNpcSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcSphere::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
