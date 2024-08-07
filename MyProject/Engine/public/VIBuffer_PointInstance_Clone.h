#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance_Clone final : public CVIBuffer_Instance
{
public:
	typedef struct tagPointInst
	{
		_uint		m_iType;
		_uint		m_iSize;
		_uint		m_iPassIndex;
		_uint		m_iTextureIndex;
		_float4		m_vColor;
		_float4		m_vPosition;
	}POINTINST;
private:
	explicit CVIBuffer_PointInstance_Clone(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_PointInstance_Clone(const CVIBuffer_PointInstance_Clone& rhs);
	virtual ~CVIBuffer_PointInstance_Clone() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance, _float fPSize);
	virtual HRESULT NativeConstruct(void* pArg) override;
private:
	HRESULT Type_One(_double TimeDelta);
	HRESULT Type_Two(_double TimeDelta);
	HRESULT Type_Three(_double TimeDelta);
	HRESULT Type_For(_double TimeDelata);
	HRESULT Type_Five(_double TimeDelta);
	HRESULT Type_Six(_double TimeDelta);
	HRESULT Type_Seven(_double TimeDelta);
	HRESULT Type_Eight(_double TimeDelta);
	HRESULT Type_Nine(_double TimeDelta);
public:
	void	Update(_double TimeDelta);
private:
	POINTINST*		m_pPointInst = nullptr;
	_float*			m_pSpeed = nullptr;
	_double			m_TimeAcc = 0.0;
	_bool			m_bChangeCheck = false;
	_float			m_fPSize = 0.f;
public:
	static CVIBuffer_PointInstance_Clone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance,_float fPSize = 1.f);
	virtual	CComponent*	Clone(void* pArg) override;
	virtual void Free() override;
};


END