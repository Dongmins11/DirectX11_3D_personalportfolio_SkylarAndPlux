#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vOriginalPoints[ePoint]);
	}

	_uint Get_Index() {
		return m_iIndex;
	}

	void Set_Neighbor(LINE eLine, CCell* pNeighbor) {
		m_pNeighbor[eLine] = pNeighbor;
	}

public:
	HRESULT NativeConstruct(const _float3* pPoints, _uint iIndex);

public:
	_bool Compare_Points(_fvector	vSourPoint, _fvector vDestPoint);
	void Update(_fmatrix TransformMatrix);
	_bool isIn(_fvector vPosition, CCell** ppNeighbor);

#ifdef _DEBUG
public:
	HRESULT Render(_uint iPassIndex);
#endif 

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

private:
	_float3			m_vOriginalPoints[POINT_END];
	_float3			m_vPoints[POINT_END];

	_float3			m_vLines[LINE_END];
	_uint			m_iIndex = 0;
	CCell*			m_pNeighbor[LINE_END];

#ifdef _DEBUG
private:
	class CVIBuffer_LineStrip*			m_pVIBuffer = nullptr;
#endif // _DEBUG

#ifdef _DEBUG
private:
	HRESULT Ready_Debug_Buffer();
#endif // _DEBUG

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END