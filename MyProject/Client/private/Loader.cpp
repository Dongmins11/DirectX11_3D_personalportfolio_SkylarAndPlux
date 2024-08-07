#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Camera_Fly.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Hp_Ui.h"
#include "Crystal_Ui.h"
#include "Box_Ui.h"
#include "Dead_Ui.h"
#include "Talk_UI.h"
#include "JetPack_Ui.h"
#include "Magenetic_Ui.h"
#include "FadeInOut_UI.h"
#include "Boss_Ui.h"
#include "BossNpc.h"
#include "Player.h"
#include "Rock.h"
#include "MapObject.h"
#include "ChuckRock.h"
#include "DesertRock.h"
#include "BigDesertRock.h"
#include "DesertWater.h"
#include "mTurret.h"
#include "Turret_Bullet.h"
#include "Collider_Object.h"
#include "DeadColider_Object.h"
#include "SaveCollider_Object.h"
#include "AnimCollider_Object.h"
#include "Scaffolding.h"
#include "ChangeFloor.h"
#include "RopeRobot.h"
#include "Colleter.h"
#include "Vase.h"
#include "NpcCage.h"
#include "NpcSphere.h"
#include "Npc.h"
#include "PetNpc.h"
#include "TalkNpc.h"
#include "BossPet.h"
#include "TimeSphere.h"
#include "TempleHead.h"
#include "ChangeFloorTwo.h"
#include "ChangeFloorThree.h"
#include "ChangeFloorFor.h"
#include "TimePedstal.h"
#include "ItemJetPack.h"
#include "JetPack.h"
#include "ItemMagenetic.h"
#include "Magenetic.h"
#include "Magenetic_Skill.h"
#include "ChangeCartoon.h"
#include "PunchRock.h"
#include "Defferde_TimeSphere.h"
#include "Potal.h"
#include "TimeRenderObject.h"
#include "Effect_Flip.h"
#include "FireAltar.h"
#include "LavaRotation.h"
#include "Lava.h"
#include "RotationBlock.h"
#include "BallHole.h"
#include "RotationBall.h"
#include "GravityMesh.h"
#include "GravitionalField.h"
#include "MoveBlock.h"
#include "MoveBlockTwo.h"
#include "MoveBlockThree.h"
#include "BossPotal.h"
#include "TestMesh.h"

#include "Effect_Rect.h"
#include "Effect_Point.h"
#include "Effect_Boss.h"
#include "Effect_Roket.h"
#include "Effect_RoketHit.h"
#include "Effect_Magenetic.h"
#include "Effect_JetPack.h"
#include "Effect_SwingAttack.h"
#include "Effect_FireCrt.h"
#include "Collector_Point.h"


#include "Render_BossHead.h"
#include "BossCRT.h"

#include "Left_Bullet.h"
#include "Right_Bullet.h"

#include "End_Ui.h"
#include "Appear_Ui.h"
#include "EndScene.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

}

unsigned int APIENTRY EntryMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(pLoader->Get_CriticalSection());

	HRESULT		hr = 0;

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_STAGE1:
		hr = pLoader->Loading_For_Stage1_Level();
		break;
	case LEVEL_STAGE2:
		hr = pLoader->Loading_For_Stage2_Level();
		break;
	case LEVEL_BOSS_STAGE:
		hr = pLoader->Loading_For_BossStage_Level();
		break;
	}

	if (FAILED(hr))
		return 0;

	LeaveCriticalSection(pLoader->Get_CriticalSection());

	return 0;

}

