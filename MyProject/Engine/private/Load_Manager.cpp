#include "..\public\Load_Manager.h"
#include "Component_Manager.h"
#include "GameObject_Manager.h"
#include <fstream>

IMPLEMENT_SINGLETON(CLoad_Manager)

CLoad_Manager::CLoad_Manager()
{
}

HRESULT CLoad_Manager::Set_Up_LoadFinder(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceCoantext)
{
	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pDeviceContext = pDeviceCoantext;
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CLoad_Manager::Load_PathFinder_To_Model(const _tchar * tPath, _uint iLevel)
{
	if (!m_ListPathFinderTcharTag.empty())
	{
		for (auto& TcharIter : m_ListPathFinderTcharTag)
			Safe_Delete_Array(TcharIter);

		m_ListPathFinderTcharTag.clear();
	}

	if (!m_ListPathFinderCharTag.empty())
	{
		for (auto& CharIter : m_ListPathFinderCharTag)
			Safe_Delete_Array(CharIter);

		m_ListPathFinderCharTag.clear();
	}

	wifstream		fin;

	fin.open(tPath);

	if (!fin.fail())
	{
		_tchar	szCount[MAX_PATH] = L"";
		_tchar	szObjectKey[MAX_PATH] = L"";
		_tchar	szModelKey[MAX_PATH] = L"";
		_tchar	szFullPath[MAX_PATH] = L"";

		_uint	iCount = 0;

		while (true)
		{
			fin.getline(szCount, MAX_PATH, '|');
			iCount = _wtoi(szCount);
			fin.getline(szObjectKey, MAX_PATH, '|');
			fin.getline(szModelKey, MAX_PATH, '|');
			fin.getline(szFullPath, MAX_PATH);

			if (fin.eof())
				break;


			for (_uint i = 0; i < iCount; ++i)
			{
				_tchar*	pPrototypeTag = new _tchar[MAX_PATH];
				_tchar* pObjectKey = new _tchar[MAX_PATH];
				_tchar* pModelKey = new _tchar[MAX_PATH];
				_tchar* pFullPath = new _tchar[MAX_PATH];

				lstrcpy(pPrototypeTag, szModelKey);
				lstrcpy(pObjectKey, szObjectKey);
				lstrcpy(pModelKey, szModelKey);
				lstrcpy(pFullPath, szFullPath);

				m_ListPathFinderTcharTag.emplace_back(pPrototypeTag);
				m_ListPathFinderTcharTag.emplace_back(pObjectKey);
				m_ListPathFinderTcharTag.emplace_back(pModelKey);
				m_ListPathFinderTcharTag.emplace_back(pFullPath);

				wsprintf(pPrototypeTag, pPrototypeTag, i);
				wsprintf(szObjectKey, szObjectKey, i);
				wsprintf(pModelKey, pModelKey, i);

				

				_matrix		PivotMatrix;

				char*		pCharFullPath = new char[MAX_PATH];
				WideCharToMultiByte(CP_ACP, 0, pFullPath, MAX_PATH, pCharFullPath, MAX_PATH, nullptr, nullptr);
				m_ListPathFinderCharTag.emplace_back(pCharFullPath);

				char*		pCharModelName = new char[MAX_PATH];
				WideCharToMultiByte(CP_ACP, 0, pModelKey, MAX_PATH, pCharModelName, MAX_PATH, nullptr, nullptr);
				m_ListPathFinderCharTag.emplace_back(pCharModelName);

				if (!lstrcmp(szObjectKey, L"Anim"))
				{
					CComponent_Manager*	pComponentInstance = CComponent_Manager::Get_Instance();
					Safe_AddRef(pComponentInstance);

					PivotMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

					if (FAILED(pComponentInstance->Add_Prototype(iLevel,const_cast<const _tchar*>(pPrototypeTag),
						CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,pCharFullPath, pCharModelName, PivotMatrix))))
					{
						return E_FAIL;
					}

					Safe_Release(pComponentInstance);
				}
				else
				{
					CComponent_Manager*	pComponentInstance = CComponent_Manager::Get_Instance();
					Safe_AddRef(pComponentInstance);

					PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f));

					if (FAILED(pComponentInstance->Add_Prototype(iLevel, const_cast<const _tchar*>(pPrototypeTag),
						CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, pCharFullPath, pCharModelName, PivotMatrix))))
					{
						return E_FAIL;
					}

					Safe_Release(pComponentInstance);
				}

			}
		}
		fin.close();
	}


	return S_OK;
}

