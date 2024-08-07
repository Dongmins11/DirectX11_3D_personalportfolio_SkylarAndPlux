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
// CColliderTool ��ȭ �����Դϴ�.

class CColliderTool : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColliderTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
