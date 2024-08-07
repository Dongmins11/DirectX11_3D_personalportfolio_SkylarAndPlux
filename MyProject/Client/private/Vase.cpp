#include "stdafx.h"
#include "..\public\Vase.h"
#include "GameInstance.h"
#include "Sound_Manager.h"

CVase::CVase(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CVase::CVase(const CVase & rhs)
	: CGameObject(rhs)
{
}

HRESULT CVase::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVase::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		//m_pTransformCom->Set_Scailing(0.7f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 14.f, 80.f, 1.f));
	}

	return S_OK;
}

_int CVase::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CVase::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (1 == Dead_Collecter(TimeDelta))
	{
		return 1;
	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	return 0;
}

HRESULT CVase::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

_int CVase::Dead_Collecter(_double TimeDelta)
{
	
	CGameInstance * pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_HandSphere"), 0, 0);
	if (nullptr == pTargetSphere)
	{
		Safe_Release(pGameInstance);
		return -1;
	}


	if (false == pTargetSphere->Get_CheckCollision() && true == m_pSphere->Collision_Sphere(pTargetSphere))
	{
			for (_uint i = 0; i < 3; ++i)
			{
				MODELDESC_CLIENT tModelDesc;
				ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));

				XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				tModelDesc.vRotaition = _float3(1.f, 1.f, 1.f);
				tModelDesc.vScaile = 1.f;

				if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Collector"), m_iMyLevel, L"Clone_RopeRobot", &tModelDesc)))
					return E_FAIL;
			}
			Safe_Release(pGameInstance);
			
			CSound_Manager::Get_Instance()->Check_Sound(L"VaseOne", L"VaseTwo", L"VaseThree", CSound_Manager::ID_OBJECT_SOUND_ONE, CSound_Manager::ID_OBJECT_SOUND_TWO, CSound_Manager::ID_OBJECT_SOUND_THREE);
			return 1;
	}


	Safe_Release(pGameInstance);


	return _int();
}



HRESULT CVase::SetUp_Component_Load()
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

HRESULT CVase::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Vase0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

HRESULT CVase::SetUp_ConstantTable()
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

CVase * CVase::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVase* pInstance = new CVase(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CVase");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CVase::Clone(void* pArg, _uint iMyLevelIndex)
{
	CVase* pInstance = new CVase(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CVase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVase::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