HRESULT CLoader::NativeConstruct(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, EntryMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	wsprintf(m_szLoading, TEXT("로고 게임 리소스를 로드 중입니다. "));



	wsprintf(m_szLoading, TEXT("로고 게임 원형객체를 로드 중입니다. "));


	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	wsprintf(m_szLoading, TEXT("로고 로딩이 완료되었습니다. "));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage1_Level()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	/* for.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_SkyBox"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/Dongmin/SkyBox%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_FilterNormal"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/Filter/SandNomal.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterDiffuse */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_FilterDiffuse"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/Filter/SandTest%d.dds"),2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Filter"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/Filter/Base%d.png"), 3))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Chat */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Chat"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Talk/Talk0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Talk */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Talk"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Chat/Chat%d.png"),3))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Effect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Effect%d.dds"),  9))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect_Png */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Effect_Png"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/EffectPng/EffectPng%d.png"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect_Filp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Effect_Filp"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect_Flip/Effect_Flip%d.png"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_BossEffect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_BossEffect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/BossEffect/BossEffect%d.dds"), 5))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_BossEffect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Texture_Dissolve"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Dissolve/dissolve%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Navigation"), CNavigation::Create(m_pDevice, m_pDeviceContext,L"../../Data/NaviMesh.dat"))))
		return E_FAIL;

	///* for.Prototype_Component_VIbuffer_Cube */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_VIBuffer_Navigation"), CNavigation::Create(m_pDevice, m_pDeviceContext, L"../../Data/NaviData.dat"))))
	//	return E_FAIL;


	/* For.Prototype_GameObject_Collider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ColliderObject"), CCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_DeadObject"), CDeadColider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_SaveObject"), CSaveCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_AABB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_OBB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_Component_Collider_SPHERE"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_MapObject"), CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Skylar"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_JetPack"), CJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Fiona"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_mTurret"), CmTerret::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Turret_Bullet"), CTurret_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChuckRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChuckRock"), CChuckRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BigDesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_BigDesertRock"), CBigDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_DeserteRock"), CDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertWater */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_DesertWater"), CDesertWater::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Scaffolding */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Scaffolding"), CScaffolding::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeFloor"), ChangeFloor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeFloorTwo"), CChangeFloorTwo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RopeRobot */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_RopeRobot"), CRopeRobot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Collector"), CColleter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Vase */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Vase"), CVase::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_NpcCage */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_NpcCage"), CNpcCage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Npc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Npc"), CNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PetNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_PetNpc"), CPetNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TalkNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_TalkNpc"), CTalkNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_TimeSphere"), CTimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimePedstal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_TimePedstal"), CTimePedstal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_TempleHead"), CTempleHead::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemJetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ItemJetPack"), CItemJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeCartoon */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_ChangeCartoon"), CChangeCartoon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PunchRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_PunchRock"), CPunchRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Deffered_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Deffered_TimeSphere"), CDefferde_TimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeRenderObject */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_TimeRenderObject"), CTimeRenderObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Potal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Potal"), CPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Filp */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Filp"), CEffect_Flip::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FireAltar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_FireAltar"), CFireAltar::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////For.Effect/////////////////////////////////////////////////////////////////

	/* For.Prototype_GameObject_Effect_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Rect"), CEffect_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Point"), CEffect_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Collector_Point"), CCollector_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Effect_Roket */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Roket"), CEffect_Roket::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_RoketHit */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_RoketHit"), CEffect_RoketHit::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Magenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_Magenetic"), CEffect_Magenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_JetPack"), CEffect_JetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_SwingAttack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Effect_SwingAttack"), CEffect_SwingAttack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.Mesh/////////////////////////////////////////////////////////////////

	/*For. LoadMesh*/
	if (FAILED(Load_Meshes(LEVEL_STAGE1)))
		return E_FAIL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.UI/////////////////////////////////////////////////////////////////


	/* For.Prototype_GameObject_HP_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_HP_Ui"), CHp_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CryStal_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Crystal_Ui"), CCrystal_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Box_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Box_Ui"), CBox_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Dead_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Dead_Ui"), CDead_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Talk_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Talk_UI"), CTalk_UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_Magenetic_Ui"), CMagenetic_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_JetPack_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE1, TEXT("Prototype_GameObject_JetPack_Ui"), CJetPack_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage2_Level()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	/* for.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_SkyBox"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/Dongmin/SkyBox%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_FilterNormal"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/WaterNomal0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterDiffuse */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_FilterDiffuse"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/LavaBaseTwo0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Filter"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/FXThing0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Chat */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Chat"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Talk/Talk0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Talk */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Talk"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Chat/Chat%d.png"), 3))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Effect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Effect%d.dds"), 9))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect_Png */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Texture_Effect_Png"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/EffectPng/EffectPng%d.png"), 1))))
		return E_FAIL;



	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_VIBuffer_Navigation"), CNavigation::Create(m_pDevice, m_pDeviceContext, L"../../Data/NaviMesh.dat"))))
		return E_FAIL;


	/* For.Prototype_GameObject_Collider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ColliderObject"), CCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_DeadObject"), CDeadColider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_SaveObject"), CSaveCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AnimObject */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_AnimObject"), CAnimCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Collider_AABB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Collider_OBB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_Component_Collider_SPHERE"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_MapObject"), CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Skylar"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_JetPack"), CJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Fiona"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_mTurret"), CmTerret::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Turret_Bullet"), CTurret_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChuckRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChuckRock"), CChuckRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BigDesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_BigDesertRock"), CBigDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_DeserteRock"), CDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertWater */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_DesertWater"), CDesertWater::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Scaffolding */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Scaffolding"), CScaffolding::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChangeFloor"), ChangeFloor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChangeFloorTwo"), CChangeFloorTwo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloorThree */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChangeFloorThree"), CChangeFloorThree::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloorFor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChangeFloorFor"), CChangeFloorFor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RopeRobot */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_RopeRobot"), CRopeRobot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Collector"), CColleter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Vase */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Vase"), CVase::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_NpcCage */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_NpcCage"), CNpcCage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Npc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Npc"), CNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PetNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_PetNpc"), CPetNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TalkNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_TalkNpc"), CTalkNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BossPet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_BossPet"), CBossPet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_TimeSphere"), CTimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimePedstal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_TimePedstal"), CTimePedstal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_TempleHead"), CTempleHead::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemJetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ItemJetPack"), CItemJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeCartoon */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ChangeCartoon"), CChangeCartoon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PunchRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_PunchRock"), CPunchRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Deffered_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Deffered_TimeSphere"), CDefferde_TimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeRenderObject */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_TimeRenderObject"), CTimeRenderObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Potal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Potal"), CPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LavaRotation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_LavaRotation"), CLavaRotation::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Lava */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Lava"), CLava::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemMagenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_ItemMagenetic"), CItemMagenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Magenetic"), CMagenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic_Skill */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Magenetic_Skill"), CMagenetic_Skill::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RotationBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_RotationBlock"), CRotationBlock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BallHole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_BallHole"), CBallHole::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RotationBall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_RotationBall"), CRotationBall::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravityMesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_GravityMesh"), CGravityMesh::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravitionalField */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_GravitionalField"), CGravitionalField::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_MoveBlock"), CMoveBlock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlockTwo */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_MoveBlockTwo"), CMoveBlockTwo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlockThree */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_MoveBlockThree"), CMoveBlockThree::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_BossPotal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_BossPotal"), CBossPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_BossHead */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_BossHead"), CRender_BossHead::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////For.Effect/////////////////////////////////////////////////////////////////

	/* For.Prototype_GameObject_Effect_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Effect_Rect"), CEffect_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Effect_Point"), CEffect_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Collector_Point"), CCollector_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Magenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Effect_Magenetic"), CEffect_Magenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Effect_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Effect_JetPack"), CEffect_JetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_SwingAttack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Effect_SwingAttack"), CEffect_SwingAttack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.Mesh/////////////////////////////////////////////////////////////////

	/*For. LoadMesh*/
	if (FAILED(Load_Meshes(LEVEL_STAGE2)))
		return E_FAIL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.UI/////////////////////////////////////////////////////////////////


	/* For.Prototype_GameObject_HP_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_HP_Ui"), CHp_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CryStal_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Crystal_Ui"), CCrystal_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Box_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Box_Ui"), CBox_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Dead_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Dead_Ui"), CDead_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Talk_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Talk_UI"), CTalk_UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FadeInOut_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_FadeInOut_UI"), CFadeInOut_UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_Magenetic_Ui"), CMagenetic_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_JetPack_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STAGE2, TEXT("Prototype_GameObject_JetPack_Ui"), CJetPack_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_BossStage_Level()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	/* for.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Terrain"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_SkyBox"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/SkyBox/Dongmin/SkyBox%d.dds"), 2))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_FilterNormal"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/WaterNomal0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_FilterDiffuse */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_FilterDiffuse"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/LavaBaseTwo0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Filter"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Filter/LavaFilter/FXThing0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Chat */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Chat"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Talk/Talk0.dds")))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Talk */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Talk"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Chat/Chat%d.png"), 3))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Effect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Effect%d.dds"), 9))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_BossFace */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_BossFace"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/BossFace/BossFace%d.dds"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_Effect_Png */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_Effect_Png"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/EffectPng/EffectPng%d.png"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_BossEffect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_BossEffect"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/BossEffect/BossEffect%d.dds"), 6))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_CRT*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_CRT"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/CRT/CRT%d.dds"), 1))))
		return E_FAIL;

	/* for.Prototype_Component_Texture_EndScene*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Texture_EndScene"), CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/EndScene/EndScene%d.png"), 4))))
		return E_FAIL;


	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* for.Prototype_Component_VIbuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_VIBuffer_Navigation"), CNavigation::Create(m_pDevice, m_pDeviceContext, L"../../Data/NaviMesh.dat"))))
		return E_FAIL;


	/* For.Prototype_GameObject_Collider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ColliderObject"), CCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_DeadObject"), CDeadColider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadCollider_Object */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_SaveObject"), CSaveCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AnimObject */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_AnimObject"), CAnimCollider_Object::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Collider_AABB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Collider_OBB"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_Component_Collider_SPHERE"), CColider::Create(m_pDevice, m_pDeviceContext, CColider::TYPE_SPHERE))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_MapObject"), CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Skylar"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_JetPack"), CJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Fiona"), CRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_mTurret"), CmTerret::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Turret_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Turret_Bullet"), CTurret_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChuckRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChuckRock"), CChuckRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BigDesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BigDesertRock"), CBigDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_DeserteRock"), CDesertRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_DesertWater */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_DesertWater"), CDesertWater::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Scaffolding */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Scaffolding"), CScaffolding::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChangeFloor"), ChangeFloor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChangeFloorTwo"), CChangeFloorTwo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloorThree */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChangeFloorThree"), CChangeFloorThree::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeFloorFor */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChangeFloorFor"), CChangeFloorFor::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RopeRobot */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_RopeRobot"), CRopeRobot::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Collector"), CColleter::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Vase */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Vase"), CVase::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_NpcCage */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_NpcCage"), CNpcCage::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Npc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Npc"), CNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BossNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossNpc"), CBossNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PetNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_PetNpc"), CPetNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TalkNpc */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_TalkNpc"), CTalkNpc::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BossPet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossPet"), CBossPet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_TimeSphere"), CTimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimePedstal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_TimePedstal"), CTimePedstal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_TempleHead"), CTempleHead::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemJetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ItemJetPack"), CItemJetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ChangeCartoon */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ChangeCartoon"), CChangeCartoon::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_PunchRock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_PunchRock"), CPunchRock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Deffered_TimeSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Deffered_TimeSphere"), CDefferde_TimeSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TimeRenderObject */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_TimeRenderObject"), CTimeRenderObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Potal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Potal"), CPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_LavaRotation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_LavaRotation"), CLavaRotation::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Lava */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Lava"), CLava::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemMagenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_ItemMagenetic"), CItemMagenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Magenetic"), CMagenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Magenetic_Skill */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Magenetic_Skill"), CMagenetic_Skill::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RotationBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_RotationBlock"), CRotationBlock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BallHole */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BallHole"), CBallHole::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_RotationBall */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_RotationBall"), CRotationBall::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravityMesh */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_GravityMesh"), CGravityMesh::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_GravitionalField */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_GravitionalField"), CGravitionalField::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlock */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_MoveBlock"), CMoveBlock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlockTwo */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_MoveBlockTwo"), CMoveBlockTwo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MoveBlockThree */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_MoveBlockThree"), CMoveBlockThree::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_BossPotal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossPotal"), CBossPotal::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_BossHead */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossHead"), CRender_BossHead::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_BossCRT */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_BossCRT"), CBossCRT::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_NpcSphere */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_NpcSphere"), CNpcSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Left_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Left_Bullet"), CLeft_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Right_Bullet */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Right_Bullet"), CRight_Bullet::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Funch */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Funch"), CTestMesh::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////For.Effect/////////////////////////////////////////////////////////////////

	/* For.Prototype_GameObject_Effect_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Rect"), CEffect_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Point"), CEffect_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Collector_Point */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Collector_Point"), CCollector_Point::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Effect_Boss */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Boss"), CEffect_Boss::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Roket */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Roket"), CEffect_Roket::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_RoketHit */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_RoketHit"), CEffect_RoketHit::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Magenetic */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_Magenetic"), CEffect_Magenetic::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Effect_JetPack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_JetPack"), CEffect_JetPack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_SwingAttack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_SwingAttack"), CEffect_SwingAttack::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_FireCrt */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Effect_FireCrt"), CEffect_FireCrt::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.Mesh/////////////////////////////////////////////////////////////////

	/*For. LoadMesh*/
	if (FAILED(Load_Meshes(LEVEL_BOSS_STAGE)))
		return E_FAIL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//////////////////////////////////////////////////////////////////////For.UI/////////////////////////////////////////////////////////////////


	/* For.Prototype_GameObject_HP_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_HP_Ui"), CHp_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CryStal_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Crystal_Ui"), CCrystal_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Box_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Box_Ui"), CBox_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Magenetic_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Magenetic_Ui"), CMagenetic_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_JetPack_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_JetPack_Ui"), CJetPack_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Dead_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Dead_Ui"), CDead_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Talk_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Talk_UI"), CTalk_UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FadeInOut_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_FadeInOut_UI"), CFadeInOut_UI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_End_UI */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_End_UI"), CEnd_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_EndScene */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_EndScene"), CEndScene::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Appear_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Appear_Ui"), CAppear_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Boss_Ui */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_Boss_Ui"), CBoss_Ui::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Draw_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoading);

	return S_OK;
}

HRESULT CLoader::Load_Meshes(LEVEL eLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix = XMMatrixIdentity();
	
	PivotMatrix = XMMatrixScaling(0.004f, 0.004f, 0.004f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Rocket1.fbx"),
		CModel::Create(m_pDevice, m_pDeviceContext,CModel::TYPE_NONANIM ,"../Bin/Resources/Meshes/Rotation/Rocket/", "Rocket1.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.f));


	if (FAILED(pGameInstance->Add_Prototype(eLevel, TEXT("Rocket2.fbx"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Rotation/Rocket/", "Rocket2.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eLevelID)))
	{
		MSGBOX("Failed to Created CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);
}
