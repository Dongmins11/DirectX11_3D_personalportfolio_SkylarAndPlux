#include "stdafx.h"
#include "..\public\Effect_JetPack.h"
#include "GameInstance.h"
#include "Rock.h"

CEffect_JetPack::CEffect_JetPack(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_JetPack::CEffect_JetPack(const CEffect_JetPack & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_JetPack::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_JetPack::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(121.4f, 8.0f, 143.84f, 1.f));
	}


	m_fScale = 15.f;
	m_fBackUpScale = m_fScale;

	m_pTransformCom->Set_Scailing(m_fScale);
	m_pTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));

	return S_OK;
}

_int CEffect_JetPack::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	Compute_ViewZ(m_pTransformCom);


	if (FAILED(Move_Effect(TimeDelta)))
		return -1;

	if (true == m_bDeadCheck)
		return 1;

	return 0;
}

_int CEffect_JetPack::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
		return E_FAIL;


	return 0;
}

HRESULT CEffect_JetPack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(21)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_JetPack::Move_Effect(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);

	_float4 vPos;
	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	vPos.y += 0.05f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	Safe_Release(pGameInstance);

	if (false == m_bOneCheck)
	{
		m_dVelocity += TimeDelta * 10;
		m_fScale -= m_dVelocity * TimeDelta;
		m_pTransformCom->Set_Scailing(m_fScale);
		if (0.01 >= m_fScale)
		{
			m_dVelocity = 0.0;
			m_fScale = m_fBackUpScale;
			m_bOneCheck = true;
			m_bTwoCheck = true;
		}
	}
	if (true == m_bTwoCheck)
	{
		m_dVelocity += TimeDelta * 17.0;
		m_fScale -= m_dVelocity * TimeDelta ;
		m_pTransformCom->Set_Scailing(m_fScale);
		if (0.01 >= m_fScale)
		{
			m_dVelocity = 0.0;
			m_fScale = m_fBackUpScale;
			m_bTwoCheck = false;
			m_bThreeCheck = true;
		}
	}
	if (true == m_bThreeCheck)
	{
		m_dVelocity += TimeDelta * 22.0;
		m_fScale -= m_dVelocity * TimeDelta;
		m_pTransformCom->Set_Scailing(m_fScale);
		if (0.01 >= m_fScale)
		{
			m_dVelocity = 0.0;
			m_fScale = m_fBackUpScale;
			m_bThreeCheck = false;
			m_bForCheck = true;
		}
	}
	if (true == m_bForCheck)
	{
		m_dVelocity += TimeDelta * 30.0;
		m_fScale -= m_dVelocity * TimeDelta;
		m_pTransformCom->Set_Scailing(m_fScale);
		if (0.01 >= m_fScale)
		{
			m_dVelocity = 0.0;
			m_fScale = m_fBackUpScale;
			m_bForCheck = false;
			m_bFiveCheck = true;
		}
	}
	if (true == m_bFiveCheck)
	{
		m_dVelocity += TimeDelta * 50.0;
		m_fScale -= m_dVelocity * TimeDelta;
		m_pTransformCom->Set_Scailing(m_fScale);
		if (0.01 >= m_fScale)
		{
			m_dVelocity = 0.0;
			m_fScale = m_fBackUpScale;
		}
	}
	
	return S_OK;
}

HRESULT CEffect_JetPack::SetUp_Component_Load()
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

	return S_OK;
}

HRESULT CEffect_JetPack::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_BossEffect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_JetPack::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pModelCom, 0);

	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_JetPack * CEffect_JetPack::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_JetPack* pInstance = new CEffect_JetPack(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_JetPack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_JetPack::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_JetPack* pInstance = new CEffect_JetPack(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_JetPack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_JetPack::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);


	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
