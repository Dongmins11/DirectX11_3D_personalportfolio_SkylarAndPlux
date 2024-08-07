#pragma once

#include "Tool_Defines.h"
#include "GameInstance.h"
#include "PathFinder.h"
#include "afxcmn.h"
#include "afxwin.h"

BEGIN(Engine)
class CGameInstance;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Tool)
class CTerrain_Tool;
class CFloor_Tool;
class CRenderObject;
class CArrow;
END
// CMapTool2 대화 상자입니다.

class CMapTool2 : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool2)

public:
	CMapTool2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CFloor_Tool*	Finder_Object(wstring ObjectTag,_uint iIndex);
	MODELDESC*		Finder_ModelDesc(wstring DescTag,_uint iIndex);
public:/*GeneRic */
	HRESULT Render_MapTool();
	HRESULT For_Picking_Terrain();
	HRESULT Check_Picking();
	HRESULT Release_Picking();
	HRESULT Select_Picking();
	HRESULT MESH_Picking();
public:/* GeneRic */
	void	Set_ComboBox_List(_uint iIndex);
	void	Set_ObjectBox_List(_uint iIndex);
	void	Set_Object_Value(_uint iIndex);
public:/*GeneRic */
	void	Radio_Value_Set();
	void	Render_Object();
public:/*GeneRic */
	wstring		m_strCurruntTag = L"";
	wstring		m_strPrototypeCurrent = L"";
	wstring		m_strCurrentObjName = L"";
	wstring		m_strPairName = L"";

public:/*GeneRic */
	list<CFloor_Tool*>				m_ListObject;
	list<MODELDESC*>				m_ListModelDesc;
	map<wstring,list<CFloor_Tool*>>	m_MapObject;
	map<wstring, list<MODELDESC*>>	m_MapModelDesc;
	list<_tchar*>				m_ListCloneTag;
	CFloor_Tool*				m_pObject = nullptr;
	CFloor_Tool*				m_pCurruntObject = nullptr;
	MODELDESC*					m_pModelDesc;
	CRenderObject*				m_pRenderObject = nullptr;
	CArrow*						m_pArrow = nullptr;
	_float2						m_vMouse = _float2(0.f, 0.f);
	_float2						m_vMouseBackUp = _float2(0.f, 0.f);
	_float						m_fTimeAcc = 0.f;
	_uint						m_iCount = 0;
	_int						m_iSign = 1;
	_float						m_fValueAcc[3];
	_uint						m_iObjectCount = 0;

public:/*GeneRic */
	CTransform*					m_pTransform = nullptr;
	CVIBuffer_Terrain*			m_pTerrainBuffer = nullptr;

public:/*For.MFC*/
	afx_msg void OnCbnSelchangeComboBox();
	afx_msg void OnLbnObjectSeleteChancePick();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedPickingOn();
	afx_msg void OnLbnSelchangeModelDesc();
	afx_msg void OnBnClickedSetUpPosButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();

public:/*For.MFC*/
	CComboBox m_ComboBox;
	CListBox m_ObjectListBox;
	CListBox m_AddListBox;

	CSliderCtrl m_SliderRotationX;
	CSliderCtrl m_SliderRotationY;
	CSliderCtrl m_SliderRotationZ;
	CEdit m_EditRotX;
	CEdit m_EditRotY;
	CEdit m_EditRotZ;

	CSpinButtonCtrl m_SpinCtalX;
	CSpinButtonCtrl m_SpinCtalY;
	CSpinButtonCtrl m_SpinCtalZ;
	CEdit m_PosX;
	CEdit m_PosY;
	CEdit m_PosZ;

	CSliderCtrl m_SliderScaile;
	CEdit m_EditScaile;

	CSliderCtrl m_SliderX;
	CSliderCtrl m_SliderY;
	CSliderCtrl m_SliderZ;
	CEdit m_EditScaleX;
	CEdit m_EditScaleY;
	CEdit m_EditScaleZ;

	CButton m_CheckPicking;
	CButton m_CheckPickPos;
	CButton m_CheckRender;
	CButton m_ButtonDelete;
	CButton m_CheckPickObject;

	CButton m_CheckRadio[3];
	CButton m_CheckMesh;
};
