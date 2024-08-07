#include "stdafx.h"
#include "..\Public\Effect_Point.h"
#include "GameInstance.h"

CEffect_Point::CEffect_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEffect_Point::CEffect_Point(const CEffect_Point & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Point::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Point::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		ZeroMemory(&m_tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
		memcpy(&m_tPointInst, pArg, sizeof(CVIBuffer_PointInstance::POINTINST));

		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}
	else
		return E_FAIL;

	m_fU = 1.f;
	m_fV = 1.f;
	m_fGlowStength = 5.f;

	m_vRectRandom = _float4((rand() % 255) / 255.f, (rand() % 225) / 255.f, (rand() % 200) / 255.f, 0.f);

	return S_OK;
}

_int CEffect_Point::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (5 == m_tPointInst.m_iType)
	{
		if (false == m_bFirstCheck)
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);
			m_pRightTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Right_Bullet", L"Com_Transform", 0, 0);
			Safe_AddRef(m_pRightTargetTransform);

			Safe_Release(pGameInstance);
			m_bFirstCheck = true;
		}
	}
	if (true == m_bDeadCheck)
	{
		m_bDeadCheck = false;
		return 1;
	}

	m_dTimeAcc += TimeDelta;

	if (3 == m_tPointInst.m_iType || 4 == m_tPointInst.m_iType)
	{
	/*	if (0.8 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			return 1;
		}*/
		if (1 == Filp_Anim(TimeDelta))
			return 1;
	}
	else if(5 == m_tPointInst.m_iType)
	{
		if (true == m_bDeadCheck)
		{
			m_bDeadCheck = false;
			return 1;
		}
	}
	else if(7 == m_tPointInst.m_iType)
	{
		if (0.7 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			return 1;
		}
	}
	else
	{
		if (1.5 <= m_dTimeAcc)
		{
			m_dTimeAcc = 0.0;
			return 1;
		}
	}
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	m_fTimeAcc += (_float)TimeDelta;

	return 0;
}

_int CEffect_Point::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (3 == m_tPointInst.m_iType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform= (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Boss_CRT", L"Com_LeftTransform", 0, 0);

		Safe_Release(pGameInstance);
		m_pLeftTransformCom->Set_WolrdMatirx_float4x4(m_tPointInst.m_Matrix);
		m_pVIBufferCom->Update(TimeDelta, pTargetTransform);
	}
	else if(4 == m_tPointInst.m_iType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Boss_CRT", L"Com_RightTransform", 0, 0);

		Safe_Release(pGameInstance);

		m_pRightTransformCom->Set_WolrdMatirx_float4x4(m_tPointInst.m_Matrix);
		m_pVIBufferCom->Update(TimeDelta, pTargetTransform);
	}
	else if (5 == m_tPointInst.m_iType)
	{
		if (nullptr != m_pRightTargetTransform)
		{
			m_pVIBufferCom->Update(TimeDelta * 20, m_pTransformCom, m_pRightTargetTransform);
		}
	}
	else if(6 == m_tPointInst.m_iType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);

		Safe_Release(pGameInstance);
		m_pVIBufferCom->Update(TimeDelta, pTargetTransform);
	}
	else
		m_pVIBufferCom->Update(TimeDelta, m_pTransformCom);


	if(7 == m_tPointInst.m_iType)
		m_pRendererCom->Add_RenderGroup(CRenderer::RNDER_BULR, this);
	else
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CEffect_Point::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	if (3 == m_tPointInst.m_iType || 4 == m_tPointInst.m_iType)
	{
		if (FAILED(m_pVIBufferCom->Render(2)))
			return E_FAIL;
	}
	else if(5 == m_tPointInst.m_iType)
	{
		if (FAILED(m_pVIBufferCom->Render(0)))
			return E_FAIL;
	}
	else if(6 == m_tPointInst.m_iType)
	{
		if (FAILED(m_pVIBufferCom->Render(3)))
			return E_FAIL;
	}
	else if(7 ==  m_tPointInst.m_iType)
	{
		if (FAILED(m_pVIBufferCom->Render(4)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pVIBufferCom->Render(0)))
			return E_FAIL;
	}

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);


#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

