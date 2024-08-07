#include "stdafx.h"
#include "..\public\Effect_SwingAttack.h"
#include "GameInstance.h"
#include "Rock.h"

CEffect_SwingAttack::CEffect_SwingAttack(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_SwingAttack::CEffect_SwingAttack(const CEffect_SwingAttack & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_SwingAttack::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_SwingAttack::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
	m_fScale = 3.f;
	m_pTransformCom->Set_Scailing(m_fScale);
	m_fAlphaColor = 1.f;
	m_fGlowValue = 5.f;
	return S_OK;
}

_int CEffect_SwingAttack::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Move_Effect(TimeDelta)))
		return -1;

	m_fAlphaColor = 1;

	m_dTimeAccDead += TimeDelta;

	if (0.5f <= m_dTimeAccDead)
		return 1;

	return 0;
}

_int CEffect_SwingAttack::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Effect_Create(TimeDelta)))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RNDER_BULR, this)))
		return E_FAIL;


	return 0;
}

HRESULT CEffect_SwingAttack::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(30)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_SwingAttack::Move_Effect(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)(pGameInstance)->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);

	_float4 vPos;
	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	vPos.y += 1.3f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	m_fRotY += 30.f;

	m_pTransformCom->Set_Rotation(_float3(90.f, m_fRotY, 0.f));
	if (360 <= m_fRotY)
		m_fRotY = 0.f;
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CEffect_SwingAttack::Effect_Create(_double TimeDelta)
{
	if (false == m_bCreateCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CVIBuffer_PointInstance::POINTINST tPointInst;
		ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
		tPointInst.m_iType = 7;
		tPointInst.m_iSize = 5;
		tPointInst.m_iTextureIndex = 7;
		tPointInst.m_iPassIndex = 4;

		XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		tPointInst.m_vPosition.y -= 0.05f;
		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PetPointEffect", &tPointInst)))
			return E_FAIL;

		Safe_Release(pGameInstance);

		m_dCreateTimeAcc = 0.f;

		m_bCreateCheck = true;
	}
	return S_OK;
}

HRESULT CEffect_SwingAttack::SetUp_Component_Load()
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

HRESULT CEffect_SwingAttack::SetUp_Component()
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
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_SwingAttack::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	_float4 RectRandom;
	RectRandom = _float4(0.f, 148.f / 255.f, 170.f / 255.f, 0.f);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));
	
	m_pModelCom->Set_RawValue("g_glowStrength", &m_fGlowValue, sizeof(_float));

	if (FAILED(m_pModelCom->Set_RawValue("g_RectColor", &RectRandom, sizeof(_float4))))
		return E_FAIL;

	m_pModelCom->Set_RawValue("g_AlphaColor", &m_fAlphaColor, sizeof(_float));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", m_pModelCom, 6);

	Safe_Release(pGameInstance);

	return S_OK;
}

CEffect_SwingAttack * CEffect_SwingAttack::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_SwingAttack* pInstance = new CEffect_SwingAttack(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_SwingAttack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_SwingAttack::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_SwingAttack* pInstance = new CEffect_SwingAttack(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_SwingAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_SwingAttack::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);


	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
