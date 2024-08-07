#include "stdafx.h"
#include "..\public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Camera_Fly.h"
#include "Sound_Manager.h"

CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::Get_Instance())
	, m_pSoundManager(CSound_Manager::Get_Instance())
{
	Safe_AddRef(m_pSoundManager);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Native_Construct()
{
	HRESULT hr = 0;


	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return E_FAIL;

	if (FAILED(Initialize_Engine()))
		return E_FAIL;

	if (FAILED(Ready_For_Font()))
		return E_FAIL;

	if (FAILED(Ready_For_StaticLevel_Component()))
		return E_FAIL;

	if (FAILED(Ready_For_StaticLevel_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_For_StaticLevel_Effect()))
		return E_FAIL;
	
	m_pSoundManager->Initialize();

	if (FAILED(Open_Level(LEVEL_BOSS_STAGE)))
		return E_FAIL;

	return S_OK;
}

_int CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Tick_Engine(TimeDelta);

	m_TimeAcc += TimeDelta;

	return _int();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Clear_Depth_Stencil_View()))
		return E_FAIL;

	if (FAILED(m_pRenderer->Draw_Object()))
		return E_FAIL;

	++m_iFPS;
	if (m_TimeAcc > 1.0)
	{	
		TCHAR	szFPS[16] = L"";
		swprintf_s(szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, szFPS);

		m_TimeAcc = 0.0;
		m_iFPS = 0;
	}

	if (m_pGameInstance->Key_Down(VK_F7))
		m_pRenderer->Set_RenderTargetCheck(true);
	if (m_pGameInstance->Key_Down(VK_F8))
		m_pRenderer->Set_RenderTargetCheck(false);

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Initialize_Engine()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CGraphic_Device::DEVICEDESC tGraphic_Device;
	ZeroMemory(&tGraphic_Device, sizeof(CGraphic_Device::DEVICEDESC));

	tGraphic_Device.eWinMode = CGraphic_Device::MODE_WIN;
	tGraphic_Device.hWnd = g_hWnd;
	tGraphic_Device.iWinCX = g_iWinCX;
	tGraphic_Device.iWinCY = g_iWinCY;

	if (FAILED(m_pGameInstance->Initalize_Engine(LEVEL_END, g_hInst, tGraphic_Device, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_For_StaticLevel_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	/* for.Prototype_GameObject_Camera_Fly */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), CCamera_Fly::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_For_StaticLevel_GameObject()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* for.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	/* for.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext,259,259))))
		return E_FAIL;

	/* for.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), CColider::Create(m_pDevice, m_pDeviceContext,CColider::TYPE_AABB))))
		return E_FAIL;

	/* for.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_OBB))))
		return E_FAIL;
	
	/* for.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_SPHERE))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_HeightMap"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext,L"../Bin/Resources/Textures/Terrain/HightMapTest0.bmp"))))
		return E_FAIL;



	////////For.Effect////////////

	/* for.Prototype_Component_VIBuffer_RectInstance_CountThree */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance_CountThree"), CVIBuffer_RectInstance::Create(m_pDevice, m_pDeviceContext,3))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_RectInstance_CountFive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance_CountFive"), CVIBuffer_RectInstance::Create(m_pDevice, m_pDeviceContext, 5))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_RectInstance_CountTen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance_CountTen"), CVIBuffer_RectInstance::Create(m_pDevice, m_pDeviceContext, 10))))
		return E_FAIL;



	/* for.Prototype_Component_VIBuffer_PointInstance_Cloud */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Cloud"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 40,2.f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_PetCloud */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_PetCloud"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext,30,0.5f))))
		return E_FAIL;


	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_One"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Two"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Three"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_For"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Five"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Collector_Six"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_SmokeOne */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_SmokeOne"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 7.f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_SmokeTwo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_SmokeTwo"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 7.f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Collector */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Clone_Ten"), CVIBuffer_PointInstance_Clone::Create(m_pDevice, m_pDeviceContext, 1, 0.3f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Landing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Landing"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;


	/////////For.Texture///////////

	/* for.Prototype_Component_Texture_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Level_Ui/Level_Ui%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Ui"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Player_Ui/Player_Ui%d.png"), 14))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_PathFinder_To_Model(L"../../Data/PathName.txt", LEVEL_STATIC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_For_StaticLevel_Effect()
{
	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_One */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_One"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.1f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Two */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Two"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.1f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Three */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Three"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.1f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_For */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_For"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.1f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Five */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Five"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 10, 0.1f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Six */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Six"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Seven */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Seven"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Eight */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Eight"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Nine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Nine"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_PointInstance_Effect_Ten */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance_Effect_Ten"), CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, 20, 0.5f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_For_Font()
{
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pDeviceContext, TEXT("Font_Gulim"), TEXT("../Bin/Resources/Font/kodia.spritefont"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{	
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, eLevel))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Native_Construct()))
	{
		MSGBOX("Failed to Create CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	CGameInstance::Release_Engine();
	
}
