#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 실제레벨들의 사전레벨. */
/* 실제레벨에서 사용할 데이터들의 준비. */
/* 실제 레벨이 무엇인지에대해서 보관. */
/* 로딩중에 보여줘야할 장면을 담당한다. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	explicit CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int Tick(_double fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Read_For_Loading_Texture(const _tchar* pCloneTag);
private:
	LEVEL			m_eNextLevel = LEVEL_END;
	class CLoader*	m_pLoader = nullptr;

	_double			m_TimeAcc = 0.0;
public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END