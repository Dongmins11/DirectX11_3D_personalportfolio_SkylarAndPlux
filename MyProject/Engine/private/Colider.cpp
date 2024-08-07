#include "..\public\Colider.h"
#include "GameObject.h"
#include "VIBuffer_Colider.h"
#include "Transform.h"
#include "PipeLine.h"

CColider::CColider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CColider::CColider(const CColider & rhs)
	:CComponent(rhs)
	,m_eType(rhs.m_eType)
	,m_pEffect(rhs.m_pEffect)
	,m_pBatch(rhs.m_pBatch)
	,m_pInputLayOut(rhs.m_pInputLayOut)
	,m_bisCollision(false)
{
	Safe_AddRef(m_pInputLayOut);
}


HRESULT CColider::NativeConstruct_Prototype(TYPE eType)
{
	m_eType = eType;

	m_pEffect = new BasicEffect(m_pDevice);

	const void*	pShaderByteCodes = nullptr;
	size_t	ShaderCodeLength = 0;

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &ShaderCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, ShaderCodeLength, &m_pInputLayOut)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);

	return S_OK;
}

HRESULT CColider::NativeConstruct(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tColiderDesc, pArg, sizeof(COLIDERDESC));


	switch (m_eType)
	{
	case Engine::CColider::TYPE_AABB:
		m_pAABB = new BoundingBox(m_tColiderDesc.vCenter, m_tColiderDesc.vExtents);
		break;
	case Engine::CColider::TYPE_OBB:
		m_pOBB = new BoundingOrientedBox(m_tColiderDesc.vCenter, m_tColiderDesc.vExtents, _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case Engine::CColider::TYPE_SPHERE:
		m_pSphere = new BoundingSphere(m_tColiderDesc.vCenter, m_tColiderDesc.fRadius);
		break;
	}

	return S_OK;
}

void CColider::Update(_fmatrix WolrdMatrix)
{
	if (nullptr == m_pEffect ||
		nullptr == m_pBatch)
		return;

	_matrix TransformationMatrix;

	if (nullptr != m_pAABB)
	{
		m_pAABB->Center = m_tColiderDesc.vCenter;
		m_pAABB->Extents = m_tColiderDesc.vExtents;

		m_pAABB->Transform(*m_pAABB, Remove_Rotation(WolrdMatrix));
	}
	if (nullptr != m_pOBB)
	{
		m_pOBB->Center = m_tColiderDesc.vCenter;
		m_pOBB->Extents = m_tColiderDesc.vExtents;
		m_pOBB->Orientation = _float4(0.f, 0.f, 0.f, 1.f);

		m_pOBB->Transform(*m_pOBB, WolrdMatrix);

	}
	if (nullptr != m_pSphere)
	{
		m_pSphere->Center = m_tColiderDesc.vCenter;
		m_pSphere->Radius = m_tColiderDesc.fRadius;

		m_pSphere->Transform(*m_pSphere, WolrdMatrix);
	}

}

