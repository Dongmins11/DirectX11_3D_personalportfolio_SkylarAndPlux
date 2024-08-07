// MyForm2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm2.h"
#include "MainFrm.h"
#include "MapTool2.h"
#include "PathFinder.h"
#include "NaviTool.h"

// CMyForm2
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNCREATE(CMyForm2, CFormView)

CMyForm2::CMyForm2()
	: CFormView(IDD_MYFORM2)
{

}

CMyForm2::~CMyForm2()
{
	Safe_Delete(m_pNaviTool);
	Safe_Delete(m_pPathFinder);
	Safe_Delete(m_pMapTool);
}

void CMyForm2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMyForm2, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm2::OnTcnSelchangeTabControl)
END_MESSAGE_MAP()


// CMyForm2 진단입니다.

#ifdef _DEBUG
void CMyForm2::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm2 메시지 처리기입니다.


void CMyForm2::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (FAILED(Native_Construct()))
		return;
}


void CMyForm2::OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint	iSelIndex = m_TabCtrl.GetCurSel();

	switch (iSelIndex)
	{
	case 0:
		m_pPathFinder->ShowWindow(SW_SHOW);
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pMapTool->ShowWindow(SW_SHOW);
		m_pPathFinder->ShowWindow(SW_HIDE);
		m_pNaviTool->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pNaviTool->ShowWindow(SW_SHOW);
		m_pPathFinder->ShowWindow(SW_HIDE);
		m_pMapTool->ShowWindow(SW_HIDE);
	default:
		break;
	}
	*pResult = 0;
}

HRESULT CMyForm2::Native_Construct()
{
	Tab_Native_Construct();
	
	return S_OK;
}

void CMyForm2::Tab_Native_Construct()
{
	m_TabCtrl.InsertItem(0, TEXT("PathFinder"));
	m_TabCtrl.InsertItem(1, TEXT("Map_Tool"));
	m_TabCtrl.InsertItem(2, TEXT("Navi_Tool"));

	CRect	Rect_Tab;

	m_TabCtrl.GetWindowRect(&Rect_Tab);


	m_pPathFinder = new CPathFinder;
	m_pPathFinder->Create(IDD_PATHFINDER, &m_TabCtrl);
	m_pPathFinder->MoveWindow(0, 25, Rect_Tab.Width(), Rect_Tab.Height());
	m_pPathFinder->ShowWindow(SW_SHOW);


	m_pMapTool = new CMapTool2;
	m_pMapTool->Create(IDD_MAPTOOL2,&m_TabCtrl);
	m_pMapTool->MoveWindow(0, 25, Rect_Tab.Width(), Rect_Tab.Height());
	m_pMapTool->ShowWindow(SW_HIDE);


	m_pNaviTool = new CNaviTool;
	m_pNaviTool->Create(IDD_NAVITOOL, &m_TabCtrl);
	m_pNaviTool->MoveWindow(0, 25, Rect_Tab.Width(), Rect_Tab.Height());
	m_pNaviTool->ShowWindow(SW_HIDE);



}
