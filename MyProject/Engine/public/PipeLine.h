#pragma once

#include "Base.h"
#include "Transform.h"
BEGIN(Engine)

class CPipeLine final :public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATETYPE {D3DTS_VIEW, D3DTS_PROJECTION, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() =default;
public:
	void	Set_HoldingMatrix(TRANSFORMSTATETYPE eState, _fmatrix HoldingMatrix) { XMStoreFloat4x4(&m_HoldMatrix[eState], HoldingMatrix); }
public:
	_matrix Get_HoldingMatrix(TRANSFORMSTATETYPE eState) { return XMLoadFloat4x4(&m_HoldMatrix[eState]); }
	_vector Get_CamPosition() { return XMLoadFloat4(&_float4(m_vCameraPosition, 1.f));}
public:
	void Update_Matrix();
private:
	_float4x4			m_HoldMatrix[D3DTS_END];
private:
	_float3				m_vCameraPosition;
public:
	virtual void Free() override;
};

END

