#include "stdafx.h"
#include "..\public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "Boss_Stage.h"
#include "Loading_Ui.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Read_For_Loading_Texture(L"Clone_Loading_Ui")))
		return E_FAIL;

	m_eNextLevel = eNextLevel;
	m_pLoader = CLoader::Create(m_pDevice, m_pDeviceContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;


	return S_OK;
}

_int CLevel_Loading::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (true == m_pLoader->Get_Finished())
	{
		m_TimeAcc += TimeDelta;

		if (m_TimeAcc < 1.0)
			return 0;

		m_TimeAcc = 0.0;

		CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CLevel*		pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			pLevel = CLevel_Logo::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_STAGE1:
			pLevel = CLevel_Stage1::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_STAGE2:
			pLevel = CLevel_Stage2::Create(m_pDevice, m_pDeviceContext);
			break;
		case LEVEL_BOSS_STAGE:
			pLevel = CBoss_Stage::Create(m_pDevice, m_pDeviceContext);
			break;
		}

		if (nullptr == pLevel)
			return 0;

		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pLevel)))
			return 0;

		Safe_Release(pGameInstance);
	}

	return _int();
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pLoader->Draw_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Read_For_Loading_Texture(const _tchar * pCloneTag)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* for.Prototype_GameObject_Loading_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_Loading_Ui"), CLoading_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_Loading_Ui"), LEVEL_LOADING, pCloneTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
	{
		MSGBOX("Failed to Created CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);


}
