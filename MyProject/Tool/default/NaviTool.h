#pragma once

#include "Tool_Defines.h"
#include "GameInstance.h"
#include "PathFinder.h"
#include "afxcmn.h"
#include "afxwin.h"
// CNaviTool 대화 상자입니다.

BEGIN(Engine)
class CGameInstance;
class CTransform;
class CVIBuffer_Terrain;
class CVIBuffer_Navigation;
END

BEGIN(Tool)
class CNavigationTool;
class CRenderObject;
class CTerrain_Tool;
class CFloor_Tool;
class CArrow;
END

class CNaviTool : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviTool)

public:
	CNaviTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:/* For.Generic */
	HRESULT		Navi_Picking();
	HRESULT		Mesh_Picking();
	HRESULT		Triangle_Picking();
	HRESULT		Select_Picking();
	HRESULT		Finder_Point();
public:/*For. Generic*/
	HRESULT		Set_CurrentObject(CFloor_Tool* FloorObject);


public:/*For. Generic*/
	CFloor_Tool*	m_pCurrentObject = nullptr;
public:/* For.Generic */
	_uint		m_iIndex = 0;
	_float		m_fTimeAcc = 0.f;
	_bool		m_bIsNaviCheck[3];
	_float3		m_vPos[3];
	_uint		m_iCount = 0;
public:/* For.Generic */
	vector<CVIBuffer_Navigation*>	m_vecNavigation;

public:/* For.Generic */
	CTransform*			m_pTransform = nullptr;
	
	CVIBuffer_Terrain*	m_pTerrainBuffer = nullptr;

	CNavigationTool*	m_pNaviMeshTool = nullptr;

public:/* For.MFC*/
	afx_msg void OnCbnSelchangeIndexPoint();
	afx_msg void OnLbnSelchangePointListBox();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
public:/* For.MFC*/
	CButton m_CheckPicking;
	CButton m_ButtonDelete;
	CListBox m_ListBoxPoint;
	CComboBox m_ComboBoxIndex;
};
