// ColliderTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ColliderTool.h"
#include "afxdialogex.h"


// CColliderTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CColliderTool, CDialogEx)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COLLIDERTOOL, pParent)
{

}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialogEx)
END_MESSAGE_MAP()


// CColliderTool �޽��� ó�����Դϴ�.
