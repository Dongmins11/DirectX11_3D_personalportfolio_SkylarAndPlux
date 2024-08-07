#pragma once

#include "Base.h"

BEGIN(Engine)
class CLight_Manager final :public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() =default;
public:
	HRESULT	NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Set_LightDesc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& tLightDesc);	
	LIGHTDESC*	Get_LighDesc(_uint iIndex);
	void	Set_LightRenderCheck(_uint iIndex,_bool bCheck);
	HRESULT	Render();
public:
	HRESULT	Clear_Light();
private:
	class CVIBuffer_Rect*	m_pVIBuffer = nullptr;
	_float4x4				m_TransformMatrix;
	_float4x4				m_ProjMatrix;
private:
	list<class CLight*>		m_listLight;

public:
	virtual void Free() override;
};

END