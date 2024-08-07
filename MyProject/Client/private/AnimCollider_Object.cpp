#include "stdafx.h"
#include "..\public\AnimCollider_Object.h"
#include "GameInstance.h"
#include "Rock.h"
#include "Camera_Fly.h"
#include "FadeInOut_UI.h"
#include "BossCRT.h"
#include "Sound_Manager.h"
CAnimCollider_Object::CAnimCollider_Object(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CAnimCollider_Object::CAnimCollider_Object(const CAnimCollider_Object & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAnimCollider_Object::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimCollider_Object::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(Setup_Component_Load()))
			return E_FAIL;


		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
		ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));
	}
	else
	{
		if (FAILED(Setup_Component()))
			return E_FAIL;

		m_pTransformCom->Set_Scailing(0.1f);
		ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));
	}




	return S_OK;
}

_int CAnimCollider_Object::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (nullptr == m_pAABB)
		return -1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());


	return _int();
}

_int CAnimCollider_Object::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Collision_Check()))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

HRESULT CAnimCollider_Object::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Setup_ConstantTable()))
		return E_FAIL;


	if (nullptr != m_pVIBufferCom)
	{
		if (FAILED(m_pVIBufferCom->Render(0)))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pAABB->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CAnimCollider_Object::Collision_Check()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CGameObject*	pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
	CGameObject*	pInOut = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_FadeInOut_Ui");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere || nullptr == pInOut)
		return -1;

	if (false == m_bCamCheck)
	{
		if (true == pTargetSphere->Collision_AABBXYZ(m_pAABB,
			&m_tPlayerCollisionDesc.bCollisionX, &m_tPlayerCollisionDesc.bCollisionY, &m_tPlayerCollisionDesc.bCollisionZ,
			&m_tPlayerCollisionDesc.fDisX, &m_tPlayerCollisionDesc.fDisY, &m_tPlayerCollisionDesc.fDisZ))
		{
			if (LEVEL_BOSS_STAGE != m_iMyLevel)
			{
				dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
				dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(4);
				m_bCamCheck = true;
			}
			else
			{
				CGameObject*	pBossCrt = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Boss_CRT");
				CSound_Manager::Get_Instance()->PlayBGM(L"Stage_BossOne", 1.f, CSound_Manager::ID_BOSS_STAGE_BGM);
				CSound_Manager::Get_Instance()->PlaySound(L"BossVoice", CSound_Manager::ID_BOSS_STAGE_VOICE);
				
				if (nullptr == pBossCrt)
				{
					Safe_Release(pGameInstance);
					return E_FAIL;
				}
				dynamic_cast<CBossCRT*>(pBossCrt)->Set_RenderCheck(true);
				dynamic_cast<CBossCRT*>(pBossCrt)->Set_AnimCheck(true);
				dynamic_cast<CFadeInOut_UI*>(pInOut)->Set_ShutCheck_Open(true);
				dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
				dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(5);
				m_bCamCheck = true;
			}
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CAnimCollider_Object::Setup_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	/* Com_VIBuffer_Cube */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimCollider_Object::Setup_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 3.f, 0.0f);
	ColliderDesc.vExtents = _float3(100.f, 30.f, 100.f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CAnimCollider_Object::Setup_ConstantTable()
{
	return S_OK;
}

CAnimCollider_Object * CAnimCollider_Object::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CAnimCollider_Object*	pInstance = new CAnimCollider_Object(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CAnimCollider_Object");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CAnimCollider_Object::Clone(void* pArg, _uint iMyLevelIndex)
{
	CAnimCollider_Object*	pInstance = new CAnimCollider_Object(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CAnimCollider_Object");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimCollider_Object::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pAABB);
}
