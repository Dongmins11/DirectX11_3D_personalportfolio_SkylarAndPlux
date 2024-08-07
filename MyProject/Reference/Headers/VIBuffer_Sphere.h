#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
private:
	explicit CVIBuffer_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs);
	virtual ~CVIBuffer_Sphere() =default;
public:
	virtual HRESULT NativeConstruct_Prototype(_float radius, _uint StackCount, _uint SliceCount);
	virtual HRESULT NativeConstruct(void* pArg);
private:
	_float	m_fRadius = 0.f;

	_uint	m_iStackCount = 0;
	_uint	m_iSliceCount = 0;

public:
	static CVIBuffer_Sphere*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,_float radius,_uint StackCount,_uint SliceCount);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END