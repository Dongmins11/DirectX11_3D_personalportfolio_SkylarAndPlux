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
class CVIBuffer_Cube;
END

BEGIN(Tool)
class CTerrain_Tool;
class CFloor_Tool;
class CRenderObject;
class CArrow;
END
// CColliderTool 대화 상자입니다.

class CColliderTool : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColliderTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