_int CEffect_Point::Filp_Anim(_double TimeDelta)
{
	m_dTimeAccFilp += TimeDelta * 2;

	if (0.03 <= m_dTimeAccFilp)
	{
		m_dTimeAccFilp = 0.0;
		++m_fU;
	}
	if (9 <= m_fU)
	{
		m_fU = 1.f;
		++m_fV;
	}
	if (9 <= m_fV)
	{
		m_fV = 1.f;
		return 1;
	}


	return 0;
}

HRESULT CEffect_Point::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_tPointInst.m_vPosition));
	XMStoreFloat4(&m_tPointInst.m_vPosition, XMVector3TransformCoord(XMLoadFloat4(&m_tPointInst.m_vPosition), m_pTransformCom->Get_WorldMatrixInverse()));

	if (0 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Cloud"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (1 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_PetCloud"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (3 == m_tPointInst.m_iType)
	{
		/* Com.Transform */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftTransform"), (CComponent**)&m_pLeftTransformCom)))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_SmokeOne"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		
	}
	else if (4 == m_tPointInst.m_iType)
	{
		/* Com.Transform */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightTransform"), (CComponent**)&m_pRightTransformCom)))
			return E_FAIL;

		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_SmokeTwo"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (5 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Cloud"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (6 == m_tPointInst.m_iType)
	{
		/* Com_VIBuffer */
		if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Landing"), TEXT("Com_PointInstance"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
			return E_FAIL;

		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (7 == m_tPointInst.m_iType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (-1 == __super::Get_ComponentSize(LEVEL_STATIC,L"Prototype_Component_VIBuffer_PointInstance_Effect_One",L"Clone_InstanceOne"))
		{
			/* Com_VIBuffer */
			if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_One"), TEXT("Clone_InstanceOne"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
				return E_FAIL;
		}
		else if (-1 == __super::Get_ComponentSize(LEVEL_STATIC, L"Prototype_Component_VIBuffer_PointInstance_Effect_One", L"Clone_InstanceTwo"))
		{
			/* Com_VIBuffer */
			if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Two"), TEXT("Clone_InstanceTwo"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
				return E_FAIL;
		}
		else if (-1 == __super::Get_ComponentSize(LEVEL_STATIC, L"Prototype_Component_VIBuffer_PointInstance_Effect_One", L"Clone_InstanceThree"))
		{
			/* Com_VIBuffer */
			if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Three"), TEXT("Clone_InstanceThree"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
				return E_FAIL;
		}
		else if(-1 == __super::Get_ComponentSize(LEVEL_STATIC, L"Prototype_Component_VIBuffer_PointInstance_Effect_One", L"Clone_InstanceFor"))
		{
			/* Com_VIBuffer */
			if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_For"), TEXT("Clone_InstanceFor"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
				return E_FAIL;
		}
		else
		{
			/* Com_VIBuffer */
			if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Five"), TEXT("Clone_InstanceFive"), (CComponent**)&m_pVIBufferCom, &m_tPointInst)))
				return E_FAIL;
		}


		/* Com_Texture */
		if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Point::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	if (FAILED(m_pVIBufferCom->Set_RawValue("g_TimeDeltaAcc", &m_fTimeAcc, sizeof(_float))))
		return E_FAIL;

	m_pVIBufferCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_vector));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pVIBufferCom, m_tPointInst.m_iTextureIndex);

	if (3 == m_tPointInst.m_iType || 4 == m_tPointInst.m_iType)
	{
		m_pVIBufferCom->Set_RawValue("g_U", &m_fU, sizeof(_float));
		m_pVIBufferCom->Set_RawValue("g_V", &m_fV, sizeof(_float));
	}
	if (7 == m_tPointInst.m_iType)
	{
		if (FAILED(m_pVIBufferCom->Set_RawValue("g_RectColor", &m_vRectRandom, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Set_RawValue("g_glowStrength", &m_fGlowStength, sizeof(_float))))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_Point * CEffect_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_Point*	pInstance = new CEffect_Point(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Point::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_Point*	pInstance = new CEffect_Point(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CEffect_Point");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Point::Free()
{
	__super::Free();

	if (nullptr != m_pLeftTransformCom)
		Safe_Release(m_pLeftTransformCom);
	if (nullptr != m_pRightTransformCom)
		Safe_Release(m_pRightTransformCom);
	Safe_Release(m_pRightTargetTransform);
	Safe_Release(m_pSphere);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);


}
