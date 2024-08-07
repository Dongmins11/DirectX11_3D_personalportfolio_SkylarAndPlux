#include "stdafx.h"
#include "../public/Sound_Manager.h"
IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return S_OK;
}


void CSound_Manager::Check_Sound(const TCHAR * pSoundKey, const TCHAR * pTwoSoundKey, const TCHAR * pThreeSoundKey, ID eOneID, ID eTwoID, ID eThreeID)
{
	auto iter = m_mapSound.find(pSoundKey);
	auto iterTwo = m_mapSound.find(pTwoSoundKey);
	auto iterThree = m_mapSound.find(pThreeSoundKey);

	if (iter == m_mapSound.end() || 
		iterTwo == m_mapSound.end() ||
		iterThree == m_mapSound.end())
		return;
	
	FMOD_BOOL bPlay = FALSE;
	FMOD_BOOL bTwoPlay = FALSE;
	FMOD_BOOL bThreePlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eOneID], &bPlay))
	{
		if (1 != bPlay)
		{
			FMOD_Channel_Stop(m_pChannelArr[eOneID]);
			FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eOneID]);
		}
	}

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eTwoID], &bTwoPlay))
	{
		if (1 != bTwoPlay)
		{
			FMOD_Channel_Stop(m_pChannelArr[eTwoID]);
			FMOD_System_PlaySound(m_pSystem, iterTwo->second, nullptr, FALSE, &m_pChannelArr[eTwoID]);
		}
	}

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eThreeID], &bThreePlay))
	{
		if (1 != bThreePlay)
		{
			FMOD_Channel_Stop(m_pChannelArr[eThreeID]);
			FMOD_System_PlaySound(m_pSystem, iterThree->second, nullptr, FALSE, &m_pChannelArr[eThreeID]);
		}
	}

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::PlaySound(const TCHAR * pSoundKey, ID eID)
{
	auto iter = m_mapSound.find(pSoundKey);

	if (iter == m_mapSound.end())
		return;

	FMOD_Channel_Stop(m_pChannelArr[eID]);
	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);

}

void CSound_Manager::PlayBGM(const TCHAR * pSoundKey,_float	fSoundValue, ID eID)
{
	auto iter = m_mapSound.find(pSoundKey);

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);

	m_fBGM = fSoundValue;
	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_fBGM);
}

void CSound_Manager::StopSound(ID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::StopAll()
{
	for (int i = 0; i < ID_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::Sound_Value(ID eIDIndex,const TCHAR* pSoundKey, _float	fSoundValue)
{
	auto iter = m_mapSound.find(pSoundKey);

	if (iter == m_mapSound.end())
		return;

	m_fSoundValue = fSoundValue;
	FMOD_Channel_SetVolume(m_pChannelArr[eIDIndex], m_fSoundValue);

}

HRESULT CSound_Manager::LoadSoundFile()
{
	FMOD_SOUND* pSound = nullptr;

	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/HorrorSound.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"GameStart", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Stage //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageOne/Stage1.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage_One", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageTwo/StageTwo.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage_Two", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageBoss/BossStage1.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage_BossOne", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageBoss/BossStage2.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Stage_BossTwo", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageBoss/BossVoice.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossVoice", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/StageEnd/StageEnd.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"StageEnd", pSound); 
	else return E_FAIL;


	///////////////////////////////////For.Player//////////////////////////////////

	///////////////////////////////////For.Attack//////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Attack/AttackFast.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"AttackFast", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Attack/AttackNormal.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"AttackNormal", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Attack/SlamAttack.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"SlamAttack", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Attack/SwingAttack.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"SwingAttack", pSound);
	else return E_FAIL;


	///////////////////////////////////For.Jump//////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Jump/Jump.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Jump", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Jump/DoubleJump.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"DoubleJump", pSound); 
	else return E_FAIL;


	///////////////////////////////////For.JetPack//////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/JetPack/JetPackLoop.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"JetPackLoop", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/JetPack/ChargeJumpReady.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ChargeJumpReady", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/JetPack/ChargeJumpStart.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ChargeJumpStart", pSound);
	else return E_FAIL;


	///////////////////////////////////For.Player_Dead //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Dead/Player_Dead.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Player_Dead", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Player_Hit //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Hit/PunchHit.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"PunchHit", pSound); 
	else return E_FAIL;


	///////////////////////////////////For.Magenetic //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Magenetic/Magenetic.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Magenetic", pSound);
	else return E_FAIL;

	///////////////////////////////////For.SwingNode //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/SwingNode/SwingNode.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"SwingNode", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Walk //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Walk/StageWalk.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"StageWalk", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Walk/BossWalk.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossWalk", pSound); 
	else return E_FAIL;

	///////////////////////////////////For.Landing //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Player/Landing/Landing.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Landing", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Boss //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/BossRelroad.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossRelroad", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/BossSwing.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossSwing", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/GunShot.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"GunShot", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/TakeDown.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"TakeDown", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/BossEletric.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossEletric", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Boss/BossEletricTwo.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"BossEletricTwo", pSound);
	else return E_FAIL;
	

	///////////////////////////////////For.Npc //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/Npc_Cry.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Npc_Cry", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/Npc_Help.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Npc_Help", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/Npc_HelpBGM.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Npc_HelpBGM", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/Npc_Explosive.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Npc_Explosive", pSound);
	else return E_FAIL;
	
	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/NpcPet.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"NpcPet", pSound); 
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/NpcTalkOne.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"NpcTalkOne", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/NpcTalkTwo.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"NpcTalkTwo", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Npc/NpcTalkThree.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"NpcTalkThree", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Roket //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Roket/RoketHit.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"RoketHit", pSound);
	else return E_FAIL;




	///////////////////////////////////For.Object //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/TimePad.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"TimePad", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/StageTwoObject.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"StageTwoObject", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/ObjectHit.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"ObjectHit", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/VaseOne.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"VaseOne", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/VaseTwo.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"VaseTwo", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/VaseThree.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"VaseThree", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/CollectorOne.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"CollectorOne", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/CollectorTwo.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"CollectorTwo", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Object/CollectorThree.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"CollectorThree", pSound);
	else return E_FAIL;

	///////////////////////////////////For.Monster //////////////////////////////////

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Monster/Monster_Dead.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Dead", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Monster/Monster_Hit.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Hit", pSound);
	else return E_FAIL;

	eRes = FMOD_System_CreateSound(m_pSystem, "../../Client/bin/Resources/Sound/Monster/Monster_Shot.wav", FMOD_DEFAULT, 0, &pSound);
	if (eRes == FMOD_OK) m_mapSound.emplace(L"Monster_Shot", pSound);
	else return E_FAIL;

	FMOD_System_Update(m_pSystem);
	return S_OK;
}

void CSound_Manager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}