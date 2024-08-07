#include "..\public\Picking.h"
#include "PipeLine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)
CPicking::CPicking()
	:m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}


HRESULT CPicking::SetUp_Picking(HWND Hwnd,ID3D11DeviceContext* pDeviceContext)
{
	if (nullptr == Hwnd)
		return E_FAIL;

	m_pHwnd = Hwnd;
	m_pDeviceContext = pDeviceContext;
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

void CPicking::Transform_ToWorldSapce()
{
	POINT pt;
	GetCursorPos(&pt);
	if (nullptr == m_pHwnd)
		return;

	_float3 vMousePos;
	_uint	a = 1;
	_uint*	iNumViewPort;
	iNumViewPort = &a;
	
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	m_pDeviceContext->RSGetViewports(iNumViewPort, &ViewPortDesc);
	ScreenToClient(m_pHwnd, &pt);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	vMousePos.x = (pt.x - (ViewPortDesc.Width * 0.5f)) / (ViewPortDesc.Width * 0.5f);
	vMousePos.y = (pt.y - (ViewPortDesc.Height * 0.5f)) / ((ViewPortDesc.Height * 0.5f) * -1.f);
	vMousePos.z = 0.f;

	_matrix	ProjecMat;
	ProjecMat = XMMatrixInverse(nullptr, pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION));
	
	XMStoreFloat3(&vMousePos, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), ProjecMat));

	m_vRayPos = _float3(0.f, 0.f, 0.f);
	XMStoreFloat3(&m_vRayDir,XMLoadFloat3(&vMousePos) - XMLoadFloat3(&m_vRayPos));

	_matrix ViewMat;
	ViewMat = XMMatrixInverse(nullptr, pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW));

	XMStoreFloat3(&m_vRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), ViewMat));
	XMStoreFloat3(&m_vRayDir, XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), ViewMat));

	Safe_Release(pGameInstance);
}

_bool CPicking::Picking_To_Object()
{
	return _bool();
}

_bool CPicking::Picking_To_Terrain(CTransform* pTransform,CVIBuffer* pVIBuffer, _float3* vOut)
{
	_matrix		WorldMat = pTransform->Get_WorldMatrixInverse();

	_vector		vLocalRayPos;
	_vector		vLocalRayDir;

	vLocalRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMat);
	vLocalRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMat);
	vLocalRayDir = XMVector3Normalize(vLocalRayDir);

	const _uint	iNumPrimitive = pVIBuffer->Get_NumPrimitive();
	const _ulong*	pIndices = (_ulong*)pVIBuffer->Get_Indices();
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();
	_uint	 iIndex = 0;
	_bool	 vCheck = false;

	for (_uint i = 0; i < iNumPrimitive; ++i)
	{
		_float	 fU, fV, fDist, fDistSave = 0.f;

		if (true == IntersectTriangle(vLocalRayPos,
			vLocalRayDir,
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			&fDist,&fU,&fV))
		{
			m_fDist = fDist;

			XMStoreFloat3(vOut,vLocalRayPos + vLocalRayDir * m_fDist);
			vCheck = true;
		}
	}
	return vCheck;
}

_bool CPicking::Picking_To_Colider(CTransform * pTransform, CVIBuffer * pVIBuffer, _float3 * vOut, _float * fOutDist)
{
	_matrix		WorldMat = pTransform->Get_WorldMatrixInverse();

	_vector		vLocalRayPos;
	_vector		vLocalRayDir;

	vLocalRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMat);
	vLocalRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMat);
	vLocalRayDir = XMVector3Normalize(vLocalRayDir);

	const _uint	iNumPrimitive = pVIBuffer->Get_NumPrimitive();
	const _ulong*	pIndices = (_ulong*)pVIBuffer->Get_Indices();
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();
	_uint	 iIndex = 0;
	_bool	 vCheck = false;

	for (_uint i = 0; i < iNumPrimitive; ++i)
	{
		_float	 fU, fV, fDist, fDistSave = 0.f;

		if (true == IntersectTriangle(vLocalRayPos,
			vLocalRayDir,
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			&fDist, &fU, &fV))
		{
			if (fDistSave > fDist || (fDist > 0.f && fDistSave == 0.f))
			{
				fDistSave = fDist;
				m_fDist = fDistSave;
				vCheck = true;
			}
			vCheck = true;
		}
	}
	if (true == vCheck)
	{
		*fOutDist = m_fDist;
		XMStoreFloat3(vOut, vLocalRayPos + vLocalRayDir * m_fDist);
	}
	return vCheck;
}

_bool CPicking::Picking_To_Model(CTransform * pTransform, CVIBuffer * pVIBuffer, _float3 * vOut, _float * fOutDist)
{
	_matrix		WorldMat = pTransform->Get_WorldMatrixInverse();

	_vector		vLocalRayPos;
	_vector		vLocalRayDir;
	list<_float> aa;
	vLocalRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMat);
	vLocalRayDir = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMat);
	vLocalRayDir = XMVector3Normalize(vLocalRayDir);

	const _uint	iNumPrimitive = pVIBuffer->Get_NumPrimitive();
	const _ulong*	pIndices = (_ulong*)pVIBuffer->Get_Indices();
	const _float3* pVerticesPos = pVIBuffer->Get_VerticesPos();
	_uint	 iIndex = 0;
	_bool	 vCheck = false;

	for (_uint i = 0; i < iNumPrimitive; ++i)
	{
		_float	 fU, fV, fDist, fDistSave = 0.f;

		if (true == IntersectTriangle(vLocalRayPos,
			vLocalRayDir,
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			XMLoadFloat3(&pVerticesPos[pIndices[iIndex++]]),
			&fDist, &fU, &fV))
		{
			if (fDistSave > fDist || (fDist > 0.f && fDistSave == 0.f))
			{
				fDistSave = fDist;
				m_fDist = fDistSave;
				aa.push_back(m_fDist);
				vCheck = true;
			}
		}
	}
	if (true == vCheck)
	{
		aa.sort();
		*fOutDist = m_fDist;
		XMStoreFloat3(vOut, vLocalRayPos + (vLocalRayDir * *aa.begin()));
	}
	return vCheck;
}

_bool CPicking::IntersectTriangle(_fvector & Origin, _fvector & Dir, _fvector & v0, GXMVECTOR & v1, HXMVECTOR & v2, _float * Dist, _float* U, _float* V)
{
	_vector edge1 = v1 - v0;
	_vector edge2 = v2 - v0;

	_vector pvec = XMVector3Cross(Dir, edge2);

	_float vecDet = XMVectorGetX(XMVector3Dot(edge1, pvec));

	_vector tvec;
	if (vecDet > 0)
	{
		tvec = Origin - v0;
	}
	else if(vecDet < 0)
	{
		tvec = v0 - Origin;
		vecDet = -vecDet;
	}

	if (vecDet < 0.0001f)
		return false;


	*U = XMVectorGetX(XMVector3Dot(tvec, pvec));
	if (*U < 0.0f || *U > vecDet)
		return false;

	_vector qVec;
	qVec = XMVector3Cross(tvec, edge1);

	*V = XMVectorGetX(XMVector3Dot(Dir, qVec));
	if (*V < 0.0f ||*U + *V > vecDet)
		return false;

	*Dist = XMVectorGetX(XMVector3Dot(edge2, qVec));
	if (*Dist < 0.f)
		return false;

	_float finalDet = 1.f / vecDet;
	*Dist *= finalDet;
	*U *= finalDet;
	*V *= finalDet;

	return true;
}

void CPicking::Free()
{
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pDeviceContext);
}
