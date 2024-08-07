#pragma once

#include "MainFrm.h"
#include "Tool_Defines.h"
#include "GameInstance.h"

// CObjView ���Դϴ�.

class CObjView : public CView
{
	DECLARE_DYNCREATE(CObjView)

protected:
	CObjView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CObjView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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


