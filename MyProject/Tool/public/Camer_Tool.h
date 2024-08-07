#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Tool)

class CCamer_Tool final :public CCamera
{
private:
	explicit CCamer_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamer_Tool(const CCamer_Tool& rhs);
	virtual ~CCamer_Tool() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CCamer_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END