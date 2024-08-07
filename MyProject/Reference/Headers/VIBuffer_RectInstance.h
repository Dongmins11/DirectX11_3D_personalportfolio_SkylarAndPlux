#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectInstance final : public CVIBuffer_Instance
{
public:
	typedef struct tagRectInst
	{
		_uint		m_iType;
		_uint		m_iSize;
		_uint		m_iPassIndex;
		_uint		m_iTextureIndex;
		_float4		m_vColor;
		_float4		m_vPosition;
	}RECTINST;
private:
	explicit CVIBuffer_RectInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectInstance(const CVIBuffer_RectInstance& rhs);
	virtual ~CVIBuffer_RectInstance() =default;
public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	void	Update(_double TimeDelta);
private:
	RECTINST*		m_pRectInst = nullptr;
	_float*			m_pSpeed = nullptr;
	_double			m_TimeAcc = 0.0;
	_double			m_OneTimeAcc = 0.0;
	_double			m_TwoTimeAcc = 0.0;
private:
	_bool			m_FirstCheck = false;
	_bool			m_SecondCheck = false;
	_bool			m_ThirdCheck = false;

public:
	static CVIBuffer_RectInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance);
	virtual	CComponent*	Clone(void* pArg) override;
	virtual void Free() override;
};


END