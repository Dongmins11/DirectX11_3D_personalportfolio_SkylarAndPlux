#include "stdafx.h"
#include "..\public\SkyBox.h"
#include "GameInstance.h"


CSkyBox::CSkyBox(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSkyBox::Native_Construct_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (FAILED(Setup_Component()))
		return E_FAIL;

	return S_OK;
}

_int CSkyBox::Tick(_double TimeDelta)
{
	return _int();
}

_int CSkyBox::LateTick(_double TimeDelta)
{
	if (nullptr == m_pRenderCom)
		return -1;


	if (FAILED(m_pRenderCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;


	return _int();
}

HRESULT CSkyBox::Render()
{

	if (FAILED(Setup_ConstantTable()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render(1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::Setup_Component()
{
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRenderCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_SkyBox"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBox::Setup_ConstantTable()
{
	if (nullptr == m_pVIBufferCom || nullptr == m_pTransformCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	_float3		CamPos;
	XMStoreFloat3(&CamPos,pGameInstance->Get_CamPosition());

	WorldMatrix._11 = 3;
	WorldMatrix._22 = 3;
	WorldMatrix._33 = 3;
	WorldMatrix._41 = CamPos.x;
	WorldMatrix._42 = CamPos.y;
	WorldMatrix._43 = CamPos.z;
	WorldMatrix._44 = 1;

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&WorldMatrix)), sizeof(_matrix));
	//m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pTextureCom->SetUp_OnShader("g_DiffuseTexture",m_pVIBufferCom, 1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CSkyBox * CSkyBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CSkyBox* pInstance = new CSkyBox(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create SkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkyBox::Clone(void* pArg, _uint iMyLevelIndex)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create SkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pRenderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}
