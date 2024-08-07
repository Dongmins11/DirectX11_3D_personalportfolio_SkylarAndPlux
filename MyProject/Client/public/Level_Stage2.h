#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage2 final : public CLevel
{
private:
	explicit CLevel_Stage2(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Stage2() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	//HRESULT Ready_For_¹¹½Ã±â;
	HRESULT Ready_For_Camera(const _tchar* pCloneTag);
	HRESULT Ready_For_UI();
public:
	static CLevel_Stage2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END