HRESULT CLoad_Manager::Load_GameObject_Tool(const _tchar * pFilePath, const _tchar* pMapPrototypeTag, _uint iLevel)
{

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_MapModelDesc.empty())
	{
		for (auto& Pair : m_MapModelDesc)
		{
			for (auto& iter : Pair.second)
			{
				Safe_Delete(iter);
			}
			Pair.second.clear();
		}
		m_MapModelDesc.clear();
	}


	DWORD	dwPairName = 0;
	DWORD	dwStrPrototype = 0; //레이어의프로토타입녀석
	DWORD	dwStrObjectKey = 0; //맵인지 애니메이션인지 구별할녀석DWORD dwByte = 0;
	DWORD	dwStrPath = 0; //레이어태그 //모델이름
	DWORD	dwByte = 0;

	_tchar*	pPairName = nullptr;
	_tchar*	pPrototypeTag = nullptr;
	_tchar*	pObjectKey = nullptr;
	_tchar*	pPath = nullptr;

	_uint		iType;
	_uint		viLevel;
	_float		vScaile;
	_float3		vScaileXYZ;
	_float3		vRotaition;
	_float4		vPos;


	while (true)
	{
		ReadFile(hFile, &dwStrPrototype, sizeof(DWORD), &dwByte, nullptr);
		pPrototypeTag = new _tchar[dwStrPrototype];
		ReadFile(hFile, pPrototypeTag, dwStrPrototype, &dwByte, nullptr);

		ReadFile(hFile, &dwStrObjectKey, sizeof(DWORD), &dwByte, nullptr);
		pObjectKey = new _tchar[dwStrObjectKey];
		ReadFile(hFile, pObjectKey, dwStrObjectKey, &dwByte, nullptr);

		ReadFile(hFile, &dwStrPath, sizeof(DWORD), &dwByte, nullptr);
		pPath = new _tchar[dwStrPath];
		ReadFile(hFile, pPath, dwStrPath, &dwByte, nullptr);

		pPairName = new _tchar[MAX_PATH];
		lstrcpy(pPairName, pPath);

		ReadFile(hFile, &iType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &viLevel, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &vScaile, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &vScaileXYZ, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &vRotaition, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_float4), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete_Array(pPairName);
			Safe_Delete_Array(pPrototypeTag);
			Safe_Delete_Array(pObjectKey);
			Safe_Delete_Array(pPath);
			break;
		}


		MODELDESC_CLIENT*	pModelDesc = new MODELDESC_CLIENT;
		pModelDesc->wstrPrototypeTag = pPrototypeTag;
		pModelDesc->wstrObjKey = pObjectKey;
		pModelDesc->wstrPath = pPath;

		pModelDesc->iType = iType;
		pModelDesc->viLevel = viLevel;
		pModelDesc->vScaile = vScaile;
		pModelDesc->vScaileXYZ = vScaileXYZ;
		pModelDesc->vRotaition = vRotaition;
		pModelDesc->vPos = vPos;


		auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(pPairName));

		if (iter == m_MapModelDesc.end())
		{
			m_ListModelDesc.emplace_back(pModelDesc);
			m_MapModelDesc.emplace(pPairName, m_ListModelDesc);
			m_ListModelDesc.clear();
		}
		else
			iter->second.emplace_back(pModelDesc);

		m_ListObjectPairTag.emplace_back(pPairName);
		m_ListObjectPairTag.emplace_back(pPrototypeTag);
		m_ListObjectPairTag.emplace_back(pObjectKey);
		m_ListObjectPairTag.emplace_back(pPath);

	}
	CloseHandle(hFile);

	for (auto& Pair : m_MapModelDesc)
	{
		auto PairObject = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(Pair.first.c_str()));

		if (PairObject != m_MapModelDesc.end())
		{
			_uint iIndex = 0;

			for (auto& PairSecond : Pair.second)
			{
				if (!lstrcmp(PairSecond->wstrObjKey,L"Anim"))
				{

					CGameObject_Manager*	pGameObjectManager = CGameObject_Manager::Get_Instance();
					Safe_AddRef(pGameObjectManager);

					MODELDESC_CLIENT* pModelDesc = new MODELDESC_CLIENT;
					memcpy(pModelDesc, PairSecond, sizeof(MODELDESC_CLIENT));

					_tchar*	pStrPath = new _tchar[MAX_PATH];
					_tchar*	pStrPath2 = new _tchar[MAX_PATH];

					lstrcpy(pStrPath, PairSecond->wstrPrototypeTag);
					lstrcpy(pStrPath2, PairSecond->wstrPath);

					pGameObjectManager->Add_ObjectToLayer(iLevel, pStrPath, iLevel,
						pStrPath2, pModelDesc);

					Safe_Delete(pModelDesc);
					m_ListObjectPair.emplace_back(pStrPath);
					m_ListObjectPair.emplace_back(pStrPath2);

					Safe_Release(pGameObjectManager);
				}
				if (!lstrcmp(PairSecond->wstrObjKey, L"Map"))
				{
					CGameObject_Manager*	pGameObjectManager = CGameObject_Manager::Get_Instance();
					Safe_AddRef(pGameObjectManager);

					MODELDESC_CLIENT* pModelDesc = new MODELDESC_CLIENT;
					memcpy(pModelDesc, PairSecond, sizeof(MODELDESC_CLIENT));

					_tchar*	pStrPath = new _tchar[MAX_PATH];
					_tchar*	pStrPath2 = new _tchar[MAX_PATH];

					lstrcpy(pStrPath, PairSecond->wstrPrototypeTag);
					lstrcpy(pStrPath2, PairSecond->wstrPath);

					pGameObjectManager->Add_ObjectToLayer(iLevel, pMapPrototypeTag, iLevel,
						pStrPath2, pModelDesc);

					Safe_Delete(pModelDesc);
					m_ListObjectPair.emplace_back(pStrPath);
					m_ListObjectPair.emplace_back(pStrPath2);

					Safe_Release(pGameObjectManager);
				}
				if (!lstrcmp(PairSecond->wstrObjKey, L"Filter"))
				{

					CGameObject_Manager*	pGameObjectManager = CGameObject_Manager::Get_Instance();
					Safe_AddRef(pGameObjectManager);

					MODELDESC_CLIENT* pModelDesc = new MODELDESC_CLIENT;
					memcpy(pModelDesc, PairSecond, sizeof(MODELDESC_CLIENT));

					_tchar*	pStrPath = new _tchar[MAX_PATH];
					_tchar*	pStrPath2 = new _tchar[MAX_PATH];

					lstrcpy(pStrPath, PairSecond->wstrPrototypeTag);
					lstrcpy(pStrPath2, PairSecond->wstrPath);

					if(FAILED(pGameObjectManager->Add_ObjectToLayer(iLevel, pStrPath, iLevel,pStrPath2, pModelDesc)))
						return S_OK;

					Safe_Delete(pModelDesc);
					m_ListObjectPair.emplace_back(pStrPath);
					m_ListObjectPair.emplace_back(pStrPath2);

					Safe_Release(pGameObjectManager);
				}
				if (!lstrcmp(PairSecond->wstrObjKey, L"Rotation"))
				{

					CGameObject_Manager*	pGameObjectManager = CGameObject_Manager::Get_Instance();
					Safe_AddRef(pGameObjectManager);

					MODELDESC_CLIENT* pModelDesc = new MODELDESC_CLIENT;
					memcpy(pModelDesc, PairSecond, sizeof(MODELDESC_CLIENT));

					_tchar*	pStrPath = new _tchar[MAX_PATH];
					_tchar*	pStrPath2 = new _tchar[MAX_PATH];

					lstrcpy(pStrPath, PairSecond->wstrPrototypeTag);
					lstrcpy(pStrPath2, PairSecond->wstrPath);

					pGameObjectManager->Add_ObjectToLayer(iLevel, pStrPath, iLevel,
						pStrPath2, pModelDesc);

					Safe_Delete(pModelDesc);
					m_ListObjectPair.emplace_back(pStrPath);
					m_ListObjectPair.emplace_back(pStrPath2);

					Safe_Release(pGameObjectManager);
				}

			}
		}
	}	
	return S_OK;
}

