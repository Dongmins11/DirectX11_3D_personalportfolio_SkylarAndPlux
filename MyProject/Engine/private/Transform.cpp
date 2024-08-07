#include "..\Public\Transform.h"
#include "Navigation.h"


CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_vector CTransform::Get_Scale()
{
	return XMVectorSet(XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK)))
		, 0.f);
}

void CTransform::Set_WolrdMatirx_float4x4(_float4x4 SetMatrix)
{
	_vector Right = XMLoadFloat4((_float4*)&SetMatrix.m[0]);
	_vector Up = XMLoadFloat4((_float4*)&SetMatrix.m[1]);
	_vector Look = XMLoadFloat4((_float4*)&SetMatrix.m[2]);
	_vector Position = XMLoadFloat4((_float4*)&SetMatrix.m[3]);
	Set_State(CTransform::STATE_RIGHT, Right);
	Set_State(CTransform::STATE_UP, Up);
	Set_State(CTransform::STATE_LOOK, Look);
	Set_State(CTransform::STATE_POSITION, Position);
}

void CTransform::Set_PosJump(_float fJumpValue)
{
	_float4 Position;
	XMStoreFloat4(&Position, Get_State(STATE_POSITION));
	Position.y = fJumpValue;

	Set_State(STATE_POSITION, XMLoadFloat4(&Position));
}

void CTransform::Set_PosGravity(_float fGravityValue)
{
	_float4 Position;

	XMStoreFloat4(&Position, Get_State(STATE_POSITION));

	Position.y += fGravityValue;

	Set_State(STATE_POSITION, XMLoadFloat4(&Position));
}

void CTransform::Set_MyLook(_fvector vLook)
{
	_vector	Scale = Get_Scale();
	_vector vRight;
	_vector vUp;

	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * XMVectorGetZ(Scale));

	vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * XMVectorGetX(Scale));

	vUp = XMVector3Cross(vLook,vRight);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * XMVectorGetY(Scale));
}

void CTransform::Set_LookMat(_fmatrix Mat,_fvector vLook)
{
	_matrix MyMat = Mat;

	Set_State(STATE_LOOK, MyMat.r[2] * XMVector3Normalize(vLook));

	Set_State(STATE_RIGHT, MyMat.r[0]);
	Set_State(STATE_UP, MyMat.r[1]);
}

void CTransform::Set_PosUp(_float fUpValue)
{
	_float4 Position;

	XMStoreFloat4(&Position, Get_State(STATE_POSITION));

	Position.y += fUpValue;

	Set_State(STATE_POSITION, XMLoadFloat4(&Position));
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_double TimeDelta, CNavigation * pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	if (nullptr != pNavigation)
	{
		if (false == pNavigation->Move_OnNavigation(vPosition))
			return S_OK;
	}

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Down(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);
	
	return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Direction(_double TimeDelta, _fvector vDirection)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vDirection) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackDirection(_double TimeDelta, _fvector vDirection)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	vPosition -= XMVector3Normalize(vDirection) * _float(m_TransformDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(m_TransformDesc.RotationPerSec * TimeDelta));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation_Turn(_fvector vAxis, _float fRadian)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fRadian));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::YawRollPitch_Set(_float3 RotationSet)
{
	_matrix newRotaion = XMMatrixRotationRollPitchYaw(XMConvertToRadians(RotationSet.x),
													XMConvertToRadians(RotationSet.y),
													XMConvertToRadians(RotationSet.z));
	_vector vRight = newRotaion.r[0];
	_vector vUp = newRotaion.r[1];
	_vector vLook = newRotaion.r[2];

	_float X = XMVectorGetX(Get_Scale());
	_float Y = XMVectorGetY(Get_Scale());
	_float Z = XMVectorGetZ(Get_Scale());

	Set_State(STATE_RIGHT, vRight*X);
	Set_State(STATE_UP, vUp*Y);
	Set_State(STATE_LOOK, vLook*Z);

	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(Get_Scale());
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetY(Get_Scale());
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetZ(Get_Scale());

	_matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fRadian));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation_Axis(_fvector vAxis, _float fRadian)
{

	//XMMatrixRotationRollPitchYaw()
	return S_OK;
}

HRESULT CTransform::Rotation_Add(_fvector vAxis, _float fRadian)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Set_Scailing(_float vScaile)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_RIGHT, XMVector4Normalize(vRight) * vScaile);
	Set_State(CTransform::STATE_UP, XMVector4Normalize(vUp) * vScaile);
	Set_State(CTransform::STATE_LOOK, XMVector4Normalize(vLook) * vScaile);

	return S_OK;
}

HRESULT CTransform::Set_ScailingXYZ(_float3 vScaile)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Length(vRight) * vScaile.x;
	vUp = XMVector3Length(vUp) * vScaile.y;
	vLook = XMVector3Length(vLook) * vScaile.z;

	Set_State(CTransform::STATE_RIGHT,vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Set_ScailingXYZTwo(_float3 vScaile)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector4Normalize(vRight) * vScaile.x;
	vUp = XMVector4Normalize(vUp) * vScaile.y;
	vLook = XMVector4Normalize(vLook) * vScaile.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Set_ScaleY(_float fY)
{
	_vector vUp = Get_State(CTransform::STATE_UP);

	Set_State(CTransform::STATE_UP, XMVector4Normalize(vUp) * fY);

	return S_OK;
}

HRESULT CTransform::Set_ScaleZ(_float fZ)
{
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	Set_State(CTransform::STATE_LOOK, XMVector4Normalize(vLook) * fZ);

	return S_OK;
}

HRESULT CTransform::Set_Rotation(_float3 vRotation)
{
	_vector vRight = XMVectorSet(1.f,0.f,0.f,0.f);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	_matrix matRotationX, matRotationY, matRotationZ, matWorld;

	matRotationX = XMMatrixRotationX(XMConvertToRadians(vRotation.x));
	matRotationY = XMMatrixRotationY(XMConvertToRadians(vRotation.y));
	matRotationZ = XMMatrixRotationZ(XMConvertToRadians(vRotation.z));

	matWorld = matRotationX *  matRotationY *  matRotationZ;

	vRight = XMVector3TransformNormal(vRight, matWorld);
	vUp = XMVector3TransformNormal(vUp, matWorld);
	vLook = XMVector3TransformNormal(vLook, matWorld);


	Set_State(STATE_RIGHT, vRight * XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	Set_State(STATE_UP, vUp * XMVector3Length(Get_State(CTransform::STATE_UP)));
	Set_State(STATE_LOOK, vLook * XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTransform");

		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();


}
