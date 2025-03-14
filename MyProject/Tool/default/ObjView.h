#pragma once

#include "MainFrm.h"
#include "Tool_Defines.h"
#include "GameInstance.h"

// CObjView 뷰입니다.

class CObjView : public CView
{
	DECLARE_DYNCREATE(CObjView)

protected:
	CObjView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CObjView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
public:
	CRenderer*				m_pRenderer = nullptr;

public:
	CGameInstance*			m_pGameInstance = nullptr;
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};


