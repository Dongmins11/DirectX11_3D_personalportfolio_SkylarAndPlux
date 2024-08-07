#include "..\public\Camera.h"
#include "Transform.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pPipeLine(CPipeLine::Get_Instance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::NativeConstruct_Prototype()
{


	return S_OK;
}

HRESULT CCamera::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pTransform)
		return E_FAIL;

	memcpy(&m_CameraDesc, pArg, sizeof(CAMEREADESC));

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_float4(m_CameraDesc.vEye, 1.f)));

	_vector		vLook = XMLoadFloat4(&_float4(m_CameraDesc.vAt, 1.f)) - XMLoadFloat4(&_float4(m_CameraDesc.vEye, 1.f));
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight;
	vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp;
	vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	return S_OK;
}

_int CCamera::Tick(_double TimeDelta)
{
	_matrix		ViewMatrix = m_pTransform->Get_WorldMatrixInverse();
	m_pPipeLine->Set_HoldingMatrix(CPipeLine::D3DTS_VIEW, ViewMatrix);

	_matrix ProjMatrix = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_pPipeLine->Set_HoldingMatrix(CPipeLine::D3DTS_PROJECTION, ProjMatrix);

	return _int();
}

_int CCamera::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CCamera::Render()
{
	return S_OK;
}



void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTransform);

}
