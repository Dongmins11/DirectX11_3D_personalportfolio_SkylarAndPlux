#pragma once

#include "stdafx.h"
#include "Tool_Defines.h"
#include "GameInstance.h"
#include "PathFinder.h"




class CFileInfo
{
public:
	static	CString		ConvertRelativePath(CString strFullPath);

	// ������ ��θ� �����ϱ� ���� �Լ�
	static	void		DirInfoExtraction(const wstring& wstrPath, list<CPathFinder::MODELPATH*>& rPathInfoList);
	
	// ���� ���� ���� ������ ��ȯ�ϴ� �Լ�
	static int		DirFileCnt(const wstring& wstrPath);

public:
	CFileInfo();
	~CFileInfo();
};

