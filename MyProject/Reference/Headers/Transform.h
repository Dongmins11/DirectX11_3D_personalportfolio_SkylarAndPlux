#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_double			SpeedPerSec;
		_double			RotationPerSec;
	}TRANSFORMDESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_vector Get_Scale();

	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_matrix Get_WorldMatrixTranspose() {
		return XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix));
	}

	_matrix Get_WorldMatrixInverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}


	_float4x4 Get_WorldFloat4x4() {return m_WorldMatrix;}

	const _float4x4* Get_WorldMatrixPtr() { return &m_WorldMatrix; }
public:

	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat3((_float3*)&m_WorldMatrix.m[eState][0], vState);
	}

	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

	void Set_WolrdMatirx_float4x4(_float4x4 SetMatrix);

	void Set_PosJump(_float fJumpValue);
	void Set_PosGravity(_float fGravityValue);
	void Set_MyLook(_fvector vLook);
	void Set_LookMat(_fmatrix Mat, _fvector vLook);
	void Set_PosUp(_float fUpValue);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Go_Straight(_double TimeDelta,class CNavigation* pNavigation = nullptr);
	HRESULT Go_Backward(_double TimeDelta);
	HRESULT Go_Down(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT	Go_Direction(_double TimeDelta, _fvector vDirection);
	HRESULT	Go_BackDirection(_double TimeDelta, _fvector vDirection);
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation_Turn(_fvector vAxis, _float fRadian);
	HRESULT YawRollPitch_Set(_float3 RotationSet);
	HRESULT Rotation(_fvector vAxis, _float fRadian);
	HRESULT Rotation_Axis(_fvector vAxis, _float fRadian);
	HRESULT Rotation_Add(_fvector vAxis, _float fRadian);
	HRESULT Set_Scailing(_float vScaile);
	HRESULT Set_ScailingXYZ(_float3 vScaile);
	HRESULT Set_ScailingXYZTwo(_float3 vScaile);
	HRESULT Set_ScaleY(_float fY);
	HRESULT Set_ScaleZ(_float fZ);
	HRESULT Set_Rotation(_float3 vRotation);

private:
	TRANSFORMDESC		m_TransformDesc;
	_float4x4			m_WorldMatrix;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END