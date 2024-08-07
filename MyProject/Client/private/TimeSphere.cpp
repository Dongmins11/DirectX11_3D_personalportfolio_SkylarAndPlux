#include "stdafx.h"
#include "..\public\TimeSphere.h"
#include "GameInstance.h"
#include "Defferde_TimeSphere.h"

CTimeSphere::CTimeSphere(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTimeSphere::CTimeSphere(const CTimeSphere & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTimeSphere::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeSphere::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Deffered_TimeSphere"), m_iMyLevel, L"Clone_TimeSphere",nullptr,(CGameObject**)&m_pDefferd_TimeSphere)))
	//	return E_FAIL;

	//dynamic_cast<CDefferde_TimeSphere*>(m_pDefferd_TimeSphere)->Set_WolrdMat(m_pTransformCom->Get_WorldFloat4x4());

	Safe_Release(pGameInstance);
	return S_OK;
}

_int CTimeSphere::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Change_Scale(TimeDelta)))
		return E_FAIL;

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	//dynamic_cast<CDefferde_TimeSphere*>(m_pDefferd_TimeSphere)->Set_WolrdMat(m_pTransformCom->Get_WorldFloat4x4());

	return 0;
}

_int CTimeSphere::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CTimeSphere::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pModelCom->Render(8);

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

void CTimeSphere::Set_WolrdMat(_float4x4 WorldMat)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Set_WolrdMatirx_float4x4(WorldMat);
	m_pTransformCom->Set_Scailing(0.01f);
}

void CTimeSphere::Set_Position(_fvector vPos)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

HRESULT CTimeSphere::Change_Scale(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (true == m_bKeyCheck)
	{
		if (false == m_bScaleCheck)
		{
			m_dTimeAcc += TimeDelta * 6.0;
			_float Scale = 0.01f * (1.0 - m_dTimeAcc) + 200.f * m_dTimeAcc;
			m_pTransformCom->Set_Scailing(Scale);

			if (1.0 <= m_dTimeAcc)
			{
				m_dTimeAcc = 0.0;
				m_bScaleCheck = true;
				m_bKeyCheck = false;
			}
		}
		else
		{
			m_dTimeAcc += TimeDelta * 6.0;
			_float Scale = 200.f * (1.0 - m_dTimeAcc) + 0.01f * m_dTimeAcc;
			m_pTransformCom->Set_Scailing(Scale);

			if (1.0 <= m_dTimeAcc)
			{
				m_dTimeAcc = 0.0;
				m_bScaleCheck = false;
				m_bKeyCheck = false;
			}
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CTimeSphere::Set_VecCollider()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	//m_vecTargetCollider.push_back()
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CTimeSphere::Collision_Check()
{
	return E_NOTIMPL;
}


HRESULT CTimeSphere::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal5.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 0.1f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeSphere::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal5.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 0.14f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeSphere::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));



	Safe_Release(pGameInstance);

	return S_OK;
}

CTimeSphere * CTimeSphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTimeSphere* pInstance = new CTimeSphere(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CTimeSphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTimeSphere::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTimeSphere* pInstance = new CTimeSphere(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CTimeSphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimeSphere::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pDefferd_TimeSphere);
	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
