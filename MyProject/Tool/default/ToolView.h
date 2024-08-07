
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once
#include "Tool_Defines.h"
#include "GameInstance.h"


BEGIN(Engine)
class CGameInstance;
class CRenderer;
END


class CToolDoc;

class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
public:
	HWND Get_Hwnd() { return m_hWnd; }
private:
	HRESULT Set_Engine_Device();
	HRESULT NativeConstruct();
	HRESULT Tool_View_Render();
private:
	HRESULT Read_GameObject_Tool_Prototype();
	HRESULT Read_Components_Tool_Prototype();
	HRESULT Ready_GameObject_Tool_Clone();
public:
	HRESULT Read_Meshes_Prototype_Tool(const _tchar* pPrototypeTag,_tchar FilePath[],_int PathLegnth, _tchar FileName[],_int NameLength );
	HRESULT Read_Meshes_Prototype_Tool2();

private:
	void	Adjust_Rect();
public:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
public:
	CRenderer*				m_pRenderer = nullptr;
public:
	CGameInstance*			m_pGameInstance = nullptr;
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

