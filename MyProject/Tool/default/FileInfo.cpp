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
	// ��� ��θ� ������ �迭
	TCHAR		szRelativePath[MAX_PATH] = L"";
	// ���� ���� ��θ� �����ϴ� �迭
	TCHAR		szCurrentDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurrentDirPath);

	//PathRelativePathTo : szCurrentDirPath���� strFullPath�� ���� ����θ� ���ؼ�
	//					   szRelativePath���� �������ִ� �Լ�(��, ���� ����̺� �������� ����)

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

		// ã�� ������ �̸��� .�̰ų� ..�̸� �ǳʶڴ�.
		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// ���� find��ü�� ã�� ������ ��θ� �־��ָ� ����� ȣ���� �Ѵ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}
	
		else // ã���� ���� ���ҽ� �����̶��
		{
			if(Find.IsSystem())
				continue;

			CPathFinder::MODELPATH*		pMeshPath = new CPathFinder::MODELPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// ���� ��ο��� ������ ��θ� �߶󳻴� �Լ�
			PathRemoveFileSpec(szPath);
			pMeshPath->iCount = DirFileCnt(szPath);

		// GetFileTitle: ������ ���� �̸��� ������ �Լ�(Ȯ���ڸ� �Ⱦ���)
		//	EX)L"AKIHA_AKI01_000"

			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(����, ��) : ���۰� ���� �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.fbx";
			// ex) wstrTextureName = L"AKIHA_AKI01_00"	+ "%d.png"

			TCHAR	szBuf[MAX_STR] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			// PathCombine(): 2,3���ڰ��� �̾�ٿ��� �ϼ��� ��θ� 1���ڿ� ����
			// 2, 3���ڰ� ���̿� �ڵ����� "\\"�� ���ԵǾ� ����
			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());
			PathRemoveFileSpec(szBuf);
			// ����η� ��ȯ�Ͽ� ����ü ����� ����
			//	L"../Texture\Stage\Player\Attack\"
			//PathFindFileName : ���� ���� ã�ų� ���� ������ �������� ã�Ƴ�
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
	wstring		strFilePath = wstrPath + L"\\*.fbx";	// ��� + ������ �̸�.Ȯ���ڸ�

	CFileFind		Find;		// mfc���� �����ϴ� ���� �� ��θ� �����ϴ� Ŭ����

	// �־��� ��ο� ������ ������ Ȯ���ϴ� ��ü, �������� ������ false �����ϸ� true ����
	// ���� ������ �ְ� ���� �� ���Ͽ� �����ϰ� ���� ���� FindNextFile�Լ��� ������ ȣ���ؾ� �Ѵ�.
	
	BOOL		bContinue = Find.FindFile(strFilePath.c_str());
	//	strFilePath���� *.*�� �پ��ִ� ���´�. �� ��� �ش� ��ο� �ִ� ���� ��� ������ Ž���ϴ� ���� �ǹ�
	// �� �� FindNextFile�Լ��� ȣ���ϸ� ��ο� �ִ� ���� �� ���� ù ��° ���Ͽ� ������ �� �ִ�.
	
	int		iFileCount = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		// ã�� ������ �ý��� ������ ��� ���ڸ� ���� �ʰ� �ǳʶ�
		if(Find.IsSystem())
			continue;

		if (Find.IsDots())
			continue;

		if(!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}


