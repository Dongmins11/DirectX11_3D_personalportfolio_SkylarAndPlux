#pragma once

#include "Base.h"

BEGIN(Engine)
class CLight final :public CBase
{
private:
	explicit CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLight() =default;
public:
	LIGHTDESC* Get_LightDesc() { return &m_pLightDesc; }
public:
	void	Set_LightOn(_bool bCheck) { m_bLightOn = bCheck; }
private:
	HRESULT NativecConstruct(const LIGHTDESC& tLight);
public:
	HRESULT Render(class CVIBuffer_Rect* pVIBuffer);
	
private:
	ID3D11Device*		 m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	LIGHTDESC	m_pLightDesc;
private:
	_bool				m_bLightOn = false;
public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,const LIGHTDESC& tLight);
	virtual void Free() override;

};

END