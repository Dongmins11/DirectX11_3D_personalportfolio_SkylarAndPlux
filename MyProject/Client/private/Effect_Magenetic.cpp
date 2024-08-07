#include "stdafx.h"
#include "..\public\Effect_Magenetic.h"
#include "GameInstance.h"
#include "Rock.h"


struct EffectListSort
{
	bool operator()(const CEffect_Magenetic::VIEWDESC vOne, const CEffect_Magenetic::VIEWDESC vTwo) const
	{
		return vOne.fSize > vTwo.fSize;
	}
};

CEffect_Magenetic::CEffect_Magenetic(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CEffect_Magenetic::CEffect_Magenetic(const CEffect_Magenetic & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Magenetic::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Magenetic::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTwoTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pThreeTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pForTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		
		m_pTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pTwoTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pThreeTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pForTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pTwoTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pThreeTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
		m_pForTransformCom->Set_Rotation(_float3(90.f, 0.f, 0.f));
	}

	ZeroMemory(&m_fOneViewZ, sizeof(VIEWDESC));
	ZeroMemory(&m_fTwoViewZ, sizeof(VIEWDESC));
	ZeroMemory(&m_fThreeViewZ, sizeof(VIEWDESC));
	ZeroMemory(&m_fForViewZ, sizeof(VIEWDESC));

	m_fScale = 3.f;
	m_fTwoScale = 6.f;
	m_fThreeScale = 9.f;
	m_fForScale = 12.f;

	m_pTransformCom->Set_Scailing(m_fScale);
	m_pTwoTransformCom->Set_Scailing(m_fTwoScale);
	m_pThreeTransformCom->Set_Scailing(m_fThreeScale);
	m_pForTransformCom->Set_Scailing(m_fForScale);

	return S_OK;
}

_int CEffect_Magenetic::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	Compute_ViewZ(m_pTransformCom);


	if (FAILED(Move_Effect(TimeDelta)))
		return -1;

	if (FAILED(Player_PosSet(TimeDelta)))
		return -1;

	return 0;

}

_int CEffect_Magenetic::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (true == m_bRenderCheck)
	{

		if (FAILED(Compute_Z(TimeDelta)))
			return -1;

		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
			return E_FAIL;

	}
	return 0;
}

HRESULT CEffect_Magenetic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Magenetic::Move_Effect(_double TimeDelta)
{
	m_fScale -= TimeDelta * 7.f;
	m_fTwoScale -= TimeDelta * 7.f;
	m_fThreeScale -= TimeDelta * 7.f;
	m_fForScale -= TimeDelta * 7.f;

	if(0 >= m_fScale)
		m_fScale = 3.f;
	if (3 >= m_fTwoScale)
		m_fTwoScale = 6.f;
	if (6 >= m_fThreeScale)
		m_fThreeScale = 9.f;
	if (9 >= m_fForScale)
		m_fForScale = 12.f;

	m_pTransformCom->Set_Scailing(m_fScale);
	m_pTwoTransformCom->Set_Scailing(m_fTwoScale);
	m_pThreeTransformCom->Set_Scailing(m_fThreeScale);
	m_pForTransformCom->Set_Scailing(m_fForScale);

	return S_OK;
}

HRESULT CEffect_Magenetic::Player_PosSet(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);

	_float4 vPos;
	XMStoreFloat4(&vPos, pTargetTransform->Get_State(CTransform::STATE_POSITION));
	vPos.y += 0.007f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMLoadFloat4(&vPos));
	
	vPos.y += 0.008f;
	m_pTwoTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	
	vPos.y += 0.009f;
	m_pThreeTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	
	vPos.y += 0.01f;
	m_pForTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CEffect_Magenetic::Compute_Z(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	m_fOneViewZ.fSize = XMVectorGetX(XMVector4Length(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));
	//m_fOneViewZ.fSize = XMVectorGetX(m_pTransformCom->Get_Scale());
	m_fOneViewZ.pTransform = m_pTransformCom;
	m_fOneViewZ.Rect = m_pModelCom;
	m_ListDesc.push_back(m_fOneViewZ);

	m_fTwoViewZ.fSize = XMVectorGetX(XMVector4Length(m_pTwoTransformCom->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));
	//m_fTwoViewZ.fSize = XMVectorGetX(m_pTwoTransformCom->Get_Scale());
	m_fTwoViewZ.pTransform = m_pTwoTransformCom;
	m_fTwoViewZ.Rect = m_pTwoModelCom;
	m_ListDesc.push_back(m_fTwoViewZ);
	
	m_fThreeViewZ.fSize = XMVectorGetX(XMVector4Length(m_pThreeTransformCom->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));
	//m_fThreeViewZ.fSize = XMVectorGetX(m_pThreeTransformCom->Get_Scale());
	m_fThreeViewZ.pTransform = m_pThreeTransformCom;
	m_fThreeViewZ.Rect = m_pThreeModelCom;
	m_ListDesc.push_back(m_fThreeViewZ);
	
	m_fForViewZ.fSize = XMVectorGetX(XMVector4Length(m_pForTransformCom->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));
	//m_fForViewZ.fSize = XMVectorGetX(m_pForTransformCom->Get_Scale());
	m_fForViewZ.pTransform = m_pForTransformCom;
	m_fForViewZ.Rect = m_pForModelCom;
	m_ListDesc.push_back(m_fForViewZ);

	

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CEffect_Magenetic::SetUp_Component_Load()
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

HRESULT CEffect_Magenetic::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_TwoVIBuffer"), (CComponent**)&m_pTwoModelCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_ThreeVIBuffer"), (CComponent**)&m_pThreeModelCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_ForVIBuffer"), (CComponent**)&m_pForModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_TwoTransform"), (CComponent**)&m_pTwoTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ThreeTransform"), (CComponent**)&m_pThreeTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ForTransform"), (CComponent**)&m_pForTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Magenetic::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_ListDesc.sort(EffectListSort());

	for (auto iter : m_ListDesc)
	{
		iter.Rect->Set_RawValue("g_WorldMatrix", &iter.pTransform->Get_WorldMatrixTranspose(), sizeof(_matrix));
		iter.Rect->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
		iter.Rect->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

		m_pTextureCom->SetUp_OnShader("g_DiffuseTexture", iter.Rect, 5);


		if (FAILED(iter.Rect->Render(21)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	m_ListDesc.clear();

	return S_OK;
}

CEffect_Magenetic * CEffect_Magenetic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEffect_Magenetic* pInstance = new CEffect_Magenetic(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CEffect_Magenetic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Magenetic::Clone(void* pArg, _uint iMyLevelIndex)
{
	CEffect_Magenetic* pInstance = new CEffect_Magenetic(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CEffect_Magenetic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Magenetic::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);


	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
