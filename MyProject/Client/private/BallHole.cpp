#include "stdafx.h"
#include "..\public\BallHole.h"
#include "GameInstance.h"
#include "Rock.h"
#include "RotationBall.h"
#include "GravitionalField.h"
#include "Camera_Fly.h"
#include <random>

CBallHole::CBallHole(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBallHole::CBallHole(const CBallHole & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBallHole::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBallHole::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;


	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scailing(0.4f);
	m_pTransformCom->Set_ScailingXYZ(_float3(0.9f, 0.9f, 1.1f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(234.9f, 46.6f, 326.7f, 1.f));
	m_pTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));

	m_pLTTransformCom->Set_Scailing(3.f);
	m_pLUTransformCom->Set_Scailing(3.f);
	m_pRUTransformCom->Set_Scailing(3.f);
	m_pLDTransformCom->Set_Scailing(3.f);
	m_pRDTransformCom->Set_Scailing(3.f);

	m_pLTTransformCom->Set_ScailingXYZ(_float3(1.f, 1.f, 1.1f));
	m_pLUTransformCom->Set_ScailingXYZ(_float3(1.f, 1.f, 1.1f));
	m_pRUTransformCom->Set_ScailingXYZ(_float3(1.f, 1.f, 1.1f));
	m_pLDTransformCom->Set_ScailingXYZ(_float3(1.f, 1.f, 1.1f));
	m_pRDTransformCom->Set_ScailingXYZ(_float3(1.f, 1.f, 1.1f));

	////////////////////LT//////////////

	m_pLTTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(231.2f, 46.6f, 321.8f, 1.f));
	m_pLTTransformCom->Set_Rotation(_float3(180.f, 218.f, 0.f));


	////////////////////LU//////////////

	m_pLUTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(238.8f, 46.6f, 321.6f, 1.f));
	m_pLUTransformCom->Set_Rotation(_float3(180.f, 145.f, 0.f));

	////////////////////RU//////////////

	m_pRUTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(241.f, 46.6f, 328.9f, 1.f));
	m_pRUTransformCom->Set_Rotation(_float3(180.f, 70.f, 0.f));

	////////////////////LD//////////////

	m_pLDTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(228.8f, 46.6f, 328.7f, 1.f));
	m_pLDTransformCom->Set_Rotation(_float3(180.f, 287.f, 0.f));

	////////////////////RD//////////////

	m_pRDTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(235.f, 46.6f, 333.2f, 1.f));
	m_pRDTransformCom->Set_Rotation(_float3(180.f, 0.f, 0.f));





	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int CBallHole::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Main_Animation(TimeDelta)))
		return -1;

	if (FAILED(Player_Collision_Check(TimeDelta)))
		return E_FAIL;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());



	return 0;
}

_int CBallHole::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;



	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CBallHole::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	m_pModelCom->Render(0);
	m_pLTModelCom->Render(0);
	m_pLUModelCom->Render(0);
	m_pRUModelCom->Render(0);
	m_pLDModelCom->Render(0);
	m_pRDModelCom->Render(0);


#ifdef _DEBUG
	m_pAABB->Render();
	m_pSphereCom->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CBallHole::Main_Animation(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform*		pHoleTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_BallHole", L"Com_Transform", 0, 0);
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Magenetic_Skill"), TEXT("Com_Sphere"), 0, 0);
	CGameObject*	pTarget = (CRotationBall*)pGameInstance->Get_GameObject(m_iMyLevel, L"RotationBall0.fbx");
	if (nullptr == pHoleTransform || nullptr == pTargetSphere || nullptr == pTarget)
		return E_FAIL;
	if (false == dynamic_cast<CRotationBall*>(pTarget)->Get_BallInCheck())
	{
		if (false == pTargetSphere->Get_CheckCollision() && true == m_pSphereCom->Collision_Sphere(pTargetSphere))
		{
			if (false == m_bChangeCheck)
			{
				if (FAILED(Rotation_Change_First(TimeDelta)))
					return E_FAIL;
			}
		}
		else
		{
			if (true == m_bChangeCheck)
			{
				if (FAILED(Rotation_Change_Second(TimeDelta)))
					return E_FAIL;
			}
		}
	}
	else
	{
		if (FAILED(Rotation_Change_Third(TimeDelta)))
			return E_FAIL;
	}
	


	

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBallHole::Rotation_Change_First(_double TimeDelta)
{
				
	m_dTimeAcc += TimeDelta * 0.5;
	_float RotX = 180 * (1.0 - m_dTimeAcc) + 260 * m_dTimeAcc;

	m_pLTTransformCom->Set_Rotation(_float3(RotX, 218.f, 0.f));
	m_pLUTransformCom->Set_Rotation(_float3(RotX, 145.f, 0.f));
	m_pRUTransformCom->Set_Rotation(_float3(RotX, 70.f, 0.f));
	m_pLDTransformCom->Set_Rotation(_float3(RotX, 287.f, 0.f));
	m_pRDTransformCom->Set_Rotation(_float3(RotX, 0.f, 0.f));

	if (1.0 <= m_dTimeAcc)
	{
		m_dTimeAcc = 0.0;
		m_bChangeCheck = true;
	}

	return S_OK;
}

HRESULT CBallHole::Rotation_Change_Second(_double TimeDelta)
{
	m_dTimeAcc += TimeDelta * 0.5;

	_float RotX = 260 * (1.0 - m_dTimeAcc) + 180 * m_dTimeAcc;

	m_pLTTransformCom->Set_Rotation(_float3(RotX, 218.f, 0.f));
	m_pLUTransformCom->Set_Rotation(_float3(RotX, 145.f, 0.f));
	m_pRUTransformCom->Set_Rotation(_float3(RotX, 70.f, 0.f));
	m_pLDTransformCom->Set_Rotation(_float3(RotX, 287.f, 0.f));
	m_pRDTransformCom->Set_Rotation(_float3(RotX, 0.f, 0.f));

	if (1.0 <= m_dTimeAcc)
	{
		m_dTimeAcc = 0.0;
		m_bChangeCheck = false;
	}

	return S_OK;
}

HRESULT CBallHole::Rotation_Change_Third(_double TimeDelta)
{
	if (false == m_bThirdCheck)
	{
		if (false == m_bSoundCheck)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_OBJECT_SOUND);
			CSound_Manager::Get_Instance()->PlaySound(L"StageTwoObject",CSound_Manager::ID_OBJECT_SOUND);
			m_bSoundCheck = true;
		}
		m_dCheckTime += TimeDelta * 0.2;

		_float RotX = 260 * (1.0 - m_dCheckTime) + 180 * m_dCheckTime;

		m_pLTTransformCom->Set_Rotation(_float3(RotX, 218.f, 0.f));
		m_pLUTransformCom->Set_Rotation(_float3(RotX, 145.f, 0.f));
		m_pRUTransformCom->Set_Rotation(_float3(RotX, 70.f, 0.f));
		m_pLDTransformCom->Set_Rotation(_float3(RotX, 287.f, 0.f));
		m_pRDTransformCom->Set_Rotation(_float3(RotX, 0.f, 0.f));

		if (1.0 <= m_dCheckTime)
		{
			m_dCheckTime = 0.0;
			m_bThirdCheck = true;

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CGameObject * pGameObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Gravity_Field");
			CGameObject * pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
			if (nullptr == pGameObject || nullptr == pCam)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			dynamic_cast<CGravitionalField*>(pGameObject)->Set_RenderCheck(true);
			dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
			dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(2);
			Safe_Release(pGameInstance);
		}
	}

	return S_OK;
}

