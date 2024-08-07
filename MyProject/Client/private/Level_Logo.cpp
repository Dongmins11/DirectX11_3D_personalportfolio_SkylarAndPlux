#include "stdafx.h"
#include "..\public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLevel(pDevice,pDeviceContext)
{
}

HRESULT CLevel_Logo::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_For_BackGround(L"Clone_BackGround")))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Logo::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_STAGE1))))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
	}

	return _int();
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("Level_Logo"));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_For_BackGround(const _tchar * pCloneTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_LOGO, L"Prototype_GameObject_BackGround", LEVEL_LOGO, pCloneTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Logo* pLevelInstance = new CLevel_Logo(pDevice, pDeviceContext);

	if (FAILED(pLevelInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create Level_Logo");
		Safe_Release(pLevelInstance);
	}

	return pLevelInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
