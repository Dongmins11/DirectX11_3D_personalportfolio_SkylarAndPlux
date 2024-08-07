#pragma once

#include "stdafx.h"
#include "Tool_Defines.h"
#include "GameInstance.h"
#include "PathFinder.h"




class CFileInfo
{
public:
	static	CString		ConvertRelativePath(CString strFullPath);

	// 폴더의 경로를 추출하기 위한 함수
	static	void		DirInfoExtraction(const wstring& wstrPath, list<CPathFinder::MODELPATH*>& rPathInfoList);
	
	// 폴더 안의 파일 개수를 반환하는 함수
	static int		DirFileCnt(const wstring& wstrPath);

public:
	CFileInfo();
	~CFileInfo();
};