HRESULT CBallHole::Player_Collision_Check(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
		return -1;

	if (true == pTargetSphere->Collision_AABBXYZ(m_pAABB,
		&m_tPlayerCollisionDesc.bCollisionX, &m_tPlayerCollisionDesc.bCollisionY, &m_tPlayerCollisionDesc.bCollisionZ,
		&m_tPlayerCollisionDesc.fDisX, &m_tPlayerCollisionDesc.fDisY, &m_tPlayerCollisionDesc.fDisZ))
	{
		m_tPlayerCollisionDesc.bMainCollision = true;
		dynamic_cast<CRock*>(pPlayerObject)->Set_PlayerCollision(m_tPlayerCollisionDesc);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CBallHole::SetUp_Component_Load()
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
	ColliderDesc.vExtents = _float3(4.f, 1.f, 4.f);
	ColliderDesc.fRadius = 500.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBallHole::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/////////////////////For.Model////////////////////

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole1.fbx"), TEXT("Com_MainModel"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole0.fbx"), TEXT("Com_OneModel"), (CComponent**)&m_pLTModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole2.fbx"), TEXT("Com_TwoModel"), (CComponent**)&m_pLUModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole3.fbx"), TEXT("Com_ThreeModel"), (CComponent**)&m_pRUModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole4.fbx"), TEXT("Com_ForModel"), (CComponent**)&m_pLDModelCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("BallHole5.fbx"), TEXT("Com_FiveModel"), (CComponent**)&m_pRDModelCom)))
		return E_FAIL;


	/////////////////////For.Transform////////////////////

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_OneTransform"), (CComponent**)&m_pLUTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_TwoTransform"), (CComponent**)&m_pRUTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ThreeTransform"), (CComponent**)&m_pLDTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ForTransform"), (CComponent**)&m_pRDTransformCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_FiveTransform"), (CComponent**)&m_pLTTransformCom)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, .0f);
	ColliderDesc.vExtents = _float3(15.3f, 1.5f, 13.f);
	ColliderDesc.fRadius = 7.f;


	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vCenter = _float3(0.0f, 0.f, -10.f);

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* Com_Sphere */
	return S_OK;
}

HRESULT CBallHole::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pLTModelCom->Set_RawValue("g_WorldMatrix", &m_pLTTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLTModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLTModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pLUModelCom->Set_RawValue("g_WorldMatrix", &m_pLUTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLUModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLUModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pRUModelCom->Set_RawValue("g_WorldMatrix", &m_pRUTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRUModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRUModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pLDModelCom->Set_RawValue("g_WorldMatrix", &m_pLDTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLDModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLDModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pRDModelCom->Set_RawValue("g_WorldMatrix", &m_pRDTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRDModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRDModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	Safe_Release(pGameInstance);

	return S_OK;
}

CBallHole * CBallHole::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBallHole* pInstance = new CBallHole(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CBallHole");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBallHole::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBallHole* pInstance = new CBallHole(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CBallHole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBallHole::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pAABB);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pLTTransformCom);
	Safe_Release(m_pLUTransformCom);
	Safe_Release(m_pRUTransformCom);
	Safe_Release(m_pLDTransformCom);
	Safe_Release(m_pRDTransformCom);

	Safe_Release(m_pLTModelCom);
	Safe_Release(m_pLUModelCom);
	Safe_Release(m_pRUModelCom);
	Safe_Release(m_pLDModelCom);
	Safe_Release(m_pRDModelCom);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);


}
