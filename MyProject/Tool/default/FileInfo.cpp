#include "stdafx.h"
#include "FileInfo.h"
#include "Tool_Defines.h"



CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	// 상대 경로를 저장할 배열
	TCHAR		szRelativePath[MAX_PATH] = L"";
	// 현재 절대 경로를 저장하는 배열
	TCHAR		szCurrentDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurrentDirPath);

	//PathRelativePathTo : szCurrentDirPath에서 strFullPath로 가는 상대경로를 구해서
	//					   szRelativePath에게 저장해주는 함수(단, 같은 드라이브 내에서만 가능)

	PathRelativePathTo(szRelativePath, 
						szCurrentDirPath, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(),
						FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<CPathFinder::MODELPATH*>& rPathInfoList)
{
	wstring		strFilePath = wstrPath + L"\\*.*";

	CFileFind		Find;	

	BOOL		bContinue = Find.FindFile(strFilePath.c_str());
	
	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		// 찾은 파일의 이름이 .이거나 ..이면 건너뛴다.
		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// 현재 find객체가 찾은 파일의 경로를 넣어주면 재귀적 호출을 한다
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
	
		else // 찾은게 실제 리소스 파일이라면
		{
			if(Find.IsSystem())
				continue;

			CPathFinder::MODELPATH*		pMeshPath = new CPathFinder::MODELPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// 현재 경로에서 마지막 경로를 잘라내는 함수
			PathRemoveFileSpec(szPath);
			pMeshPath->iCount = DirFileCnt(szPath);

		// GetFileTitle: 파일의 파일 이름만 얻어오는 함수(확장자명도 안얻어옴)
		//	EX)L"AKIHA_AKI01_000"

			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(시작, 끝) : 시작과 끝에 해당하는 문자열을 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.fbx";
			// ex) wstrTextureName = L"AKIHA_AKI01_00"	+ "%d.png"

			TCHAR	szBuf[MAX_STR] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			// PathCombine(): 2,3인자값을 이어붙여서 완성된 경로를 1인자에 저장
			// 2, 3인자값 사이에 자동으로 "\\"가 삽입되어 저장
			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());
			PathRemoveFileSpec(szBuf);
			// 상대경로로 변환하여 구조체 멤버에 저장
			//	L"../Texture\Stage\Player\Attack\"
			//PathFindFileName : 파일 명을 찾거나 가장 말단의 폴더명을 찾아냄
			wstring PrototypeTag = L"Prototype_GameObject_";

			wstring aa = PathFindFileName(szBuf);
			wstring bb;
			wstring cc;
			bb = L"%d.fbx";
			cc = L"\\";
			pMeshPath->wstrPrototypeTag = PrototypeTag + aa;
			pMeshPath->wstrPath = aa + bb;
			pMeshPath->wstrStateKey = PathFindFileName(szBuf);
			PathRemoveFileSpec(szBuf);
			pMeshPath->wstrObjKey = PathFindFileName(szBuf);
			pMeshPath->wstrRealPathKey = ConvertRelativePath(szPath);
			pMeshPath->wstrRealPathKey = pMeshPath->wstrRealPathKey + cc;

			rPathInfoList.push_back(pMeshPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}	
	}
}

int CFileInfo::DirFileCnt(const wstring & wstrPath)
{
	wstring		strFilePath = wstrPath + L"\\*.fbx";	// 경로 + 파일의 이름.확장자명

	CFileFind		Find;		// mfc에서 제공하는 파일 및 경로를 제어하는 클래스

	// 주어진 경로에 파일의 유무를 확인하는 객체, 존재하지 않으면 false 존재하면 true 리턴
	// 만약 파일이 있고 실제 그 파일에 접근하고 싶을 때느 FindNextFile함수를 무조건 호출해야 한다.
	
	BOOL		bContinue = Find.FindFile(strFilePath.c_str());
	//	strFilePath에는 *.*이 붙어있는 상태다. 이 경우 해당 경로에 있는 하위 모든 파일을 탐색하는 것을 의미
	// 이 때 FindNextFile함수를 호출하면 경로에 있는 파일 중 가장 첫 번째 파일에 접근할 수 있다.
	
	int		iFileCount = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		// 찾은 파일이 시스템 파일인 경우 숫자를 세지 않고 건너뜀
		if(Find.IsSystem())
			continue;

		if (Find.IsDots())
			continue;

		if(!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}


