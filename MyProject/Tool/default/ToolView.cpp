
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Tool_Defines.h"
#include "GameInstance.h"
#include "Tool_Add_Defines.h"
#include "MainFrm.h"
#include "MyForm2.h"
#include "MapTool2.h"
#include "NaviTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
HWND g_hWnd;
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
	CGameInstance::Release_Engine();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnTimer(nIDEvent);

	if (FAILED(Tool_View_Render()))
		return;
}

 
void CToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	KillTimer(1);
}

HRESULT CToolView::Set_Engine_Device()
{
	CGraphic_Device::DEVICEDESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::DEVICEDESC));

	GraphicDesc.hWnd = GetSafeHwnd();
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eWinMode = CGraphic_Device::MODE_WIN;

	if (FAILED(m_pGameInstance->Initalize_Engine_Tool(LEVEL_END, AfxGetInstanceHandle(), GraphicDesc, &m_pDevice, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Read_GameObject_Tool_Prototype()))
		return E_FAIL;

	if (FAILED(Read_Components_Tool_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Tool_Clone()))
		return E_FAIL;

	return S_OK;
}

HRESULT CToolView::NativeConstruct()
{
	SetTimer(1, 1, nullptr);
	Adjust_Rect();
	g_hWnd = m_hWnd;

	return S_OK;
}

HRESULT CToolView::Read_GameObject_Tool_Prototype()
{
	/* for.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), CCamer_Tool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), CFloor_Tool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_RenderObject"), CRenderObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Arrow"), CArrow::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Navigation"), CNavigationTool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_NavigationCube"), CNavigationCube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CToolView::Read_Components_Tool_Prototype()
{

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_SPHERE))))
		return E_FAIL;


	/* for.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	/* for.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shpere"), CVIBuffer_Sphere::Create(m_pDevice, m_pDeviceContext,5.f,20,20))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_CubeColider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_CubeColider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_CubeColider"), CVIBuffer_Colider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_CubeColider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Navigation"), CVIBuffer_Navigation::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, 256,256 ))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Textures/Default%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Default */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Arrow"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Textures/Arrow.png")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dest"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Meshes/Filter/Filter/Base0.png")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Filter */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Filter"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Meshes/Filter/Filter/SandTest.png")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_BumpMap */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BumpMap"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../../Client/Bin/Resources/Meshes/Rotation/DesertWater/SandTest.dds")))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Terrain"), CTerrain_Tool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain_HeightMap"), CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, L"../../Client/Bin/Resources/Textures/Terrain/HightMapTest0.bmp"))))
		return E_FAIL;


	

	LIGHTDESC	tLightDesc;
	ZeroMemory(&tLightDesc, sizeof(LIGHTDESC));

	tLightDesc.iType = LIGHTDESC::TYPE_DIRECTIONAL;
	tLightDesc.vDirection = _float4(0.f, -1.f, -1.f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDesc);

	if (FAILED(Read_Meshes_Prototype_Tool2()))
		return E_FAIL;


	return S_OK;
}

HRESULT CToolView::Read_Meshes_Prototype_Tool(const _tchar* pPrototypeTag, _tchar FilePath[], _int PathLegnth, _tchar FileName[], _int NameLength)
{
	//_matrix			PivotMatrix;
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ForkLift0.fbx"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/Map/ForkLift/", "ForkLift0.fbx", PivotMatrix))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Read_Meshes_Prototype_Tool2()
{
	//_matrix			PivotMatrix2;
	//PivotMatrix2 = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Floor0.fbx"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/Map/Floor/", "Floor0.fbx", PivotMatrix2))))
	//	return E_FAIL;

	//_matrix			PivotMatrix;
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ForkLift0.fbx"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, "../../Client/Bin/Resources/Meshes/Map/ForkLift/", "ForkLift0.fbx", PivotMatrix))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CToolView::Ready_GameObject_Tool_Clone()
{
	CCamera::CAMEREADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMEREADESC));
	CameraDesc.vEye = _float3(236.f, 20.f, 83.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = g_iWinCX / (_float)g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPerSec = 10.f;

	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(m_pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), LEVEL_STATIC, L"Clone_Camera", &CameraDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Terrain"), LEVEL_STATIC, L"Clone_Terrain")))
		return E_FAIL;


	return S_OK;
}

void CToolView::Adjust_Rect()
{
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();

	RECT	rcWnd{};
	pMainFrm->GetWindowRect(&rcWnd);
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rCScrollView{};

	GetClientRect(&rCScrollView);

	float	fRowFrm = float(rcWnd.right - rCScrollView.right);
	float	fColFrm = float(rcWnd.bottom - rCScrollView.bottom);

	pMainFrm->SetWindowPos(nullptr,
		0, 
		0, 
		int(g_iWinCX + fRowFrm),
		int(g_iWinCY + fColFrm),
		SWP_NOZORDER); 
}

HRESULT CToolView::Tool_View_Render()
{
	if (0 > m_pGameInstance->Tick_Engine(0.2f))
		return E_FAIL;

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 1.f, 1.f, 1.f));
	m_pGameInstance->Clear_Depth_Stencil_View();

	if (FAILED(m_pRenderer->Draw_Object()))
	{
		MessageBoxW(TEXT("Failed To Draw Renderer in ToolView"));
		return E_FAIL;
	}

	m_pGameInstance->Present();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	if (FAILED(pMyForm->m_pMapTool->Render_MapTool()))
		return E_FAIL;

	//pMyForm->m_pNaviTool->Navi_Picking();
	pMyForm->m_pNaviTool->Mesh_Picking();
	pMyForm->m_pNaviTool->Select_Picking();

	return S_OK;
}


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (FAILED(NativeConstruct()))
		return;
	if (FAILED(Set_Engine_Device()))
		return;
}
