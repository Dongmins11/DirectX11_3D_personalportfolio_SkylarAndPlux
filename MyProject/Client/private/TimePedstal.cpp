#include "stdafx.h"
#include "..\public\TimePedstal.h"
#include "GameInstance.h"
#include "Rock.h"
#include "TimeSphere.h"
#include "Sound_Manager.h"

CTimePedstal::CTimePedstal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTimePedstal::CTimePedstal(const CTimePedstal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTimePedstal::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTimePedstal::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTransformCom->Set_Scailing(5.f);
		//m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);

		m_pOneTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pTwoTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pThreeTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pForTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());

		m_vSourcePosition = m_pModelDesc_Client->vPos;
		m_vOneSourcePosition = m_vSourcePosition;
		m_vOneSourcePosition.y += 5.f;
		m_pForTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vOneSourcePosition));

		m_vDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 0.5f, m_vSourcePosition.z, 1.f);
		m_vOneDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 0.8f, m_vSourcePosition.z, 1.f);
		m_vTwoDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 1.2f, m_vSourcePosition.z, 1.f);
		m_vThreeDestPosition = _float4(m_vOneSourcePosition.x, m_vOneSourcePosition.y + 1.5f, m_vOneSourcePosition.z, 1.f);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 14.f, 80.f, 1.f));
		m_pTransformCom->Set_Scailing(5.f);
		m_pOneTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pTwoTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pThreeTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pForTransformCom->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pForTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 19.f, 80.f, 1.f));


		m_vSourcePosition = _float4(233.f, 14.f, 80.f, 1.f);
		m_vOneSourcePosition = _float4(233.f, 19.f, 80.f, 1.f);
		m_vDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 0.5f, m_vSourcePosition.z, 1.f);
		m_vOneDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 0.8f, m_vSourcePosition.z, 1.f);
		m_vTwoDestPosition = _float4(m_vSourcePosition.x, m_vSourcePosition.y + 1.2f, m_vSourcePosition.z, 1.f);
		m_vThreeDestPosition = _float4(m_vOneSourcePosition.x, m_vOneSourcePosition.y + 1.5f, m_vOneSourcePosition.z, 1.f);
	}



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_TimeSphere"), m_iMyLevel, L"Clone_TimeSphere",nullptr, (CGameObject**)&m_pTimeSphere)))
		return E_FAIL;

	if (nullptr == m_pTimeSphere)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	m_pTimeSphere->Set_WolrdMat(m_pForTransformCom->Get_WorldFloat4x4());

	Safe_Release(pGameInstance);
	


	return S_OK;
}

_int CTimePedstal::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (true == m_bKeyCheck)
	{
		m_dTimeAcc += TimeDelta * 400;
		m_pOneTransformCom->Set_Rotation(_float3(0.f, m_dTimeAcc, 0.f));
		m_pTwoTransformCom->Set_Rotation(_float3(0.f, -m_dTimeAcc, 0.f));
		m_pThreeTransformCom->Set_Rotation(_float3(0.f, m_dTimeAcc, 0.f));
	}

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CTimePedstal::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(Collision_Check(TimeDelta)))
		return -1;

	m_pTimeSphere->Set_Position(m_pForTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CTimePedstal::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pOneModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pTwoModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pThreeModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pForModelCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG
	return S_OK;
}

