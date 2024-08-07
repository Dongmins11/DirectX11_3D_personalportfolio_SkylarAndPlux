#include "stdafx.h"
#include "..\public\Boss_Stage.h"
#include "GameInstance.h"
#include "Camera_Fly.h"
#include "Sound_Manager.h"

CBoss_Stage::CBoss_Stage(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLevel(pDevice, pDeviceContext)
{
}

HRESULT CBoss_Stage::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	if (FAILED(Ready_For_Camera(L"Clone_Camera_Fly")))
		return E_FAIL;

	if (FAILED(Ready_For_Object()))
		return E_FAIL;

	if (FAILED(Ready_For_Ui()))
		return E_FAIL;

	CSound_Manager::Get_Instance()->Initialize();

	return S_OK;
}

_int CBoss_Stage::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	return _int();
}

HRESULT CBoss_Stage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBoss_Stage::Ready_For_Camera(const _tchar * pCloneTag)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::CAMEREADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMEREADESC));
	CameraDesc.vEye = _float3(0.f, 10.f, -8.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = g_iWinCX / (_float)g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPerSec = 10.f;
	CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_JetPack_Ui"), LEVEL_BOSS_STAGE, L"Clone_JetPack_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Skylar"), LEVEL_BOSS_STAGE, L"Clone_Player")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_JetPack"), LEVEL_BOSS_STAGE, L"Clone_JetPack")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Magenetic"), LEVEL_BOSS_STAGE, L"Clone_Magenetic")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Fly"), LEVEL_BOSS_STAGE, pCloneTag, &CameraDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBoss_Stage::Ready_For_Object()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_SkyBox"), LEVEL_BOSS_STAGE, L"Clone_SkyBox")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ColliderObject"), LEVEL_BOSS_STAGE, L"Clone_Colider_Object")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossCRT"), LEVEL_BOSS_STAGE, L"Clone_Boss_CRT")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_NpcSphere"), LEVEL_BOSS_STAGE, L"Clone_Npc_Sphere")))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_FireCrt"), LEVEL_BOSS_STAGE, L"Clone_Effect_FireCrt")))
		return E_FAIL;


	if (FAILED(pGameInstance->Load_GameObject_Tool(L"../../Data/Bosst_MapTest3.dat", L"Prototype_GameObject_MapObject", LEVEL_BOSS_STAGE)))
		return E_FAIL;



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBoss_Stage::Ready_For_Ui()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);




	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Crystal_Ui"), LEVEL_BOSS_STAGE, L"Clone_Crystal_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Box_Ui"), LEVEL_BOSS_STAGE, L"Clone_Box_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Magenetic_Ui"), LEVEL_BOSS_STAGE, L"Clone_Magenetic_Ui")))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Dead_Ui"), LEVEL_BOSS_STAGE, L"Clone_Dead_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_FadeInOut_UI"), LEVEL_BOSS_STAGE, L"Clone_FadeInOut_Ui")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_End_UI"), LEVEL_BOSS_STAGE, L"Clone_End_Ui")))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Clear_Light()))
		return E_FAIL;

	LIGHTDESC	tLightDesc;
	ZeroMemory(&tLightDesc, sizeof(LIGHTDESC));

	tLightDesc.iType = LIGHTDESC::TYPE_DIRECTIONAL;
	tLightDesc.vDirection = _float4(0.f, -1.0f, 1.0f, 0.f);
	tLightDesc.vDiffuse = _float4((171.f/255.f), (215.f / 255.f), (158.f/255.f), 1.f);
	tLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	pGameInstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDesc);


	LIGHTDESC	tLightDescTwo;
	ZeroMemory(&tLightDescTwo, sizeof(LIGHTDESC));

	tLightDescTwo.iType = tagLightDesc::TYPE_POINT;
	tLightDescTwo.vPosition = _float4(10.f, 3.f, 5.f,0.f);
	tLightDescTwo.fRange = 30.f;
	tLightDescTwo.vDiffuse = _float4((139.f / 255.f), 0.f, 1.f, 1.f);
	tLightDescTwo.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDescTwo.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Set_LightDesc(m_pDevice, m_pDeviceContext, tLightDescTwo)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CBoss_Stage * CBoss_Stage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBoss_Stage* pLevelInstance = new CBoss_Stage(pDevice, pDeviceContext);

	if (FAILED(pLevelInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Create Boss_Stage");
		Safe_Release(pLevelInstance);
	}

	return pLevelInstance;
}

void CBoss_Stage::Free()
{
	__super::Free();
}
