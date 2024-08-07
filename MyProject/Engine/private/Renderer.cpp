#include "..\public\Renderer.h"
#include "RendererLayer.h"
#include "RenderTarget_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
	, m_pTarget_Manager(CRenderTarget_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	for (_uint i = 0; i < RENDER_GROUP::RENDER_END; ++i)
		m_pRendererGroup[i] = CRendererLayer::Create();

	_uint				iNumViewports = 1;
	D3D11_VIEWPORT		Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	m_pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Diffuse"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Normal"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Depth"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_EmmissiveColor"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shade"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Emmissive"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Shadow"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Total"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_Effect"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;																	

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_VerticalBlur"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_MapVerticalBlur"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_MapHorizonBlur"), Viewport.Width, Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDeviceContext, TEXT("Target_FirstDepth"), Viewport.Width, Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		//return E_FAIL;


	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Deferred"), TEXT("Target_EmmissiveColor"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_LightAcc"), TEXT("Target_Emmissive"))))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_LightAcc"), TEXT("Target_Shadow"))))
	//	return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Total"), TEXT("Target_Total"))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_MapBlurX"), TEXT("Target_MapVerticalBlur"))))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_MapBlurY"), TEXT("Target_MapHorizonBlur"))))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_Effect"), TEXT("Target_Effect"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MultiRenderTarget(TEXT("MRT_VerticalBlur"), TEXT("Target_VerticalBlur"))))
		return E_FAIL;



	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pVerticalBlurVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pVerticalBlurVIBuffer)
		return E_FAIL;

	m_pHorizonBlurVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pHorizonBlurVIBuffer)
		return E_FAIL;

	//m_pMapVerticalBlurVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	//if (nullptr == m_pMapVerticalBlurVIBuffer)
	//	return E_FAIL;

	//m_pMapHorizonBlurVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext);
	//if (nullptr == m_pMapHorizonBlurVIBuffer)
	//	return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixIdentity());

	m_TransformMatrix._11 = Viewport.Width;
	m_TransformMatrix._22 = Viewport.Height;

	m_TransformMatrix._41 = 0.0f;
	m_TransformMatrix._42 = 0.0f;

	XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));

	Weight[0] = 0.0561f;
	Weight[1] = 0.1353f;
	Weight[2] = 0.278f;
	Weight[3] = 0.4868f;
	Weight[4] = 0.7216f;
	Weight[5] = 0.9231f;
	Weight[6] = 1.f;
	Weight[7] = 0.9231f;
	Weight[8] = 0.7261f;
	Weight[9] = 0.4868f;
	Weight[10] = 0.278f;
	Weight[11] = 0.1353f;
	Weight[12] = 0.0561f;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Diffuse"), 0.f, 0.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Normal"), 0.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Depth"), 0.f, 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_EmmissiveColor"), 0.f, 300.f, 100.f, 100.f)))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_FirstDepth"), 0.f, 300.f, 100.f, 100.f)))
		//return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Shade"), 100.f, 0.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Emmissive"), 100.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Shadow"), 100.f, 200.f, 100.f, 100.f)))
	//	return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Total"), 200.f,	0.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_Effect"), 200.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_MapVerticalBlur"), 300.f, 0.f, 100.f, 100.f)))
		//return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_MapHorizonBlur"), 300.f, 100.f, 100.f, 100.f)))
		//return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug_Buffer_Desc(TEXT("Target_VerticalBlur"), 200.f, 200.f, 100.f, 100.f)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER_GROUP eRenderState, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pRendererGroup[eRenderState]->Add_RenderGroup(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_Object()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;
	
	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Light()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	
	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_EFFECT()))
		return E_FAIL;

	if (FAILED(Render_Total()))
		return E_FAIL;

	//if (FAILED(Render_MapBlur()))
		//return E_FAIL;

	if (FAILED(Render_Blur()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_EndScene()))
		return E_FAIL;


#ifdef _DEBUG
	if (false == m_bIsRender)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Deferred"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_LightAcc"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Total"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_Effect"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_VerticalBlur"))))
			return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_MapBlurX"))))
			//return E_FAIL;
		//if (FAILED(m_pTarget_Manager->Render_Debug_Buffer(TEXT("MRT_MapBlurY"))))
			//return E_FAIL;

	}
#endif // _DEBUG
	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	return m_pRendererGroup[RENDER_PRIORITY]->Render();
}

