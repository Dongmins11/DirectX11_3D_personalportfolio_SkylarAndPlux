#include "stdafx.h"
#include "..\Public\PunchRock.h"
#include "GameInstance.h"
#include "Client_AnimationDefine.h"
#include "Rock.h"
#include "Turret_Bullet.h"

CPunchRock::CPunchRock(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CPunchRock::CPunchRock(const CPunchRock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPunchRock::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPunchRock::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

		m_vStartValue = 4.f;
		m_vBlendValue = 4.f;
		m_vEndValue = 5.f;
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		m_pTransformCom->Set_Scailing(10.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(235.f, 16.f, 93.f, 1.f));

		m_vStartValue = 9.f;
		m_vBlendValue = 9.f;
		m_vEndValue = 10.f;
	}

	m_dBlendTime = 5.f;
	m_iHitIndex = 0;
	m_iCurMotion = 1;

	m_fRatio = 0.f;

	return S_OK;
}

_int CPunchRock::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pHitSphere->Update(m_pTransformCom->Get_WorldMatrix());

	if (4 == m_iHitIndex)
		m_bDissolveCheck = true;
	
	
	return 0;
}

_int CPunchRock::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (FAILED(Hit_Monster(TimeDelta)))
	{
		m_bDissolveCheck = true;
	}
	if (true == m_bDissolveCheck)
	{
		m_fRatio += TimeDelta;
		if (1.0f <= m_fRatio)
		{
			m_fRatio = 1.f;
			return 1;
		}
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CPunchRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(19);

#ifdef _DEBUG
	m_pHitSphere->Render();
#endif // _DEBUG



	return S_OK;
}

HRESULT CPunchRock::Hit_Monster(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_HandSphere"), 0, 0);
	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere || nullptr == pTargetTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	if (false == pTargetSphere->Get_CheckCollision() && m_pHitSphere->Collision_Sphere(pTargetSphere))
	{
		if (false == m_bCheckCollision)
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_OBJECT_HIT);
			CSound_Manager::Get_Instance()->PlaySound(L"ObjectHit", CSound_Manager::ID_OBJECT_HIT);

			m_iScaleIndex = 0;
			m_dBlendTime = 5.f;
			m_vBlendValue = 4.f;
			m_vStartValue = 4.f;
			m_vEndValue = 5.f;
			m_iScaleIndex = 0;
			m_vTimeAcc = 0.0;
			m_pHitSphere->Set_CheckCollision(true);
			m_bCheckCollision = true;
			m_bMotionCheck = true;
		}
		++m_iHitIndex;
	}


	if (true == m_bCheckCollision)
	{
		m_dCollisionTimeAcc += TimeDelta; 

		if (FAILED(Hit_Motion(TimeDelta)))
			return E_FAIL;
		if (ATTACKMOTION == dynamic_cast<CRock*>(pPlayerObject)->Get_CurrentAnimation())
		{
			if (0.5f <= m_dCollisionTimeAcc)
			{
				m_dCollisionTimeAcc = 0.0;
				m_pHitSphere->Set_CheckCollision(false);
				m_bCheckCollision = false;
			}
		}
		//else
		//{
		//	m_dCollisionTimeAcc = 0.0;
		//	m_pHitSphere->Set_CheckCollision(false);
		//	m_bCheckCollision = false;
		//}
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CPunchRock::Hit_Motion(_double TimeDelta)
{
	if (true == m_bMotionCheck)
	{
		m_vTimeAcc += TimeDelta * m_dBlendTime;

		_double dSize = m_vStartValue * (1.0 - m_vTimeAcc) + m_vEndValue * m_vTimeAcc;
		//m_pTransformCom->Set_ScailingXYZ(_float3((_float)dSize, 1.f, (_float)dSize));
		m_pTransformCom->Set_Scailing((_float)dSize);
		if (1.0 <= m_vTimeAcc)
		{
			_uint iScaleIndex = m_iScaleIndex % 2;


			if (0 == iScaleIndex)
			{
				m_vStartValue = 5.f;
				m_vEndValue = m_vBlendValue;

			}
			else
			{
				m_vStartValue = m_vEndValue;
				m_vEndValue = 5.f;

				//m_vBlendValue += 2.f;
			}

			m_vBlendValue += 0.48f;
			m_dBlendTime += 6.f;
			m_vTimeAcc = 0.0;
			++m_iScaleIndex;


			if (m_iScaleIndex == 5)
			{
				m_dBlendTime = 5.f;
				m_vBlendValue = 4.f;
				m_vStartValue = 4.f;
				//m_vEndValue = m_pModelDesc_Client->vScaileXYZ.x;
				m_vEndValue = 5.f;
				m_iScaleIndex = 0;
				m_bMotionCheck = false;
			}
		}
	}

	return S_OK;
}

HRESULT CPunchRock::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, L"PunchRock0.fbx", TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(1.f, 1.f, 1.f);
	ColliderDesc.fRadius = 0.8f;


	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_HitSphere"), (CComponent**)&m_pHitSphere, &ColliderDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Dissolve"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPunchRock::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));
	
	m_pVIBufferCom->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float));
	m_pTextureCom->SetUp_OnShader("g_DissolveTexture", m_pVIBufferCom, 0);
	m_pTextureCom->SetUp_OnShader("g_DissolveColorTexture", m_pVIBufferCom, 1);

	m_pVIBufferCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_vector));
	
	Safe_Release(pGameInstance);
	return S_OK;
}

CPunchRock * CPunchRock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPunchRock*	pInstance = new CPunchRock(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CPunchRock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPunchRock::Clone(void* pArg, _uint iMyLevelIndex)
{
	CPunchRock*	pInstance = new CPunchRock(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CPunchRock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPunchRock::Free()
{
	__super::Free();
	Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pAABB);
	Safe_Release(m_pSphere);
	Safe_Release(m_pHitSphere);
	Safe_Release(m_pTransformCom);

}
