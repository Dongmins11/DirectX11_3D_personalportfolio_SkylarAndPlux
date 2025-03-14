
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "Tool_Defines.h"
#include "GameInstance.h"
#include "Graphic_Device.h"
#include "ToolView.h"
#include "MyForm2.h"
#include "ObjView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

//const int  iMaxUserToolbars = 10;
//const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
//const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetMenu(NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_SpMainSplitter.CreateStatic(this, 1, 2);

	m_SpMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(g_iWinCX, g_iWinCY), pContext);

	m_SecondSplitter.CreateStatic(&m_SpMainSplitter, 2, 1,
									WS_CHILD | WS_VISIBLE,
									m_SpMainSplitter.IdFromRowCol(0, 0));

	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CObjView), CSize(400, 250), pContext);
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm2), CSize(400, 400), pContext);

	m_SpMainSplitter.SetColumnInfo(0, 630, 100);


	return TRUE;
}