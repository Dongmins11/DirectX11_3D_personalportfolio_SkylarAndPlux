#include "stdafx.h"
#include "..\public\ColliderObject_Tool.h"
#include "GameInstance.h"


CColliderObject_Tool::CColliderObject_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CColliderObject_Tool::CColliderObject_Tool(const CColliderObject_Tool & rhs)
	:CGameObject(rhs)
{
}

HRESULT CColliderObject_Tool::Native_Construct_Prototype()
{
	return S_OK;
}

HRESULT CColliderObject_Tool::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (nullptr != pArg)
	{
		m_pModelDesc = new MODELDESC;
		memcpy(m_pModelDesc, pArg, sizeof(MODELDESC));

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc->vPos));
	}

	return S_OK;
}

_int CColliderObject_Tool::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (1 == m_iDeadNumber)
		return 1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());

	return _int();
}

_int CColliderObject_Tool::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

HRESULT CColliderObject_Tool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCubeCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG
	return S_OK;
}

void CColliderObject_Tool::Set_MyMinusCount(_uint iCount)
{
	m_iMyCount -= iCount;

	if (0 >= iCount)
		iCount = 0;
}

void CColliderObject_Tool::Add_ChildColider(CColliderObject_Tool * pGameObject)
{
	if (nullptr == pGameObject)
		return;

	m_vecChildCollider.push_back(pGameObject);
}

HRESULT CColliderObject_Tool::Set_Pos()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc->vPos));

	return S_OK;
}

HRESULT CColliderObject_Tool::Set_Scaile()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Scailing(m_pModelDesc->vScaile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CColliderObject_Tool::Set_ScaileXYZ()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ScailingXYZ(m_pModelDesc->vScaileXYZ)))
		return E_FAIL;

	return S_OK;
}

HRESULT CColliderObject_Tool::Set_Rotation()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Rotation(m_pModelDesc->vRotaition)))
		return E_FAIL;

	return S_OK;
}

HRESULT CColliderObject_Tool::Set_ModelDesc(MODELDESC * pModelDesc)
{
	if (nullptr == pModelDesc)
		return E_FAIL;
	MODELDESC tModelDesc;
	ZeroMemory(&tModelDesc, sizeof(MODELDESC));

	tModelDesc = *pModelDesc;
	*m_pModelDesc = tModelDesc;

	return S_OK;
}

HRESULT CColliderObject_Tool::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCubeCom)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform2"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CColliderObject_Tool::Setup_Component_Tool()
{
	return S_OK;
}

HRESULT CColliderObject_Tool::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCubeCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCubeCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCubeCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CColliderObject_Tool * CColliderObject_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CColliderObject_Tool*	pInstance = new CColliderObject_Tool(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CColliderObject_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CColliderObject_Tool::Clone(void* pArg, _uint iMyLevelIndex)
{
	CColliderObject_Tool*	pInstance = new CColliderObject_Tool(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CColliderObject_Tool");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColliderObject_Tool::Free()
{
	__super::Free();

	Safe_Release(m_pAABB);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCubeCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pSphere);
}
