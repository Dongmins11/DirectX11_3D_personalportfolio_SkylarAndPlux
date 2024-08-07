#include "stdafx.h"
#include "..\public\BossPet.h"
#include "GameInstance.h"
#include "Rock.h"

CBossPet::CBossPet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBossPet::CBossPet(const CBossPet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBossPet::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossPet::Native_Construct(void* pArg, _uint iMyLevelIndex)
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
		m_pTransformCom->Set_Scailing(0.02f);

	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

		m_pTransformCom->Set_Scailing(0.04f);
	}



	m_iCurMotion = NPC_CRY;

	return S_OK;
}

_int CBossPet::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pModelCom->Set_Animation(m_iCurMotion);
	m_pModelCom->Update(TimeDelta, &m_bIsAnimEndCheck);
	m_pModelCom->Set_PreAnimation(m_iCurMotion);

	if (FAILED(Boss_Trace(TimeDelta)))
		return -1;

	return _int();
}

_int CBossPet::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CBossPet::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossPet::Boss_Trace(_double TimeDelta)
{
	m_Distance = XMVectorSet(0.35f, 0.5, 1.2f, 1.f);
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

HRESULT CBossPet::Lerp_Y(_double TimeDelta)
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


HRESULT CBossPet::SetUp_Component()
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

	/* Com_Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;



	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pTargetTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Components(m_iMyLevel, L"BossHead0.fbx", L"Com_Transform", 0));
	Safe_AddRef(m_pTargetTransform);

	m_pPlayerMatrix = m_pTargetTransform->Get_WorldMatrixPtr();
	if (nullptr == m_pPlayerMatrix)
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_Distance = XMVectorSet(0.f, 1.9f, 4.5f, 1.f);


	return S_OK;
}

HRESULT CBossPet::SetUp_ConstantTable()
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

CBossPet * CBossPet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBossPet*	pInstance = new CBossPet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CBossPet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBossPet::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBossPet*	pInstance = new CBossPet(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CBossPet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossPet::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
