#include "stdafx.h"
#include "..\public\Level_Stage2.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
#include "Sound_Manager.h"

CLevel_Stage2::CLevel_Stage2(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Stage2::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_For_Camera(L"Clone_Camera_Fly")))
		return E_FAIL;

	if (FAILED(Ready_For_UI()))
		return E_FAIL;

	CSound_Manager::Get_Instance()->Initialize();
	CSound_Manager::Get_Instance()->PlayBGM(L"Stage_Two", 0.5f, CSound_Manager::ID_STAGETWO_BGM);

	return S_OK;
}

_int CLevel_Stage2::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;


	return _int();
}

HRESULT CLevel_Stage2::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_For_Camera(const _tchar * pCloneTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_JetPack_Ui"), LEVEL_STAGE2, L"Clone_JetPack_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_Skylar"), LEVEL_STAGE2, L"Clone_Player")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_JetPack"), LEVEL_STAGE2, L"Clone_JetPack")))
		return E_FAIL;


	CCamera::CAMEREADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMEREADESC));
	CameraDesc.vEye = _float3(236.f, 20.f, 83.f);
	CameraDesc.vAt = _float3(235.f, 20.f, 83.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(90.0f);
	CameraDesc.fAspect = g_iWinCX / (_float)g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 700.f;

	CameraDesc.TransformDesc.SpeedPerSec = 10.f;

	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), LEVEL_STAGE2, pCloneTag, &CameraDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_SkyBox"), LEVEL_STAGE2,L"Clone_SkyBox")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_ColliderObject"), LEVEL_STAGE2, L"Clone_Colider_Object")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_ItemMagenetic"), LEVEL_STAGE2, L"Clone_ItemMagenetic")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_BallHole"), LEVEL_STAGE2, L"Clone_BallHole")))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_GravitionalField"), LEVEL_STAGE2, L"Clone_Gravity_Field")))
		return E_FAIL;

	if (FAILED(pGameInstance->Load_GameObject_Tool(L"../../Data/MapTool_ObjectStage25Main.dat", L"Prototype_GameObject_MapObject", LEVEL_STAGE2)))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage2::Ready_For_UI()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_Crystal_Ui"), LEVEL_STAGE2, L"Clone_Crystal_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_Box_Ui"), LEVEL_STAGE2, L"Clone_Box_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_Dead_Ui"), LEVEL_STAGE2, L"Clone_Dead_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE2, TEXT("Prototype_GameObject_FadeInOut_UI"), LEVEL_STAGE2, L"Clone_FadeInOut_Ui")))
		return E_FAIL;


	if (FAILED(pGameInstance->Clear_Light()))
		return E_FAIL;

	LIGHTDESC	tLightDesc;
	ZeroMemory(&tLightDesc, sizeof(LIGHTDESC));

	tLightDesc.iType = LIGHTDESC::TYPE_DIRECTIONAL;
	tLightDesc.vDirection = _float4(0.f, -1.0f, 1.0f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, (143.f / 255.f), (71.f / 255.f), 1.f);
	tLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	CGameInstance* pGameIstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameIstance);

	pGameIstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDesc);


	LIGHTDESC	tLightDescTwo;
	ZeroMemory(&tLightDescTwo, sizeof(LIGHTDESC));

	tLightDescTwo.iType = tagLightDesc::TYPE_POINT;
	tLightDescTwo.vPosition = _float4(10.f, 3.f, 5.f, 0.f);
	tLightDescTwo.fRange = 30.f;
	tLightDescTwo.vDiffuse = _float4((139.f / 255.f), 0.f, 1.f, 1.f);
	tLightDescTwo.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDescTwo.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDescTwo)))
		return E_FAIL;


	Safe_Release(pGameIstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Stage2 * CLevel_Stage2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Stage2* pLevelInstance = new CLevel_Stage2(pDevice, pDeviceContext);

	if (FAILED(pLevelInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create Level_Stage2");
		Safe_Release(pLevelInstance);
	}

	return pLevelInstance;
}

void CLevel_Stage2::Free()
{
	__super::Free();
}
