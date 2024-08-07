#include "stdafx.h"
#include "..\public\Effect_Flip.h"
#include "GameInstance.h"
#include "Rock.h"


CEffect_Flip::CEffect_Flip(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_Flip::CEffect_Flip(const CEffect_Flip & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Flip::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Flip::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 16.f, 106.f, 1.f));
		m_pTransformCom->Set_Scailing(5.0f);

	}

	m_fU = 1.f;
	m_fV = 1.f;

	return S_OK;
}

_int CEffect_Flip::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);
	
	if (nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}
	_vector TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	_vector MyLook = TargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float4 Look;
	XMStoreFloat4(&Look, MyLook);
	Look.y = 0.f;
	m_pTransformCom->Set_MyLook(-XMLoadFloat4(&Look));
	m_dTimeAcc += TimeDelta;

	Safe_Release(pGameInstance);


	if (FAILED(Set_VecCollider()))
		return -1;

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CEffect_Flip::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Change_UV(TimeDelta)))
		return E_FAIL;

	Collision_Check();

	return 0;
}

HRESULT CEffect_Flip::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render(7)))
		return E_FAIL;


#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

void CEffect_Flip::Set_Position(_fvector vPos)
{
	if (nullptr == m_pTransformCom)
		return;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

HRESULT CEffect_Flip::Change_UV(_double TimeDelta)
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_dTimeAcc += TimeDelta;

	if (0.05 <= m_dTimeAcc)
	{
		m_dTimeAcc = 0.0;
		++m_iIndex;
	}
	if (0 == m_iIndex)
	{
		m_fU = 1.f;
	}
	if (1 == m_iIndex)
	{
		m_fU = 2.f;
	}
	if (2 == m_iIndex)
	{
		m_fU = 3.f;
	}
	if (3 == m_iIndex)
	{
		m_fU = 4.f;
		++m_fV;
		m_iIndex = 0;
	}

	if (4 <= m_fV)
	{
		m_fV = 1.f;
	}

	return S_OK;
}

HRESULT CEffect_Flip::Set_VecCollider()
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

void CEffect_Flip::Collision_Check()
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
			if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
				return;
		}
	}
}



HRESULT CEffect_Flip::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.2f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 1.2f, 4.7f);
	ColliderDesc.fRadius = 3.f;

	return S_OK;
}

HRESULT CEffect_Flip::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect_Filp"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.2f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 1.2f, 4.7f);
	ColliderDesc.fRadius = 0.5f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Flip::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBuffer->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pVIBuffer->Set_RawValue("g_U",&m_fU, sizeof(_float));
	m_pVIBuffer->Set_RawValue("g_V",&m_fV, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBuffer, 0);

	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_Flip * CEffect_Flip::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_Flip* pInstance = new CEffect_Flip(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_Flip");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Flip::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_Flip* pInstance = new CEffect_Flip(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_Flip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Flip::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		if (nullptr != m_pModelDesc_Client)
		{
			Safe_Delete(m_pModelDesc_Client);
		}
	}

	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
