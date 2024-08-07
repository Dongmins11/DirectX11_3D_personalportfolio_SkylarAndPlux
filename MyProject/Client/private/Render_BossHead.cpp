#include "stdafx.h"
#include "..\public\Render_BossHead.h"
#include "GameInstance.h"
#include "FadeInOut_UI.h"

CRender_BossHead::CRender_BossHead(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRender_BossHead::CRender_BossHead(const CRender_BossHead & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRender_BossHead::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_BossHead::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

_int CRender_BossHead::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (false == m_bFristCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_BossPet"), LEVEL_STAGE2, L"Clone_BossPet")))
			return E_FAIL;

		Safe_Release(pGameInstance);
		m_bFristCheck = true;
	}


	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());
	
	if (FAILED(Animation_Boss(TimeDelta)))
		return -1;

	return 0;
}

_int CRender_BossHead::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	return 0;
}

HRESULT CRender_BossHead::Render()
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

HRESULT CRender_BossHead::Animation_Boss(_double TimeDelta)
{
	if (true == m_bAnimCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"BossPotal0.fbx", L"Com_Transform", 0, 0);

		m_dTimeAcc += TimeDelta * 0.01;
		_float4 BossPos;
		XMStoreFloat4(&BossPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
		BossPos.z = BossPos.z + 10.f;
		_vector vBossPos = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&BossPos), m_dTimeAcc);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vBossPos);
		if (0.03 <= m_dTimeAcc)
		{
			CGameObject* pGameObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_FadeInOut_Ui");
			if (nullptr == pGameObject)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			dynamic_cast<CFadeInOut_UI*>(pGameObject)->Set_Value(1);
			dynamic_cast<CFadeInOut_UI*>(pGameObject)->Set_FadeCheck(true);
			m_dTimeAcc = 0.0;
			m_bAnimCheck = false;
		}

		Safe_Release(pGameInstance);
	}




	return S_OK;
}



HRESULT CRender_BossHead::SetUp_Component_Load()
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

HRESULT CRender_BossHead::SetUp_Component()
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

HRESULT CRender_BossHead::SetUp_ConstantTable()
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

CRender_BossHead * CRender_BossHead::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRender_BossHead* pInstance = new CRender_BossHead(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CRender_BossHead");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRender_BossHead::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRender_BossHead* pInstance = new CRender_BossHead(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CRender_BossHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRender_BossHead::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