_bool CColider::Collision_AABB(CColider * pTarget)
{
	if (nullptr == m_pAABB || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	BoundingBox*	pTargetBox = pTarget->m_pAABB;
	
	if (true == m_pAABB->Intersects(*pTargetBox))
		m_bisCollision = true;
	else
		m_bisCollision = false;

	return 	m_bisCollision;
}

_bool CColider::Collision_AABBXYZ(CColider* pTarget, _bool* bX, _bool* bY, _bool* bZ, _float* fDisX, _float* fDisY, _float* fDisZ)
{
	if (nullptr == m_pAABB || nullptr == pTarget)
		return false;

	BoundingBox*	pTargetBox = pTarget->m_pAABB;

	AABBDESC		AABBDesc[2];

	AABBDesc[0].vMin = _float3(m_pAABB->Center.x - m_pAABB->Extents.x,
		m_pAABB->Center.y - m_pAABB->Extents.y,
		m_pAABB->Center.z - m_pAABB->Extents.z);

	AABBDesc[0].vMax = _float3(m_pAABB->Center.x + m_pAABB->Extents.x,
		m_pAABB->Center.y + m_pAABB->Extents.y,
		m_pAABB->Center.z + m_pAABB->Extents.z);

	AABBDesc[1].vMin = _float3(pTargetBox->Center.x - pTargetBox->Extents.x,
		pTargetBox->Center.y - pTargetBox->Extents.y,
		pTargetBox->Center.z - pTargetBox->Extents.z);

	AABBDesc[1].vMax = _float3(pTargetBox->Center.x + pTargetBox->Extents.x,
		pTargetBox->Center.y + pTargetBox->Extents.y,
		pTargetBox->Center.z + pTargetBox->Extents.z);

	m_bisCollision = true;

	if (max(AABBDesc[0].vMin.x, AABBDesc[1].vMin.x) >
		min(AABBDesc[0].vMax.x, AABBDesc[1].vMax.x))
	{
		*bX = false;
		*bY = false;
		*bZ = false;
		return m_bisCollision = false;
	}

	if (max(AABBDesc[0].vMin.y, AABBDesc[1].vMin.y) >
		min(AABBDesc[0].vMax.y, AABBDesc[1].vMax.y))
	{
		*bX = false;
		*bY = false;
		*bZ = false;
		return m_bisCollision = false;
	}

	if (max(AABBDesc[0].vMin.z, AABBDesc[1].vMin.z) >
		min(AABBDesc[0].vMax.z, AABBDesc[1].vMax.z))
	{
		*bX = false;
		*bY = false;
		*bZ = false;
		return m_bisCollision = false;
	}

	_float	fDistanceX, fDistanceY, fDistanceZ;
	_bool	LeftandRight = false;
	_bool	UpandDown = false;
	_bool	FrontandBack = false;

	if (AABBDesc[0].vMin.x > AABBDesc[1].vMin.x)
	{
		LeftandRight = false;
		fDistanceX = abs(AABBDesc[1].vMax.x - AABBDesc[0].vMin.x);
	}
	else
	{
		LeftandRight = true;
		fDistanceX = abs(AABBDesc[0].vMax.x - AABBDesc[1].vMin.x);
	}


	if (AABBDesc[0].vMin.y > AABBDesc[1].vMin.y)
	{
		UpandDown = false;
		fDistanceY = abs(AABBDesc[1].vMax.y - AABBDesc[0].vMin.y);
	}
	else
	{
		UpandDown = true;
		fDistanceY = abs(AABBDesc[0].vMax.y - AABBDesc[1].vMin.y);
	}


	if (AABBDesc[0].vMin.z > AABBDesc[1].vMin.z)
	{
		FrontandBack = false;
		fDistanceZ = abs(AABBDesc[1].vMax.z - AABBDesc[0].vMin.z);
	}
	else
	{
		FrontandBack = true;
		fDistanceZ = abs(AABBDesc[0].vMax.z - AABBDesc[1].vMin.z);
	}


	if (abs(fDistanceX) < abs(fDistanceY) && abs(fDistanceX) < abs(fDistanceZ))
	{
		*bX = true;
		if (true == LeftandRight)
			*fDisX = -fDistanceX;
		else
			*fDisX = fDistanceX;
	}
	else
		*bX = false;

	if (abs(fDistanceY) < abs(fDistanceX) && abs(fDistanceY) < abs(fDistanceZ))
	{
		*bY = true;
		if (true == UpandDown)
			*fDisY = -fDistanceY;
		else
			*fDisY = fDistanceY;
	}
	else
		*bY = false;

	if (abs(fDistanceZ) < abs(fDistanceX) && abs(fDistanceZ) < abs(fDistanceY))
	{
		*bZ = true;
		if (true == FrontandBack)
			*fDisZ = -fDistanceZ;
		else
			*fDisZ = fDistanceZ;
	}
	else
		*bZ = false;

	return m_bisCollision;
}

_bool CColider::Collision_OBB(CColider * pTarget)
{	
	if (nullptr == m_pOBB || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pOBB->Intersects(*pTarget->m_pOBB);
}

_bool CColider::Collision_Sphere(CColider * pTarget)
{
	if (nullptr == m_pSphere || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pSphere->Intersects(*pTarget->m_pSphere);
}

_bool CColider::Collision_AABBOBB(CColider * pTarget)
{
	if (nullptr == m_pAABB || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pAABB->Intersects(*pTarget->m_pOBB);
}

_bool CColider::Collision_OBBAABB(CColider * pTarget)
{
	if (nullptr == m_pOBB || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pOBB->Intersects(*pTarget->m_pAABB);
}

_bool CColider::Collision_AABBSphere(CColider * pTarget)
{
	if (nullptr == m_pAABB || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pAABB->Intersects(*pTarget->m_pSphere);
}

_bool CColider::Collision_SphereAABB(CColider * pTarget)
{
	if (nullptr == m_pSphere || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;

	return m_bisCollision = m_pSphere->Intersects(*pTarget->m_pAABB);
}

_bool CColider::Collision_SphereBoard(CColider * pTarget)
{
	if (nullptr == m_pSphere || nullptr == pTarget || true == m_bTumCollisionCheck)
		return false;
	_float fRadiusMiddleValue = 5.f;

	BoundingSphere*	pTargetSphere = pTarget->m_pSphere;

	SPHEREDESC		SphereDesc[2];

	SphereDesc[0].vCenter = _float3(m_pSphere->Center.x,
		m_pSphere->Center.y,
		m_pSphere->Center.z);

	SphereDesc[0].fRadius = m_pSphere->Radius;

	SphereDesc[1].vCenter = _float3(pTargetSphere->Center.x,
		pTargetSphere->Center.y,
		pTargetSphere->Center.z);

	SphereDesc[1].fRadius = pTargetSphere->Radius;

	
	if (SphereDesc[0].vCenter.y >= SphereDesc[1].vCenter.y)
	{
		_vector vCenterA = XMLoadFloat3(&SphereDesc[0].vCenter);
		_vector vCenterB = XMLoadFloat3(&SphereDesc[1].vCenter);
		
		_vector vRadiusA = XMVectorReplicatePtr(&SphereDesc[0].fRadius);
		_vector vRadiusB = XMVectorReplicatePtr(&SphereDesc[1].fRadius);

		_vector vDelta = vCenterB - vCenterA;
		_vector vDistanceSquared = XMVector3LengthSq(vDelta);

		_vector vRadiusSquared = XMVectorAdd(vRadiusA, vRadiusB);
		vRadiusSquared = XMVectorMultiply(vRadiusSquared, vRadiusSquared);

		_float fRadius = XMVectorGetX(vRadiusSquared);
		_float fDistance = XMVectorGetX(vDistanceSquared);

		_float fMinRadius = fRadius - fRadiusMiddleValue;
		_float fMaxRadius = fRadius + fRadiusMiddleValue;

		if (fMaxRadius >= fDistance && fMinRadius <= fDistance)
       			return true;
	}

	return false;
}

_float CColider::Get_Radius()
{
	SPHEREDESC		tSphereDesc;
	ZeroMemory(&tSphereDesc, sizeof(SPHEREDESC));

	tSphereDesc.vCenter = m_pSphere->Center;
	tSphereDesc.fRadius = m_pSphere->Radius;

	_float HalfRadius;
	HalfRadius = pow(4.f * XM_PI* tSphereDesc.fRadius,2.f); //구둘레

	HalfRadius = sqrtf((2.f * tSphereDesc.fRadius) - ((tSphereDesc.fRadius * 0.2f) * (tSphereDesc.fRadius * 0.2f))) * XM_PI; //원의 반의반둘레

	return HalfRadius;
}

#ifdef _DEBUG

HRESULT CColider::Render()
{
	//if (nullptr == m_pEffect ||
	//	nullptr == m_pBatch)
	//	return E_FAIL;

	//CPipeLine* pPipeLine = CPipeLine::Get_Instance();
	//Safe_AddRef(pPipeLine);

	//m_pDeviceContext->IASetInputLayout(m_pInputLayOut);

	//m_pEffect->SetWorld(XMMatrixIdentity());
	//m_pEffect->SetView(pPipeLine->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW));
	//m_pEffect->SetProjection(pPipeLine->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION));

	//m_pEffect->Apply(m_pDeviceContext);

	//_vector	vColor = m_bisCollision == true ? DirectX::Colors::HotPink : DirectX::Colors::Aqua;

	//m_pBatch->Begin();

	//switch (m_eType)
	//{
	//case Engine::CColider::TYPE_AABB:
	//	DX::Draw(m_pBatch, *m_pAABB, vColor);
	//	break;
	//case Engine::CColider::TYPE_OBB:
	//	DX::Draw(m_pBatch, *m_pOBB, vColor);
	//	break;
	//case Engine::CColider::TYPE_SPHERE:
	//	DX::Draw(m_pBatch, *m_pSphere, vColor);
	//	break;
	//}


	//m_pBatch->End();

	//Safe_Release(pPipeLine);

	return S_OK;
}
#endif // _DEBUG


_bool CColider::Check_Colision(_float3 * vDstMin, _float3 * vSrcMin, _float3 * vDstMax, _float3 * vSrcMax)
{
	if (vDstMax->x <= vSrcMin->x || vDstMin->x >= vSrcMax->x)
		return false;

	if (vDstMax->y <= vSrcMin->y || vDstMin->y >= vSrcMax->y)
		return false;

	if (vDstMax->z <= vSrcMin->z || vDstMin->z >= vSrcMax->z)
		return false;

	return true;
}

_matrix CColider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix		ResultMatrix = TransformMatrix;

	ResultMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	ResultMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	ResultMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return ResultMatrix;
}

CColider::OBBDESC CColider::Compute_OBBDesc()
{
	OBBDESC		OBBDesc;

	_float3		vPoints[8];
	_vector		vPointVector[8];

	for (_uint i = 0; i < 8; ++i)
		vPointVector[i] = XMLoadFloat3(&vPoints[i]);

	OBBDesc.vCenter = m_pOBB->Center;
	XMStoreFloat3(&OBBDesc.vExtenDeir[0], (vPointVector[2] + vPointVector[5]) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vExtenDeir[1], (vPointVector[2] + vPointVector[7]) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vExtenDeir[2], (vPointVector[2] + vPointVector[0]) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));

	XMStoreFloat3(&OBBDesc.vAlignDir[0], XMVector3Normalize(vPointVector[2] - vPointVector[3]));
	XMStoreFloat3(&OBBDesc.vAlignDir[1], XMVector3Normalize(vPointVector[2] - vPointVector[1]));
	XMStoreFloat3(&OBBDesc.vAlignDir[2], XMVector3Normalize(vPointVector[2] - vPointVector[6]));


	return OBBDesc;
}

CColider * CColider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
{
	CColider* pInstance = new CColider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed to Create CColider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CColider::Clone(void * pArg)
{
	CColider* pInstance = new CColider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CColider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CColider::Free()
{
	__super::Free();

	if (false == m_bIsClone)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);

	Safe_Release(m_pInputLayOut);

	
}

