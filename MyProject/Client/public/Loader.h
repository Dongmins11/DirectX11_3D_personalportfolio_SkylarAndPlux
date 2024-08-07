#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
public:
	explicit CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLoader() = default;
public:
	LEVEL Get_LevelID() { return m_eLevelID; }

	CRITICAL_SECTION* Get_CriticalSection() { return &m_CriticalSection;}

	_bool Get_Finished() const {return m_isFinished;}
public:
	HRESULT NativeConstruct(LEVEL eLevelID);
public:
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_For_Stage1_Level();
	HRESULT Loading_For_Stage2_Level();
	HRESULT Loading_For_BossStage_Level();
	HRESULT Draw_LoadingText();
	HRESULT Load_Meshes(LEVEL eLevel);

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CriticalSection;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	LEVEL				m_eLevelID = LEVEL_END;
	_tchar				m_szLoading[MAX_PATH] = TEXT("");
	_bool				m_isFinished = false;

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevelID);
	virtual void Free() override;
};

END