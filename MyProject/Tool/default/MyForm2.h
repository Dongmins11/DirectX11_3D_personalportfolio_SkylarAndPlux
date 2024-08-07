#pragma once

#include "afxcmn.h"
#include "GameInstance.h"


// CMyForm2 �� ���Դϴ�.

class CMyForm2 : public CFormView
{
	DECLARE_DYNCREATE(CMyForm2)

protected:
	CMyForm2();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm2();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM2 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult);
public:
	HRESULT Native_Construct();
	void Tab_Native_Construct();
public:
	class CMapTool2*			m_pMapTool = nullptr;
	class CPathFinder*		m_pPathFinder = nullptr;
	class CNaviTool*		m_pNaviTool = nullptr;
	CTabCtrl m_TabCtrl;
};


