#include "stdafx.h"
#include "..\Public\ItemMagenetic.h"

#include "GameInstance.h"
#include "Effect_Rect.h"


CItemMagenetic::CItemMagenetic(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CItemMagenetic::CItemMagenetic(const CItemMagenetic & rhs)
	: CGameObject(rhs)
{

}

HRESULT CItemMagenetic::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CItemMagenetic::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.0f, 38.0f, 25.f, 1.f));
	m_pTransformCom->Set_Scailing(2.f);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_RectInstance::RECTINST tRectInst;
	ZeroMemory(&tRectInst, sizeof(CVIBuffer_RectInstance::RECTINST));
	tRectInst.m_iType = 0;
	tRectInst.m_iSize = 5;
	tRectInst.m_iTextureIndex = 0;
	tRectInst.m_iPassIndex = 1;
	XMStoreFloat4(&tRectInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	tRectInst.m_vPosition.z += 0.3f;
	if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Rect"), m_iMyLevel, L"Clone_RectEffect", &tRectInst, (CGameObject**)&m_pEffectObject)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

_int CItemMagenetic::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_dRotationY += TimeDelta * 150;
	m_pTransformCom->Set_Rotation(_float3(0.f, m_dRotationY, 0.f));
	if (360.0 <= m_dRotationY)
	{
		m_dRotationY = 0.0;
	}

	if (true == m_bDeadCheck)
		return 1;

	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CItemMagenetic::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	if (FAILED(Collision_Check()))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CItemMagenetic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CItemMagenetic::Collision_Check()
{
	CGameInstance * pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);
	if (nullptr == pTargetSphere)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	if (true == m_pSphereCom->Collision_Sphere(pTargetSphere))
	{
		dynamic_cast<CEffect_Rect*>(m_pEffectObject)->Set_DeadCheck(true);
		m_bDeadCheck = true;

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Magenetic"), m_iMyLevel, L"Clone_Magenetic")))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Magenetic_Ui"), m_iMyLevel, L"Clone_Magenetic_Ui")))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CItemMagenetic::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Magenetic0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 7.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;
	//if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_BufferTransform"), (CComponent**)&m_pBufferTransformCom, &TransformDesc)))
	//	return E_FAIL;

	///* Com_Texture */
	//if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;


	/* For.Com_SPHERE*/
	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



HRESULT CItemMagenetic::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CItemMagenetic * CItemMagenetic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CItemMagenetic*	pInstance = new CItemMagenetic(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created : CItemMagenetic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CItemMagenetic::Clone(void* pArg, _uint iMyLevelIndex)
{
	CItemMagenetic*	pInstance = new CItemMagenetic(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CItemMagenetic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItemMagenetic::Free()
{
	__super::Free();

	if (nullptr != m_pEffectObject)
	{
		Safe_Release(m_pEffectObject);
	}


	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferTransformCom);



}
