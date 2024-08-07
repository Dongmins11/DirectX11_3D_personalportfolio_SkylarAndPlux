#pragma once

#include "Base.h"

BEGIN(Engine)
class CPipeLine;
class CTransform;
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() =default;
public:
	typedef struct tagPickingDesc
	{
		_float		vDist;
		_float3		vNormalVec;
	}PICKINGDESC;
public:
	HRESULT SetUp_Picking(HWND Hwnd,ID3D11DeviceContext* pDeviceContext);
public:
	void Transform_ToWorldSapce();
	_bool Picking_To_Object();
	_bool Picking_To_Terrain(CTransform* pTransform,class CVIBuffer* pVIBuffer, _float3* vOut);
	_bool Picking_To_Colider(CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* vOut, _float* fOutDist);
	_bool Picking_To_Model(CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* vOut, _float* fOutDist);
private:
	_bool IntersectTriangle(_fvector& Origin, _fvector& Dir, _fvector& v0, GXMVECTOR& v1, HXMVECTOR& v2,_float* Dist,_float* U,_float* V);

private:
	list<_float3>			m_ListDist;
	HWND					m_pHwnd;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
private:
	CPipeLine*			m_pPipeLine = nullptr;

private:
	_float3		m_vRayDir;
	_float3		m_vRayPos;
	_float		m_fDist = 0.f;
public:
	virtual void Free() override;
};

END