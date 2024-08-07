// PathFinder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFinder.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "MyForm2.h"
#include "MapTool2.h"
#include "ToolView.h"
#include "GameInstance.h"
#include <fstream>
using namespace std;


// CPathFinder 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFinder, CDialogEx)

CPathFinder::CPathFinder(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PATHFINDER, pParent)
{

}

CPathFinder::~CPathFinder()
{

	for (auto& iter : m_PathList)
	{
		Safe_Delete(iter);
	}
	m_PathList.clear();


	for (auto& iter : m_ListTchar)
	{
		Safe_Delete_Array(iter);
	}
	m_ListTchar.clear();

	for (auto& iter : m_Listchar)
	{
		Safe_Delete_Array(iter);
	}
	m_Listchar.clear();

	for (auto& iter : m_ListFileName)
	{
		Safe_Delete_Array(iter);
	}
	m_ListFileName.clear();

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}

void CPathFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPathFinder, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFinder::OnBnClickedButtonPathFinder)
END_MESSAGE_MAP()


// CPathFinder 메시지 처리기입니다.


void CPathFinder::OnBnClickedButtonPathFinder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	TCHAR		szFullPath[MAX_PATH] = L"";

	CFileFind tempFind;

	wstring tempwstrPath = L"../../Client/Bin/Resources/Meshes";

	Safe_ReleaseList();

	CFileInfo::DirInfoExtraction(tempwstrPath, m_PathList);

	wstring		wstrCombined = L"";
	TCHAR		szBuf[MAX_STR] = L"";
	wstring		MapName = L"Map";
	wstring		FilterName = L"Filter";
	wstring		RotationName = L"Rotation";

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CToolView*	pToolView = dynamic_cast<CToolView*>(pMainFrm->m_SpMainSplitter.GetPane(0,1));

	m_pDevice = pToolView->m_pDevice;
	Safe_AddRef(m_pDevice);
	m_pDeviceContext = pToolView->m_pDeviceContext;
	Safe_AddRef(m_pDeviceContext);

	pMyForm->m_pMapTool->m_ComboBox.ResetContent();
	for (auto& iter : m_PathList)
	{
		pMyForm->m_pMapTool->m_ComboBox.AddString(iter->wstrStateKey.c_str());
		for (_int i = 0; i < iter->iCount; ++i)
		{
			TCHAR	pState[MAX_PATH] = L"";
			TCHAR	pFullpath[MAX_PATH] = L"";
			TCHAR	pPrototypeTag[MAX_PATH] = L"";
			wsprintf(pState, iter->wstrPath.c_str(), i);
			wsprintf(pFullpath, iter->wstrRealPathKey.c_str(), i);
			wsprintf(pPrototypeTag, iter->wstrPath.c_str(), i);

			_tchar* pStrState = new _tchar[MAX_PATH];
			_tchar* pStrFullpath = new _tchar[MAX_PATH];
			_tchar* pStrPrototypeTag = new _tchar[MAX_PATH];

			lstrcpy(pStrState, pState);
			lstrcpy(pStrFullpath, pFullpath);
			lstrcpy(pStrPrototypeTag, pPrototypeTag);
			m_ListFileName.emplace_back(pStrState);

			_matrix			PivotMatrix2;

			char*		szFullPath = new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, pStrFullpath, MAX_PATH, szFullPath, MAX_PATH,nullptr,nullptr);

			char*		szFileName = new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, pStrState, MAX_PATH, szFileName, MAX_PATH, nullptr, nullptr);

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			if (!lstrcmp(iter->wstrObjKey.c_str(), MapName.c_str()))
			{
				PivotMatrix2 = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, pStrPrototypeTag,
					CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE::TYPE_NONANIM, szFullPath, szFileName, PivotMatrix2))))
				{
					Safe_Release(pGameInstance);
					return;
				}
			}
			else if (!lstrcmp(iter->wstrObjKey.c_str(), FilterName.c_str()))
			{
				PivotMatrix2 = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, pStrPrototypeTag,
					CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE::TYPE_NONANIM, szFullPath, szFileName, PivotMatrix2))))
				{
					Safe_Release(pGameInstance);
					return;
				}
			}
			else if (!lstrcmp(iter->wstrObjKey.c_str(), RotationName.c_str()))
			{
				PivotMatrix2 = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, pStrPrototypeTag,
					CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE::TYPE_NONANIM, szFullPath, szFileName, PivotMatrix2))))
				{
					Safe_Release(pGameInstance);
					return;
				}
			}
			else 
			{
				PivotMatrix2 = XMMatrixScaling(1.0f, 1.0f, 1.0f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, pStrPrototypeTag,
					CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE::TYPE_ANIM, szFullPath, szFileName, PivotMatrix2))))
				{
					Safe_Release(pGameInstance);
					return;
				}
			}
			Safe_Release(pGameInstance);

			m_ListTchar.emplace_back(pStrState);
			m_ListTchar.emplace_back(pStrFullpath);
			m_ListTchar.emplace_back(pStrPrototypeTag);
			m_Listchar.emplace_back(szFullPath);
		}

	}


	wofstream	fOut;
	fOut.open(L"../../Data/PathName.txt");

	if (!fOut.fail())
	{
		for (auto& iter : m_PathList)
		{
			wstring RealPath = iter->wstrRealPathKey;
			RealPath = RealPath.substr(12, RealPath.size());
			RealPath = L".." + RealPath;
			fOut << iter->iCount << L"|" << iter->wstrObjKey << L"|"
				<< iter->wstrPath << L"|" << RealPath << endl;
		}
		fOut.close();
	}


	UpdateData(FALSE);
}

void CPathFinder::Safe_ReleaseList()
{
	if (!m_PathList.empty())
	{
		Safe_Release(m_pDevice);
		Safe_Release(m_pDeviceContext);
		for (auto& iter : m_PathList)
		{
			Safe_Delete(iter);
		}
		m_PathList.clear();
	}


	if (!m_Listchar.empty())
	{
		for (auto& iter : m_ListTchar)
		{
			Safe_Delete_Array(iter);
		}
		m_ListTchar.clear();
	}


	if (!m_Listchar.empty())
	{
		for (auto& iter : m_Listchar)
		{
			Safe_Delete_Array(iter);
		}
		m_Listchar.clear();
	}
	if (!m_ListFileName.empty())
	{
		for (auto& iter : m_ListFileName)
		{
			Safe_Delete_Array(iter);
		}
		m_ListFileName.clear();
	}
}

void CPathFinder::Set_Device()
{
}
