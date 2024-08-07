#include "stdafx.h"
#include "..\public\TimeRenderObject.h"
#include "GameInstance.h"



CTimeRenderObject::CTimeRenderObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTimeRenderObject::CTimeRenderObject(const CTimeRenderObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTimeRenderObject::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimeRenderObject::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 14.f, 86.f, 1.f));
	}

	return S_OK;
}

_int CTimeRenderObject::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Set_VecCollider()))
		return -1;

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CTimeRenderObject::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	Collision_Check();

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_TimeSphere"), TEXT("Com_Sphere"), 0, 0);
	if (nullptr == pTargetSphere)
	{
		Safe_Release(pGameInstance);
		return -1;
	}
	Safe_Release(pGameInstance);


	if (true == m_pSphere->Collision_Sphere(pTargetSphere))
	{
		return 0;
	}
	else
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
			return E_FAIL;
	}

	return 0;
}

HRESULT CTimeRenderObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pModelCom->Render(0);

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTimeRenderObject::Set_VecCollider()
{
	if (false == m_SetVector)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		_uint iIndex = pGameInstance->Get_GameObjectSize(m_iMyLevel, L"Clone_TimeSphere");

		if (0 >= iIndex)
		{
			Safe_Release(pGameInstance);
			m_SetVector = true;
			return E_FAIL;
		}

		for (_uint i = 0; i < iIndex; ++i)
		{
			CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_TimeSphere"), TEXT("Com_Sphere"), i, 0);

			if (nullptr == pTargetSphere)
			{
				Safe_Release(pGameInstance);
				m_SetVector = true;
				return E_FAIL;
			}

			Safe_AddRef(pTargetSphere);
			m_vecTargetCollider.push_back(pTargetSphere);
		}

		Safe_Release(pGameInstance);

		m_SetVector = true;
	}

	return S_OK;
}

void CTimeRenderObject::Collision_Check()
{
	if (!m_vecTargetCollider.empty())
	{
		for (auto iter : m_vecTargetCollider)
		{
			if (true == m_pSphere->Collision_Sphere(iter))
			{
				m_bRenderCheck = true;
				break;
			}
			else
				m_bRenderCheck = false;

		}
		if (false == m_bRenderCheck)
		{
			if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
				return;
		}
	}

}



HRESULT CTimeRenderObject::SetUp_Component_Load()
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

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.0f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTimeRenderObject::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TempleHead0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
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

HRESULT CTimeRenderObject::SetUp_ConstantTable()
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

CTimeRenderObject * CTimeRenderObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTimeRenderObject* pInstance = new CTimeRenderObject(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CTimeRenderObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTimeRenderObject::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTimeRenderObject* pInstance = new CTimeRenderObject(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CTimeRenderObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimeRenderObject::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	for (auto iter : m_vecTargetCollider)
		Safe_Release(iter);

	m_vecTargetCollider.swap(vector<CColider*>());

	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);

}
