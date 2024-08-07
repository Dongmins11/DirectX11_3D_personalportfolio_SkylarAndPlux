#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CBoss_Stage final : public CLevel
{
private:
	explicit CBoss_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CBoss_Stage() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	//HRESULT Ready_For_¹¹½Ã±â;
	HRESULT Ready_For_Camera(const _tchar* pCloneTag);
	HRESULT Ready_For_Object();
	HRESULT Ready_For_Ui();
public:
	static CBoss_Stage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END