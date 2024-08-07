#include "stdafx.h"
#include "..\public\Npc.h"
#include "GameInstance.h"
#include "Rock.h"
#include "..\public\PetNpc.h"

CPetNpc::CPetNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPetNpc::CPetNpc(const CPetNpc & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPetNpc::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPetNpc::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(0.04f);

	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

		m_pTransformCom->Set_Scailing(0.01f);
	}
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CVIBuffer_PointInstance::POINTINST tPointInst;
	ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
	tPointInst.m_iType = 1;
	tPointInst.m_iSize = 5;
	tPointInst.m_iTextureIndex = 2;
	tPointInst.m_iPassIndex = 0;
	XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PointEffect", &tPointInst)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_iCurMotion = NPC_IDLE;

	return S_OK;
}

_int CPetNpc::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pModelCom->Set_Animation(m_iCurMotion);
	m_pModelCom->Update(TimeDelta, &m_bIsAnimEndCheck);
	m_pModelCom->Set_PreAnimation(m_iCurMotion);

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());


	

	if (FAILED(Player_Lerp_Trace(TimeDelta)))
		return -1;

	return _int();
}

_int CPetNpc::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CPetNpc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CPetNpc::Player_Lerp_Trace(_double TimeDelta)
{
	m_Distance = XMVectorSet(0.8f, 1.2, -0.8f, 1.f);
	_vector PlayerPos = XMVector3TransformCoord(m_Distance, m_pTargetTransform->Get_WorldMatrix());
	_vector PetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector MyPos = XMVectorLerp(PetPos, PlayerPos, 0.1);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, MyPos);

	_vector MyDir = PlayerPos - PetPos;
	_float3 vLook;
	XMStoreFloat3(&vLook, MyDir);
	vLook.y = 0;
	m_pTransformCom->Set_MyLook(XMLoadFloat3(&vLook));


	return S_OK;
}

HRESULT CPetNpc::Player_Trace(_double TimeDelta)
{
	m_Distance = XMVectorSet(0.8f, 1.2, -0.8f, 1.f);
	_vector PlayerPos = XMVector3TransformCoord(m_Distance, m_pTargetTransform->Get_WorldMatrix());
	_vector PetPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector MyPos = PlayerPos - PetPos;
	_float3 vLook;
	_float Length = XMVectorGetZ(XMVector4Length(MyPos));

	if (0.3 <= Length && 5 >= Length)
	{
		//_vector PetLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		//_vector CheckVec = XMVectorEqual(XMVector3Normalize(PlayerLook), XMVector3Normalize(PetLook));
		//if (false == XMVectorGetX(CheckVec) || false == XMVectorGetZ(CheckVec))
		//{
		//	m_dLookTimeDelta += TimeDelta;
		//	_vector MyLook = XMVectorLerp(XMVector3Normalize(PetLook), XMVector3Normalize(PlayerLook), m_dLookTimeDelta);
		//	m_pTransformCom->Set_MyLook(MyLook);
		//	if (1.0 <= m_dLookTimeDelta)
		//	{
		//		m_dLookTimeDelta = 0.0;
		//	}
		//}
		XMStoreFloat3(&vLook, MyPos);
		vLook.y = 0;
		m_pTransformCom->Set_MyLook(XMLoadFloat3(&vLook));
		PetPos += XMVector3Normalize(MyPos) * TimeDelta * 5.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PetPos);
	}
	else if (5 < Length)
	{
		XMStoreFloat3(&vLook, MyPos);
		vLook.y = 0;
		m_pTransformCom->Set_MyLook(XMLoadFloat3(&vLook));
		PetPos += XMVector3Normalize(MyPos) * TimeDelta * 10.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PetPos);
	}
	else if (0.3 > Length)
	{
		//if (FAILED(Lerp_Y(TimeDelta)))
			//return -1;
		_float4 vPetPos;
		XMStoreFloat4(&vPetPos, PetPos);
		_vector PlayerLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
		XMStoreFloat3(&vLook, PlayerLook);
		vLook.y = 0;
		m_pTransformCom->Set_MyLook(XMLoadFloat3(&vLook));
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPetPos.x, vPetPos.y + m_fDisY,vPetPos.z, vPetPos.w));
	}


	//m_pTransformCom->Set_WolrdMatirx_float4x4(m_pTargetTransform->Get_WorldFloat4x4());
	//m_pTransformCom->Set_Scailing(0.01f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, m_pTargetTransform->Get_WorldMatrix()));

	if (false == m_bEffectCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CVIBuffer_PointInstance::POINTINST tPointInst;
		ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
		tPointInst.m_iType = 1;
		tPointInst.m_iSize = 3;
		tPointInst.m_iTextureIndex = 2;
		tPointInst.m_iPassIndex = 0;
		XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PetPointEffect", &tPointInst)))
			return E_FAIL;

		Safe_Release(pGameInstance);


		m_bEffectCheck = true;
	}

	return S_OK;
}

HRESULT CPetNpc::Lerp_Y(_double TimeDelta)
{
	if (false == m_YCheck)
	{
		m_dPlayer_ReadyTime += TimeDelta;

		m_fDisY = 0.7 * (1.0 - m_dPlayer_ReadyTime) + 1.7 * m_dPlayer_ReadyTime;

		if (1.0 <= m_dPlayer_ReadyTime)
		{
			m_dPlayer_ReadyTime = 0.0;
			m_YCheck = true;
		}
	}
	else
	{
		m_dPlayer_ReadyTime += TimeDelta;

		m_fDisY = 1.7 * (1.0 - m_dPlayer_ReadyTime) + 0.7 * m_dPlayer_ReadyTime;

		if (1.0 <= m_dPlayer_ReadyTime)
		{
			m_dPlayer_ReadyTime = 0.0;
			m_YCheck = false;
		}
	}

	return S_OK;
}


HRESULT CPetNpc::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, L"Npc0.fbx", TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(50.f, 90.f, 50.f);
	ColliderDesc.fRadius = 0.7f;


	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pTargetTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0));
	Safe_AddRef(m_pTargetTransform);

	m_pPlayerMatrix = m_pTargetTransform->Get_WorldMatrixPtr();
	if (nullptr == m_pPlayerMatrix)
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_Distance = XMVectorSet(0.f, 1.9f, -4.5f, 1.f);


	return S_OK;
}

HRESULT CPetNpc::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CPetNpc * CPetNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPetNpc*	pInstance = new CPetNpc(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CPetNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPetNpc::Clone(void* pArg, _uint iMyLevelIndex)
{
	CPetNpc*	pInstance = new CPetNpc(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CPetNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPetNpc::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