HRESULT CLoad_Manager::Load_Nevigation_To_Client(const _tchar * pFilePath, const _tchar * pPrototypeTag, _uint iLevel)
{
	return S_OK;
}


void CLoad_Manager::Free()
{


	if (!m_MapModelDesc.empty())
	{
		for (auto& Pair : m_MapModelDesc)
		{
			for (auto& iter : Pair.second)
			{
				Safe_Delete(iter);
			}
			Pair.second.clear();
		}
		m_MapModelDesc.clear();
	}


	if (!m_ListObjectPairTag.empty())
	{
		for (auto& iter : m_ListObjectPairTag)
		{
			Safe_Delete_Array(iter);
		}
		m_ListObjectPairTag.clear();
	}

	if (!m_ListObjectPair.empty())
	{
		for (auto& TcharIter : m_ListObjectPair)
			Safe_Delete_Array(TcharIter);

		m_ListObjectPair.clear();
	}


	if (!m_ListPathFinderTcharTag.empty())
	{
		for (auto& TcharIter : m_ListPathFinderTcharTag)
			Safe_Delete_Array(TcharIter);

		m_ListPathFinderTcharTag.clear();
	}



	if (!m_ListPathFinderCharTag.empty())
	{
		for (auto& CharIter : m_ListPathFinderCharTag)
			Safe_Delete_Array(CharIter);

		m_ListPathFinderCharTag.clear();
	}

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
