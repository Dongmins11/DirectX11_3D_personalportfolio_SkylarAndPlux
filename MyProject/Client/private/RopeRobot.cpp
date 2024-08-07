#include "stdafx.h"
#include "..\public\RopeRobot.h"
#include "GameInstance.h"
#include "Rock.h"


CRopeRobot::CRopeRobot(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRopeRobot::CRopeRobot(const CRopeRobot & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRopeRobot::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRopeRobot::Native_Construct(void* pArg, _uint iMyLevelIndex)
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


		m_pRopeTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pRopeTransformCom->Set_ScailingXYZ(_float3(0.5f,1.f,0.5f));
		m_pSphereTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 23.f, 146.f, 1.f));
		m_pRopeTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 23.f, 146.f, 1.f));
		m_pRopeTransformCom->Set_Scailing(1.f);
		m_pSphereTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 23.f, 146.f, 1.f));
	}


	return S_OK;
}

_int CRopeRobot::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;



	m_MatRopeWorldMatrix = m_pSphereTransform->Get_WorldFloat4x4();

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CRopeRobot::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(Rope_Swing(TimeDelta)))
		return E_FAIL;

	if (FAILED(Rope_Look(TimeDelta)))
		return E_FAIL;


	if (FAILED(Rope_CollisionCheck()))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CRopeRobot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pSphereModelCom->Render(0)))
		return E_FAIL;

	if (true == m_bIsRenderCheck)
	{
		if (FAILED(m_pRopeModelCom->Render(0)))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG


	return S_OK;
}



_matrix CRopeRobot::Get_RopeWolrdMatrix()
{
	if (nullptr == m_pSphereTransform)
		return XMMatrixIdentity();

	return XMLoadFloat4x4(&m_MatRopeWorldMatrix);
}

HRESULT CRopeRobot::Rope_Swing(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_HandSphere"), 0, 0);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);

	if (nullptr == pPlayerObject || nullptr == pTargetSphere || nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);

	if (true == m_bIsRenderCheck)
	{
		if (false == m_bRotationCheck)
		{
			m_pSphereTransform->Turn(m_pSphereTransform->Get_State(CTransform::STATE_RIGHT), -TimeDelta);
		}
		else
		{
			m_pSphereTransform->Turn(m_pSphereTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta);
		}
	}
	else
	{
		m_pSphereTransform->Set_State(CTransform::STATE_RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f));
		m_pSphereTransform->Set_State(CTransform::STATE_UP, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		m_pSphereTransform->Set_State(CTransform::STATE_LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	}

	return S_OK;
}

HRESULT CRopeRobot::Rope_Look(_double TimeDelta)
{
	//랜더 킬때 보여주는것이다 bool조건받아서 이거를 똑같이 해주자

	if (true == m_bIsRenderCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");

		if (nullptr == pPlayerObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		Safe_Release(pGameInstance);

		_vector MyLook = dynamic_cast<CRock*>(pPlayerObject)->Get_Player_ColiderMat().r[3] - m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		_vector	vLookLength = dynamic_cast<CRock*>(pPlayerObject)->Get_Player_ColiderMat().r[3] - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float	fLength = XMVectorGetX(XMVector3Length(vLookLength));

		m_pRopeTransformCom->Set_MyLook(-MyLook);
		m_pRopeTransformCom->Set_ScaleZ(fLength * 0.5f);
	}

	return S_OK;
}

HRESULT CRopeRobot::Rope_CollisionCheck()
{
	if (nullptr == m_pSphere)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);

	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);


	if (true == m_pSphere->Collision_Sphere(pTargetSphere))
	{
		m_bIsRopeCollisionCheck = true;
		dynamic_cast<CRock*>(pPlayerObject)->Set_RopeObject(this);
	}


	return S_OK;
}

HRESULT CRopeRobot::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_SphereModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot1.fbx"), TEXT("Com_SphereModel"), (CComponent**)&m_pSphereModelCom)))
		return E_FAIL;

	/* Com_RopeModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot3.fbx"), TEXT("Com_RopeModel"), (CComponent**)&m_pRopeModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_SphereTransform"), (CComponent**)&m_pSphereTransform, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RopeTransform"), (CComponent**)&m_pRopeTransformCom, &TransformDesc)))
		return E_FAIL;




	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.5f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 20.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRopeRobot::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_SphereModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot1.fbx"), TEXT("Com_SphereModel"), (CComponent**)&m_pSphereModelCom)))
		return E_FAIL;

	/* Com_RopeModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot3.fbx"), TEXT("Com_RopeModel"), (CComponent**)&m_pRopeModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_SphereTransform"), (CComponent**)&m_pSphereTransform, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RopeTransform"), (CComponent**)&m_pRopeTransformCom, &TransformDesc)))
		return E_FAIL;
	

	

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.5f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 20.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRopeRobot::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pSphereModelCom->Set_RawValue("g_WorldMatrix", &m_pSphereTransform->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pSphereModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pSphereModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pRopeModelCom->Set_RawValue("g_WorldMatrix", &m_pRopeTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRopeModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRopeModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);

	return S_OK;
}

CRopeRobot * CRopeRobot::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRopeRobot* pInstance = new CRopeRobot(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CRopeRobot");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRopeRobot::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRopeRobot* pInstance = new CRopeRobot(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CRopeRobot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRopeRobot::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		if (nullptr != m_pModelDesc_Client)
		{
			Safe_Delete(m_pModelDesc_Client);
		}
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);

	Safe_Release(m_pRopeModelCom);
	Safe_Release(m_pSphereModelCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pRopeTransformCom);
	Safe_Release(m_pSphereTransform);
	Safe_Release(m_pTransformCom);
}
