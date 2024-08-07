#pragma once


#include "Tool_Defines.h"
#include "GameInstance.h"
// CPathFinder 대화 상자입니다.

class CPathFinder : public CDialogEx
{
	DECLARE_DYNAMIC(CPathFinder)
public:
	typedef struct tagModelPath
	{
		wstring			wstrPrototypeTag;
		wstring			wstrObjKey;
		wstring			wstrStateKey;
		wstring			wstrPath;
		wstring			wstrRealPathKey;
		int				iCount;
	}MODELPATH;
public:
	CPathFinder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFinder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFINDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPathFinder();
private:
	void	Safe_ReleaseList();
	void	Set_Device();

public:
	MODELPATH*				m_pModelPath;
	list<MODELPATH*>		m_PathList;
	list<_tchar*>			m_ListTchar;
	list<char*>				m_Listchar;
	list<_tchar*>			m_ListFileName;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
};
