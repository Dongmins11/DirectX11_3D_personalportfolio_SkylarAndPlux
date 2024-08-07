#include "stdafx.h"
#include "..\public\BossNpc.h"
#include "GameInstance.h"
#include "Rock.h"
#include "Sound_Manager.h"

CBossNpc::CBossNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CBossNpc::CBossNpc(const CBossNpc & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBossNpc::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossNpc::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_Rotation(_float3(0.f,90.f,0.f));

		_float4 Pos = m_pModelDesc_Client->vPos;
		Pos.y -= 5.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&Pos));
		m_pTransformCom->Set_Scailing(0.1f);
	}

	m_iCurMotion = NPC_CRY;

	return S_OK;
}

_int CBossNpc::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pModelCom->Set_Animation(m_iCurMotion);
	m_pModelCom->Update(TimeDelta, &m_bIsAnimEndCheck);
	m_pModelCom->Set_PreAnimation(m_iCurMotion);

	if (FAILED(End_Scene(TimeDelta)))
		return -1;

	return _int();
}

_int CBossNpc::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (false == m_bRenderCheck)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
	}
	return 0;
}

HRESULT CBossNpc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossNpc::Animation_Change()
{
	if (true == m_bAnimCageCheck && m_iCurMotion == NPC_CRY)
	{
		m_bIsAnimEndCheck = false;
		m_iCurMotion = NPC_SKY;
	}
	else if (m_iCurMotion == NPC_SKY && true == m_bIsAnimEndCheck)
	{
		//사라진다 
		m_bAnimCageCheck = false;
		m_bRenderCheck = true;
	}

	return S_OK;
}

HRESULT CBossNpc::End_Scene(_double TimeDelta)
{
	if (true == m_bEndStart)
	{
		if (false == m_bNpcGoal)
		{
			m_iCurMotion = NPC_IDLE;

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
			if (nullptr == pPlayerTransform)
			{
				MSGBOX("BossNpc Failed Finder End Sence");
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			Safe_Release(pGameInstance);

			_vector MyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector PlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
			_matrix MyMat = pPlayerTransform->Get_WorldMatrix();
			_vector Distance = XMVectorSet(0.f, 1.f, 5.f, 1.f);
			_vector ResultPos = XMVectorLerp(MyPos, XMVector3TransformCoord(Distance, MyMat), 0.005);
			_vector Dir = PlayerPos - MyPos;
			_float fLength = XMVectorGetX(XMVector3Length(Dir));

			m_pTransformCom->Set_MyLook(Dir);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, ResultPos);

			if (5.5 >= fLength)
			{
				CSound_Manager::Get_Instance()->PlayBGM(L"StageEnd", 1.f, CSound_Manager::ID_STAGE_END);

				if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_BOSS_STAGE, TEXT("Prototype_GameObject_EndScene"), LEVEL_BOSS_STAGE, L"Clone_End_Scene")))
					return E_FAIL;


				m_iCurMotion = NPC_TALK;
				m_bNpcGoal = true;
			}
		}
	}
	return S_OK;
}


HRESULT CBossNpc::SetUp_Component()
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


	return S_OK;
}

HRESULT CBossNpc::SetUp_ConstantTable()
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

CBossNpc * CBossNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CBossNpc*	pInstance = new CBossNpc(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CBossNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBossNpc::Clone(void* pArg, _uint iMyLevelIndex)
{
	CBossNpc*	pInstance = new CBossNpc(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CBossNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossNpc::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
