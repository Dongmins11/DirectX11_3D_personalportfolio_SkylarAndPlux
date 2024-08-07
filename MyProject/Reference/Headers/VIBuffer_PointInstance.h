#pragma once

#include "VIBuffer_Instance.h"
#include "Transform.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance final : public CVIBuffer_Instance
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
		_float4x4	m_Matrix;
	}POINTINST;
public:
	typedef struct tagViewZ
	{
		_uint		iIndex;
		_float		fViewZ;
	}VIEWDESC;
private:
	explicit CVIBuffer_PointInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_PointInstance(const CVIBuffer_PointInstance& rhs);
	virtual ~CVIBuffer_PointInstance() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance, _float fPSize);
	virtual HRESULT NativeConstruct(void* pArg) override;
private:
	HRESULT Type_One(_double TimeDelta,CTransform* pTransform);
	HRESULT Type_Two(_double TimeDelta, CTransform* pTransform);
	HRESULT Type_Three(_double TimeDelta,CTransform* pTransform);
	HRESULT Type_For(_double TimeDelata, CTransform* pTransform, CTransform* pTargetTransform);
	HRESULT Type_Five(_double TimeDelta, CTransform* pTransform);
	HRESULT Type_Six(_double TimeDelta, CTransform* pTransform);
	HRESULT Type_Seven(_double TimeDelta, CTransform* pTransform);
	HRESULT Type_Eight(_double TimeDelta, CTransform* pTransform);
	HRESULT Type_Nine(_double TimeDelta,  CTransform* pTransform);
public:
	void	Update(_double TimeDelta,CTransform* pTransform, CTransform* pTargetTransform = nullptr);
private:
	POINTINST*		m_pPointInst = nullptr;
	_float*			m_pSpeed = nullptr;
	_double			m_TimeAcc = 0.0;
	_double			m_BackTimeAcc = 0.0;
	_bool			m_bChangeCheck = false;
	_bool			m_bFirstCheck = true;
	_uint			m_iOriginNumInstance;
	_uint			m_iNumInstanceBackUp;
private:
	_float4			m_vDir[360] = {};
public:
	static CVIBuffer_PointInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance,_float fPSize = 1.f);
	virtual	CComponent*	Clone(void* pArg) override;
	virtual void Free() override;
};


END