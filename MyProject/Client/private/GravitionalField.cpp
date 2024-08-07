#include "stdafx.h"
#include "..\public\GravitionalField.h"
#include "GameInstance.h"
#include "GravityMesh.h"
#include "Camera_Fly.h"

CGravitionalField::CGravitionalField(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CGravitionalField::CGravitionalField(const CGravitionalField & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGravitionalField::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGravitionalField::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(234.9f, 76.6f, 326.7f, 1.f));
	m_pTransformCom->Set_Scailing(1.7f);
	m_pTransformCom->Set_ScaleY(20.f);


	return S_OK;
}

_int CGravitionalField::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Gravity_Coord(TimeDelta)))
		  return -1;

	if (true == m_bRenderCheck)
	{
		m_fTimer += (_float)(TimeDelta * 0.1f);

		if (0.6 <= m_fTimer)
		{
			m_bReadyCheck = true;
			m_fTimer = 0.6;

			if (false == m_bFirstCheck)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);
				CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");

				if (nullptr == pCam)
				{
					Safe_Release(pGameInstance);
					return -1;
				}

				dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(3);
				Safe_Release(pGameInstance);

				m_bFirstCheck = true;
			}
		}
	}


	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CGravitionalField::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (true == m_bRenderCheck)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
			return E_FAIL;
	}


	return 0;
}

HRESULT CGravitionalField::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (true == m_bRenderCheck)
	{

		if (FAILED(SetUp_ConstantTable()))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(12)))
			return E_FAIL;

#ifdef _DEBUG
		m_pSphere->Render();
		m_pAABB->Render();
#endif // _DEBUG
	}

	return S_OK;
}

HRESULT CGravitionalField::Gravity_Coord(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"GravityMesh0.fbx", L"Com_Transform", 0, 0);
	if (nullptr == pTargetTransform)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), pTargetTransform->Get_WorldMatrix()));

	Safe_Release(pGameInstance);
	return S_OK;
}




HRESULT CGravitionalField::SetUp_Component_Load()
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


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.0f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGravitionalField::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("GravityMesh1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 0.5f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(0.7f, 10.f, 0.7f);

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGravitionalField::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pModelCom->Set_RawValue("g_TimerUV", &m_fTimer, sizeof(_float));

	Safe_Release(pGameInstance);

	return S_OK;
}

CGravitionalField * CGravitionalField::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGravitionalField* pInstance = new CGravitionalField(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CGravitionalField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGravitionalField::Clone(void* pArg, _uint iMyLevelIndex)
{
	CGravitionalField* pInstance = new CGravitionalField(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CGravitionalField");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravitionalField::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pAABB);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