HRESULT CTimePedstal::Collision_Check(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);


	if (true == m_pSphere->Collision_Sphere(pTargetSphere))
	{
		if (false == m_bPosCheck)
		{
			m_dAnimTimeAcc += TimeDelta * 3;
			_vector vPos = XMVectorLerp(XMLoadFloat4(&m_vSourcePosition), XMLoadFloat4(&m_vDestPosition), m_dAnimTimeAcc);
			_vector vPosOne = XMVectorLerp(XMLoadFloat4(&m_vSourcePosition), XMLoadFloat4(&m_vOneDestPosition), m_dAnimTimeAcc);
			_vector vPosTwo = XMVectorLerp(XMLoadFloat4(&m_vSourcePosition), XMLoadFloat4(&m_vTwoDestPosition), m_dAnimTimeAcc);
			_vector vPosThree = XMVectorLerp(XMLoadFloat4(&m_vOneSourcePosition), XMLoadFloat4(&m_vThreeDestPosition), m_dAnimTimeAcc);

			m_pOneTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			m_pTwoTransformCom->Set_State(CTransform::STATE_POSITION, vPosOne);
			m_pThreeTransformCom->Set_State(CTransform::STATE_POSITION, vPosTwo);
			m_pForTransformCom->Set_State(CTransform::STATE_POSITION, vPosThree);

			if (1.0 <= m_dAnimTimeAcc)
			{
 				m_bPosCheck = true;
				m_dAnimTimeAcc = 0.0;
			}
		}
		if (pGameInstance->Key_Down('F'))
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_OBJECT_SOUND);
			CSound_Manager::Get_Instance()->PlaySound(L"TimePad", CSound_Manager::ID_OBJECT_SOUND);

			m_bKeyCheck = true;
			m_pTimeSphere->Set_KeyCheck(m_bKeyCheck);
		}
	}
	else
	{
		if (true == m_bPosCheck)
		{
			m_dAnimTimeAcc += TimeDelta * 3;
			_vector vPos = XMVectorLerp(XMLoadFloat4(&m_vDestPosition), XMLoadFloat4(&m_vSourcePosition), m_dAnimTimeAcc);
			_vector vPosOne = XMVectorLerp(XMLoadFloat4(&m_vOneDestPosition), XMLoadFloat4(&m_vSourcePosition), m_dAnimTimeAcc);
			_vector vPosTwo = XMVectorLerp(XMLoadFloat4(&m_vTwoDestPosition), XMLoadFloat4(&m_vSourcePosition), m_dAnimTimeAcc);
			_vector vPosThree = XMVectorLerp(XMLoadFloat4(&m_vThreeDestPosition), XMLoadFloat4(&m_vOneSourcePosition), m_dAnimTimeAcc);
			m_pOneTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			m_pTwoTransformCom->Set_State(CTransform::STATE_POSITION, vPosOne);
			m_pThreeTransformCom->Set_State(CTransform::STATE_POSITION, vPosTwo);
			m_pForTransformCom->Set_State(CTransform::STATE_POSITION, vPosThree);

			if (1.0 <= m_dAnimTimeAcc)
			{
				m_bPosCheck = false;
				m_dAnimTimeAcc = 0.0;
			}
			m_bKeyCheck = false;
			m_pOneTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
			m_pTwoTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
			m_pThreeTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CTimePedstal::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal1.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pOneModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal2.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pTwoModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal3.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pThreeModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_OneTransform"), (CComponent**)&m_pOneTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_TwoTransform"), (CComponent**)&m_pTwoTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ThreeTransform"), (CComponent**)&m_pThreeTransformCom)))
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

HRESULT CTimePedstal::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal0.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal1.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pOneModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal2.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pTwoModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal3.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pThreeModelCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("TimePedstal5.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pForModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_OneTransform"), (CComponent**)&m_pOneTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_TwoTransform"), (CComponent**)&m_pTwoTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ThreeTransform"), (CComponent**)&m_pThreeTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ThreeTransform"), (CComponent**)&m_pForTransformCom)))
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

HRESULT CTimePedstal::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pOneModelCom->Set_RawValue("g_WorldMatrix", &m_pOneTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pOneModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pOneModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pTwoModelCom->Set_RawValue("g_WorldMatrix", &m_pTwoTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pTwoModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pTwoModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pThreeModelCom->Set_RawValue("g_WorldMatrix", &m_pThreeTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pThreeModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pThreeModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pForModelCom->Set_RawValue("g_WorldMatrix", &m_pForTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pForModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pForModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	Safe_Release(pGameInstance);

	return S_OK;
}

CTimePedstal * CTimePedstal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTimePedstal* pInstance = new CTimePedstal(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CTimePedstal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTimePedstal::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTimePedstal* pInstance = new CTimePedstal(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CTimePedstal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimePedstal::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pOneModelCom);
	Safe_Release(m_pTwoModelCom);
	Safe_Release(m_pThreeModelCom);
	Safe_Release(m_pForModelCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pOneTransformCom);
	Safe_Release(m_pTwoTransformCom);
	Safe_Release(m_pThreeTransformCom);
	Safe_Release(m_pForTransformCom);

	Safe_Release(m_pTimeSphere);
}
