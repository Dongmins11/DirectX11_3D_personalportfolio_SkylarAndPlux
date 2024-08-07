#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
class CColider;
END


BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() =default;
public:
	HRESULT Native_Construct();
	_int	Tick(_double TimeDelta);
	HRESULT Render();

private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
private:
	CGameInstance*			m_pGameInstance;
	CRenderer*				m_pRenderer = nullptr;
	CColider*				m_pColider = nullptr;
	class CSound_Manager*	m_pSoundManager = nullptr;
private:
	_double				m_TimeAcc = 0.0;
	_uint				m_iFPS = 0;
private:
	HRESULT Initialize_Engine();
	HRESULT Ready_For_StaticLevel_Component();
	HRESULT Ready_For_StaticLevel_GameObject();
	HRESULT Ready_For_StaticLevel_Effect();
	HRESULT Ready_For_Font();
	HRESULT Open_Level(LEVEL eLevel);
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END