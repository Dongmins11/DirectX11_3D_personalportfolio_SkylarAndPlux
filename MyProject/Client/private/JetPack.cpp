#include "stdafx.h"
#include "..\Public\JetPack.h"

#include "GameInstance.h"

CJetPack::CJetPack(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CJetPack::CJetPack(const CJetPack & rhs)
	: CGameObject(rhs)
{

}

HRESULT CJetPack::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CJetPack::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		return E_FAIL;


	if (FAILED(SetUp_TargetBoneMatrices("spine_05")))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 25.f, -130.f, 1.f));
	m_pTransformCom->Set_Scailing(1.4f);
	m_pTransformCom->Set_Rotation(_float3(-90.f, 0.f,180.f));
	m_pPlayerWorldMat = m_pTransformCom->Get_WorldFloat4x4();

	return S_OK;
}

_int CJetPack::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pModelCom->Set_Animation(1);
	m_pModelCom->Update(TimeDelta);
	m_pModelCom->Set_PreAnimation(1);


	return 0;
}

_int CJetPack::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	if (nullptr != m_pCombinedMatrix ||
		nullptr != m_pTargetWorldMatrix)
	{
		_matrix		TransformMatrix = XMLoadFloat4x4(&m_OffsetMatrix) * XMLoadFloat4x4(m_pCombinedMatrix) * XMLoadFloat4x4(&m_PivotMatrix) * XMLoadFloat4x4(m_pTargetWorldMatrix);
		_float4x4	Matrix;
		XMStoreFloat4x4(&Matrix, XMLoadFloat4x4(&m_pPlayerWorldMat) * TransformMatrix);
		m_pTransformCom->Set_WolrdMatirx_float4x4(Matrix);
		m_pSphereCom->Update(XMLoadFloat4x4(&Matrix));
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CJetPack::Render()
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

HRESULT CJetPack::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("JetPack1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 7.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_SPHERE*/
	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(-100.f, 0.0f, -140.0f);
	ColliderDesc.fRadius = 500.f;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CJetPack::SetUp_TargetBoneMatrices(const char * pBoneName)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*	pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0);
	if (nullptr == pTargetTransform)
		return E_FAIL;

	CModel*		pModel = (CModel*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == pModel)
		return E_FAIL;

	m_OffsetMatrix = pModel->Get_OffsetMatrix(pBoneName);
	m_PivotMatrix = pModel->Get_PivotMatrix();
	m_pCombinedMatrix = pModel->Get_CombinedMatrixPtr(pBoneName);
	m_pTargetWorldMatrix = pTargetTransform->Get_WorldMatrixPtr();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CJetPack::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CJetPack * CJetPack::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CJetPack*	pInstance = new CJetPack(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created : CJetPack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJetPack::Clone(void* pArg, _uint iMyLevelIndex)
{
	CJetPack*	pInstance = new CJetPack(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CJetPack");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJetPack::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);



}
