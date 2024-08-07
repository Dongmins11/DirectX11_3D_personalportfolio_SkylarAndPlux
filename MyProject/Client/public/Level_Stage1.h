#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
private:
	explicit CLevel_Stage1(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Stage1() =default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	//HRESULT Ready_For_¹¹½Ã±â;
	HRESULT Ready_For_Camera(const _tchar* pCloneTag);
	HRESULT Ready_For_Terrain(const _tchar* pCloneTag);
	HRESULT Ready_For_SkyBox(const _tchar* pCloneTag);
	HRESULT Read_For_Ui();
	HRESULT Ready_For_Meshes();
	HRESULT Read_For_LightDesc();
	HRESULT	Read_For_GameObject_To_Layers();
	HRESULT Ready_For_Effect();
public:
	static CLevel_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END