#include "stdafx.h"
#include "..\public\RotationBall.h"
#include "GameInstance.h"
#include "Rock.h"
#include <random>

CRotationBall::CRotationBall(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRotationBall::CRotationBall(const CRotationBall & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRotationBall::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRotationBall::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		//m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		//m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Scailing(1.6f);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}

	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int CRotationBall::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;
	
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CRotationBall::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Ball_Check(TimeDelta)))
		return E_FAIL;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CRotationBall::Render()
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


HRESULT CRotationBall::Ball_Check(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
	CTransform*		pHoleTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_BallHole", L"Com_Transform", 0, 0);
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Magenetic_Skill"), TEXT("Com_Sphere"), 0, 0);
	CColider*		pHoleTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_BallHole"), TEXT("Com_Sphere"), 0, 0);

	if (nullptr == pTargetTransform || nullptr == pTargetSphere || nullptr == pHoleTransform || nullptr == pHoleTargetSphere)
		return E_FAIL;
	

		if (false == m_pSphere->Collision_Sphere(pHoleTargetSphere))
		{
			if (false == pTargetSphere->Get_CheckCollision() && true == m_pSphere->Collision_Sphere(pTargetSphere))
			{
				_float3 vDis = _float3(0.f, 8.f, 7.f);
				++m_fRotZ;
				m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, m_fRotZ));
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(XMLoadFloat3(&vDis), pTargetTransform->Get_WorldMatrix()));
			}
		}
		else
		{
			if (false == m_bBallInCheck)
			{
				m_dTimeAcc += TimeDelta * 0.5;
				_vector Pos = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMVectorSet(234.9f, 60.6f, 326.7f, 1.f), m_dTimeAcc);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);
				if (1.0 <= m_dTimeAcc)
				{
					m_dTimeAcc = 0.0;
					m_bSkillCheckTwo = true;
					m_bBallInCheck = true;
					m_bSkillCheck = true;
				}
			}
		}

	if (true == m_bSkillCheck)
	{
		m_dTimeAcc += TimeDelta * 0.5;
		_vector Pos = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMVectorSet(234.9f, 46.6f, 326.7f, 1.f), m_dTimeAcc);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);
		if (1.0 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			m_bSkillCheck = false;
			m_bBallIn = true;
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CRotationBall::Ball_Animation(_double TimeDelta)
{
	return E_NOTIMPL;
}


HRESULT CRotationBall::SetUp_Component_Load()
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
	ColliderDesc.vExtents = _float3(2.f, 0.3f, 2.f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRotationBall::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RotationBall0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.1f, 0.0f);
	ColliderDesc.vExtents = _float3(2.f, 0.3f, 2.f);
	ColliderDesc.fRadius = 2.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRotationBall::SetUp_ConstantTable()
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

CRotationBall * CRotationBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRotationBall* pInstance = new CRotationBall(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CRotationBall");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRotationBall::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRotationBall* pInstance = new CRotationBall(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CRotationBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRotationBall::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pSphere);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