HRESULT CRenderer::Render_NonAlpha()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, L"MRT_Deferred")))
		return E_FAIL;

	if (FAILED(m_pRendererGroup[RENDER_NONALPHA]->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Light()
{
	if (nullptr == m_pLight_Manager || nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, L"MRT_LightAcc")))
		return E_FAIL;

	if (FAILED(m_pLight_Manager->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}



HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(L"Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_TextureValue("g_ShadeTexture", m_pTarget_Manager->Get_ShaderResourceView(L"Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_TextureValue("g_EmissiveTexture", m_pTarget_Manager->Get_ShaderResourceView(L"Target_Emmissive"))))
		return E_FAIL;
	

	if (FAILED(m_pVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	m_pVIBuffer->Render(5);


	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	return m_pRendererGroup[RENDER_NONLIGHT]->Render();
}

HRESULT CRenderer::Render_Alpha()
{
	return m_pRendererGroup[RENDER_ALPHA]->Render();
}

HRESULT CRenderer::Render_EFFECT()
{
	return m_pRendererGroup[RENDER_EFFECT]->Sort_Render();
}

HRESULT CRenderer::Render_Total()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, L"MRT_Total")))
		return E_FAIL;

	if (FAILED(m_pRendererGroup[RENDER_PRIORITY]->Render()))
		return E_FAIL;

	m_pVIBuffer->Render(5);

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_MapBlur()
{
	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, TEXT("MRT_MapBlurX"))))
		return E_FAIL;

	_uint	a = 1;
	_uint*	iNumViewPort;
	iNumViewPort = &a;
	ZeroMemory(&m_tViewPortDesc, sizeof(D3D11_VIEWPORT));
	m_pDeviceContext->RSGetViewports(iNumViewPort, &m_tViewPortDesc);


	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_WeightArray", Weight, sizeof(_float) * 13)))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Total")))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_TextureValue("g_VerticalBlurTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4    ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_fWinCX", &m_tViewPortDesc.Width, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Set_RawValue("g_fWinCY", &m_tViewPortDesc.Height, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMapVerticalBlurVIBuffer->Render(28)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, TEXT("MRT_MapBlurY"))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_WeightArray", Weight, sizeof(_float) * 13)))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Total")))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_TextureValue("g_HorizonBlurTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_MapVerticalBlur")))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_fWinCX", &m_tViewPortDesc.Width, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Set_RawValue("g_fWinCY", &m_tViewPortDesc.Height, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMapHorizonBlurVIBuffer->Render(29)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur()
{
	_uint	a = 1;
	_uint*	iNumViewPort;
	iNumViewPort = &a;
	ZeroMemory(&m_tViewPortDesc, sizeof(D3D11_VIEWPORT));
	m_pDeviceContext->RSGetViewports(iNumViewPort, &m_tViewPortDesc);

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, TEXT("MRT_Effect"))))
		return E_FAIL;

	if (FAILED(m_pRendererGroup[RNDER_BULR]->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	/////수직 블러//////


	if (FAILED(m_pVerticalBlurVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Total")))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Set_TextureValue("g_VerticalBlurTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Effect")))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4    ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pVerticalBlurVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Set_RawValue("g_fWinCX", &m_tViewPortDesc.Width, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Set_RawValue("g_fWinCY", &m_tViewPortDesc.Height, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MultiRenderTarget(m_pDeviceContext, TEXT("MRT_VerticalBlur"))))
		return E_FAIL;

	if (FAILED(m_pVerticalBlurVIBuffer->Render(24)))//패스);
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MultiRenderTarget(m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_TextureValue("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Total")))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_TextureValue("g_FilterTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Effect")))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_TextureValue("g_HorizonBlurTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_VerticalBlur")))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_RawValue("g_WorldMatrix", &m_TransformMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pHorizonBlurVIBuffer->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_RawValue("g_fWinCX", &m_tViewPortDesc.Width, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Set_RawValue("g_fWinCY", &m_tViewPortDesc.Height, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pHorizonBlurVIBuffer->Render(25))) //패스);
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_EndScene()
{
	return m_pRendererGroup[RENDER_ENDSCENE]->Render();
}

HRESULT CRenderer::Render_UI()
{
	return m_pRendererGroup[RENDER_UI]->Render();
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create Renderer ProtoType");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();
	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_GROUP::RENDER_END; ++i)
		Safe_Release(m_pRendererGroup[i]);

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);

}
