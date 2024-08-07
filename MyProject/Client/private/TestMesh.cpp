#include "stdafx.h"
#include "..\public\TestMesh.h"
#include "GameInstance.h"


CTestMesh::CTestMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestMesh::CTestMesh(const CTestMesh & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestMesh::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestMesh::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_MyLook(XMLoadFloat3(&m_pModelDesc_Client->vRotaition));
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		m_pTransformCom->Set_Scailing(10.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(200.3f, 10.0f, 147.0f, 1.f));
	}
	m_fTimeDelta = 0.0000001f;

	m_fGlowStrength = 4.f;

	return S_OK;
}

_int CTestMesh::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_fTimeAcc += (_float)(TimeDelta * 2.0f);
	if (2.f <= m_fTimeDelta)
		return 1;


	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CTestMesh::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RNDER_BULR, this)))
		return E_FAIL;


	return 0;
}

HRESULT CTestMesh::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	//if (FAILED(m_pModelCom->Render(15)))
		//return E_FAIL;

	if (FAILED(m_pModelCom->Render(17)))
		return E_FAIL;
#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

_int CTestMesh::Dead_Collecter(_double TimeDelta)
{
	return _int();
}



HRESULT CTestMesh::SetUp_Component_Load()
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

HRESULT CTestMesh::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Funch0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

HRESULT CTestMesh::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));
	
	m_pModelCom->Set_RawValue("g_glowStrength", &m_fGlowStrength, sizeof(_float));

	

	m_pModelCom->Set_RawValue("g_WaterTime", &m_fTimeDelta, sizeof(_float));

	m_fTimeDelta = m_fTimeAcc;

	Safe_Release(pGameInstance);

	return S_OK;
}

CTestMesh * CTestMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTestMesh* pInstance = new CTestMesh(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CTestMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestMesh::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTestMesh* pInstance = new CTestMesh(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CTestMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestMesh::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
