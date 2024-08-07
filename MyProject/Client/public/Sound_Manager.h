#pragma once
#include "Base.h"
#include "Client_Defines.h"



BEGIN(Client)
class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

public:
	enum ID {
		ID_STAGEONE_BGM, ID_STAGETWO_BGM, ID_BOSS_STAGE_BGM,ID_BOSS_STAGE_VOICE,ID_STAGE_END,
		ID_PLAYER_ATTACK_PUNCH, ID_PLAYER_ATTACK_SWING, ID_PLAYER_ATTCK_DOWN,
		ID_PLAYER_WALK,ID_PLAYER_JUMP, ID_PLAYER_DOUBLEJUMP,ID_PLAYER_JETPACK,ID_PLAYER_SUPERJUMP,
		ID_PLAYER_DEAD,ID_PLAYER_HIT,ID_PLAYER_MAGENETIC,ID_PLAYER_SWINGNODE,
		ID_BOSS_ATTCK,ID_BOSS_RELROAD,
		ID_MONSTER_DEAD, ID_MONSTER_HIT, ID_MONSTER_SHOT,
		ID_NPC_TALK, ID_NPC_CRY, ID_NPC_LOYA,ID_NPC_HELP, ID_NPC_EXPLOSIVE,
		ID_OBJECT_SOUND, ID_OBJECT_HIT,
		ID_OBJECT_SOUND_ONE, ID_OBJECT_SOUND_TWO, ID_OBJECT_SOUND_THREE,
		ID_OBJECT_SOUND_FOR, ID_OBJECT_SOUND_FIVE, ID_OBJECT_SOUND_SIX,
		ID_ROKET_DEAD, ID_ROKET_RIGHTDEAD,ID_ROKET_LEFTDEAD,
		ID_END
	};

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize();

public:
	void Check_Sound(const TCHAR* pSoundKey, const TCHAR* pTwoSoundKey, const TCHAR* pThreeSoundKey, ID eOneID, ID eTwoID, ID eThreeID);
	void PlaySound(const TCHAR* pSoundKey, ID eID);
	void PlayBGM(const TCHAR* pSoundKey,_float	fSoundValue, ID eID);
	void StopSound(ID eID);
	void StopAll();
	void Sound_Value(ID eIDIndex,const TCHAR* pSoundKey, _float	fSoundValue);

private:
	_float m_fBGM;
	_float m_fSoundValue = 0.f;

private:
	HRESULT LoadSoundFile();
private:
	// 사운드 리소스 정보를 갖는 객체 
	map<wstring, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[ID_END];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free()override;
};

END