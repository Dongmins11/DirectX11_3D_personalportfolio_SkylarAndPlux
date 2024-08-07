#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Navigation final : public CVIBuffer
{
private:
	explicit CVIBuffer_Navigation(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Navigation(const CVIBuffer_Navigation& rhs);
	virtual ~CVIBuffer_Navigation() =default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	void	Set_ColorCheck(_bool bCheck) { m_bIsColorCheck = bCheck; }
	_bool	Get_ColorCheck() { return m_bIsColorCheck; }
	_uint	Get_Property() { return m_iProperty; }
public:
	HRESULT Set_PointPosition(_float3 Position,_uint iIdex);
	void	Set_Property(_uint iProperty) { m_iProperty = iProperty; }
private:
	_float3		m_vPostion[3];
	_bool		m_bIsColorCheck = false;
	_uint		m_iProperty = 0;

public:
	static	CVIBuffer_Navigation*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free()override;
};

END