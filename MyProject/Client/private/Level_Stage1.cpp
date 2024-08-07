#include "stdafx.h"
#include "..\public\Level_Stage1.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
#include "Level_Loading.h"
#include "Sound_Manager.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLevel(pDevice,pDeviceContext)
{
}

HRESULT CLevel_Stage1::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Read_For_LightDesc()))
		return E_FAIL;

	if (FAILED(Ready_For_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_For_Camera(L"Clone_Camera_Fly")))
		return E_FAIL;

	if (FAILED(Ready_For_Terrain(L"Clone_Terrain")))
		return E_FAIL;

	if (FAILED(Read_For_Ui()))
		return E_FAIL;

	if (FAILED(Ready_For_SkyBox(L"Clone_SkyBox")))
		return E_FAIL;

	if (FAILED(Ready_For_Effect()))
		return E_FAIL;


	if (FAILED(Read_For_GameObject_To_Layers()))
		return E_FAIL;


	CSound_Manager::Get_Instance()->Initialize();
	CSound_Manager::Get_Instance()->PlayBGM(L"Stage_One", 0.7f, CSound_Manager::ID_STAGEONE_BGM);
	return S_OK;
}

_int CLevel_Stage1::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (GetKeyState(VK_F6) & 0x8000)
		pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice,m_pDeviceContext, LEVEL_STAGE2));
	
	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

HRESULT CLevel_Stage1::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_For_Camera(const _tchar * pCloneTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMEREADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMEREADESC));
	CameraDesc.vEye = _float3(236.f, 20.f, 83.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(70.0f);
	CameraDesc.fAspect = g_iWinCX / (_float)g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPerSec = 10.f;

	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), LEVEL_STAGE1, pCloneTag, &CameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_For_Terrain(const _tchar * pCloneTag)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Terrain"), LEVEL_STAGE1, pCloneTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_For_SkyBox(const _tchar * pCloneTag)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_SkyBox"), LEVEL_STAGE1, pCloneTag)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Read_For_Ui()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Crystal_Ui"), LEVEL_STAGE1, L"Clone_Crystal_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Box_Ui"), LEVEL_STAGE1, L"Clone_Box_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Dead_Ui"), LEVEL_STAGE1, L"Clone_Dead_Ui")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_For_Meshes()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	//
	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_mTurret"), LEVEL_STAGE1, L"Clone_mTuerrt")))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Scaffolding"), LEVEL_STAGE1, L"Clone_Scaffolding")))
		return E_FAIL;*/

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeFloor"), LEVEL_STAGE1, L"Clone_ChangeFloor")))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeFloorTwo"), LEVEL_STAGE1, L"Clone_ChangeFloor")))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Vase"), LEVEL_STAGE1, L"Clone_Vase")))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_NpcCage"), LEVEL_STAGE1, L"Clone_NpcCage")))
		//return E_FAIL;

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_PetNpc"), LEVEL_STAGE1, L"Clone_PetNpc")))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ItemJetPack"), LEVEL_STAGE1, L"Clone_ItemJetPack")))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_JetPack_Ui"), LEVEL_STAGE1, L"Clone_JetPack_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Skylar"), LEVEL_STAGE1, L"Clone_Player")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ColliderObject"), LEVEL_STAGE1, L"Clone_Colider_Object")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_RopeRobot"), LEVEL_STAGE1, L"Clone_RopeRobot")))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_TimeSphere"), LEVEL_STAGE1, L"Clone_NpcCage")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_TimePedstal"), LEVEL_STAGE1, L"Clone_TimePedstal")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_TempleHead"), LEVEL_STAGE1, L"Clone_TempleHead")))
		return E_FAIL;*/

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Filp"), LEVEL_STAGE1, L"Clone_Effect_Filp")))
	//	return E_FAIL;


	
	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeCartoon"), LEVEL_STAGE1, L"Clone_TempleHead")))
	//	return E_FAIL;
	
	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_PunchRock"), LEVEL_STAGE1, L"Clone_PunchRock")))
	//	return E_FAIL;
	

	

	

	//MODELDESC_CLIENT tModelDesc;
	//ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));
	//tModelDesc.vPos = _float4(221.5f, 12.8f, 97.2f, 1.f);
	//tModelDesc.vScaile = 0.4f;
	//tModelDesc.vScaileXYZ = _float3(1.f, 1.6f, 1.f);
	//tModelDesc.viLevel = 1;


	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChuckRock"), LEVEL_STAGE1, L"Clone_ChuckRock", &tModelDesc)))
	//	return E_FAIL;

	
	

	//if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STAGE1, TEXT("Prototype_GameObject_MapObject"), LEVEL_STAGE1, L"Clone_MapObject")))
		//return E_FAIL;

	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Stage1::Read_For_LightDesc()
{
	LIGHTDESC	tLightDesc;
	ZeroMemory(&tLightDesc, sizeof(LIGHTDESC));

	tLightDesc.iType = LIGHTDESC::TYPE_DIRECTIONAL;
	tLightDesc.vDirection = _float4(0.f, -1.0f, 1.0f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, (143.f/255.f), (71.f/255.f), 1.f);
	tLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	CGameInstance* pGameIstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameIstance);

	pGameIstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDesc);

	Safe_Release(pGameIstance);
	return S_OK;
}

HRESULT CLevel_Stage1::Read_For_GameObject_To_Layers()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Load_GameObject_Tool(L"../../Data/MapTool_ObjectStage7Main.dat",L"Prototype_GameObject_MapObject",LEVEL_STAGE1)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_For_Effect()
{

	return S_OK;
}

CLevel_Stage1 * CLevel_Stage1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_Stage1* pLevelInstance = new CLevel_Stage1(pDevice, pDeviceContext);

	if (FAILED(pLevelInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create Level_Stage1");
		Safe_Release(pLevelInstance);
	}

	return pLevelInstance;
}

void CLevel_Stage1::Free()
{
	__super::Free();
}
