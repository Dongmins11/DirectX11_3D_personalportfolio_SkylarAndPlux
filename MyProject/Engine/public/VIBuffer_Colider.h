#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CTransform;
class ENGINE_DLL CVIBuffer_Colider final : public CVIBuffer
{
private:
	explicit CVIBuffer_Colider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_Colider(const CVIBuffer_Colider& rhs);
	virtual ~CVIBuffer_Colider() =default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	CTransform*		Get_Transform() const { return m_pTransform; }
public:
	HRESULT			BigAndSmallVertex(CTransform* pTransform,_float3* MinPointOut, _float3* MaxPointOut);
private:
	CTransform*			m_pTransform = nullptr;

public:
	static CVIBuffer_Colider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END