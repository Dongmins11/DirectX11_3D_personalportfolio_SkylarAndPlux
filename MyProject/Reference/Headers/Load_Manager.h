#pragma once

#include "Base.h"

BEGIN(Engine)

class CLoad_Manager final : public CBase
{
	DECLARE_SINGLETON(CLoad_Manager)
private:
	CLoad_Manager();
	virtual ~CLoad_Manager() = default;
public:
	HRESULT Set_Up_LoadFinder(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceCoantext);
public:
	HRESULT	Load_PathFinder_To_Model(const _tchar* tPath, _uint iLevel);
	HRESULT Load_GameObject_Tool(const _tchar* pFilePath,const _tchar* pMapPrototypeTag, _uint iLevel);
	HRESULT Load_Nevigation_To_Client(const _tchar* pFilePath, const _tchar* pPrototypeTag, _uint iLevel);
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
private:
	list<_tchar*>			m_ListPathFinderTcharTag;
	list<char*>				m_ListPathFinderCharTag;
	list<_tchar*>			m_ListObjectPairTag;
	list<_tchar*>			m_ListObjectPair;
private:
	list<MODELDESC_CLIENT*>		m_ListModelDesc;
	map<wstring, list<MODELDESC_CLIENT*>>	m_MapModelDesc;
public:
	virtual void Free() override;

};

END