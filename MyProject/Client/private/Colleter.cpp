#include "stdafx.h"
#include "..\public\Colleter.h"
#include "GameInstance.h"
#include "NpcCage.h"
#include "Crystal_Ui.h"
#include "Sound_Manager.h"

CColleter::CColleter(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CColleter::CColleter(const CColleter & rhs)
	: CGameObject(rhs)
{
}

HRESULT CColleter::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CColleter::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component_Load()))
			return E_FAIL;

		if (1 == m_pModelDesc_Client->iType)
		{
			m_dRandomBlendTime = ((rand() % 6) + 1) * 0.5;

			m_pModelDesc_Client->vScaileXYZ.y = m_pModelDesc_Client->vScaileXYZ.y - 2.f;
			m_vResultPos = m_pModelDesc_Client->vScaileXYZ;

			m_pModelDesc_Client->vPos.y += 1.f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));

			m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);

			m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);

			/*For.Random_Position*/
			m_vRandomDestPos = m_pModelDesc_Client->vPos;
			m_vRandomSourcePos = _float4((m_pModelDesc_Client->vPos.x) + (rand() % 6) - 2.5,
				m_pModelDesc_Client->vPos.y,
				(m_pModelDesc_Client->vPos.z) + (rand() % 6) - 2.5, 1.f);

			/*For.Random_Rotation*/
			m_vRandomDestRotaition = m_pModelDesc_Client->vRotaition;
			m_vRandomSourceRotation = _float3((rand() % 91) + 10, m_pModelDesc_Client->vRotaition.y, (rand() % 21) + 10);

			/*For.Random_Scaling*/
			m_fRandomDestScale = m_pModelDesc_Client->vScaile;
			m_fRandomSourceScale = m_pModelDesc_Client->vScaile * 0.3f;
		}
		else
		{

			m_pModelDesc_Client->vPos.y += 1.f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));

			m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);

			m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);

			/*For.Random_Position*/
			m_vRandomDestPos = m_pModelDesc_Client->vPos;
			m_vRandomSourcePos = _float4((m_pModelDesc_Client->vPos.x) + (rand() % 11) - 5,
				m_pModelDesc_Client->vPos.y,
				(m_pModelDesc_Client->vPos.z) + (rand() % 11) - 5, 1.f);

			/*For.Random_Rotation*/
			m_vRandomDestRotaition = m_pModelDesc_Client->vRotaition;
			m_vRandomSourceRotation = _float3((rand() % 91) + 10, m_pModelDesc_Client->vRotaition.y, (rand() % 21) + 10);

			/*For.Random_Scaling*/
			m_fRandomDestScale = m_pModelDesc_Client->vScaile;
			m_fRandomSourceScale = m_pModelDesc_Client->vScaile * 0.3f;
		}
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
		//m_pTransformCom->Set_Scailing(0.7f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 15.f, 90.f, 1.f));
	}

	return S_OK;
}

_int CColleter::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Blender_Pos_Rot(TimeDelta)))
		return E_FAIL;
	
	if (1 == m_pModelDesc_Client->iType)
	{
		if (1 == (Cage_Trace(TimeDelta)))
			return 1;
	}
	else
	{
		if (1 == (Player_Trace(TimeDelta)))
		{
			return 1;
		}
	}


	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CColleter::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this)))
		return E_FAIL;


	return 0;
}

HRESULT CColleter::Render()
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

void CColleter::Set_TargetObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return;

	m_pGameObject = pGameObject;
	Safe_AddRef(m_pGameObject);

}

HRESULT CColleter::Blender_Pos_Rot(_double TimeDelta)
{
	if (false == m_bIsChangeCheck)
	{
		m_dRandomBlenderTime += TimeDelta;

		_vector Position = XMVectorLerp(XMLoadFloat4(&m_vRandomDestPos), XMLoadFloat4(&m_vRandomSourcePos), m_dRandomBlenderTime);
		_float3 Rotation;
		XMStoreFloat3(&Rotation, XMQuaternionSlerp(XMLoadFloat3(&m_vRandomDestRotaition), XMLoadFloat3(&m_vRandomSourceRotation), m_dRandomBlenderTime));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);
		m_pTransformCom->Set_Rotation(Rotation);

		if (1.0 <= m_dRandomBlenderTime)
		{
			m_dRandomBlenderTime = 0.f;
			m_bIsChangeCheck = true;
		}
	}
	return S_OK;
}

_int CColleter::Player_Trace(_double TImeDelta)
{
	if (true == m_bIsChangeCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
		CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);
		CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Transform"), 0, 0);

		if (nullptr == pPlayerObject || nullptr == pTargetSphere || nullptr == pTargetTransform)
		{
			Safe_Release(pGameInstance);
			return -1;
		}

		Safe_Release(pGameInstance);


		m_dPlayer_BlnederTime += TImeDelta;

		_float4 PlayerPosition;
		XMStoreFloat4(&PlayerPosition, pTargetTransform->Get_State(CTransform::STATE_POSITION));
		PlayerPosition.y += 1.f;
		_vector Position = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&PlayerPosition), m_dPlayer_BlnederTime);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);

		if (1.0 <= m_dPlayer_BlnederTime)
		{
			m_dPlayer_BlnederTime = 0.0;
		}


		if (true == m_bIsScaleCheck)
		{
			m_dPlayer_ScaleTime += TImeDelta * 5.0;
			_float Scale = m_fRandomDestScale * (1.0 - m_dPlayer_ScaleTime) + m_fRandomSourceScale * m_dPlayer_ScaleTime;
			m_pTransformCom->Set_Scailing(Scale);
			
			if (1.0 <= m_dPlayer_ScaleTime)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);
				CGameObject* pUi = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Crystal_Ui");
				dynamic_cast<CCrystal_Ui*>(pUi)->Set_CristalAdd(1);
				Safe_Release(pGameInstance);
				m_dPlayer_ScaleTime = 0.0;
				
				CSound_Manager::Get_Instance()->Check_Sound(L"CollectorOne", L"CollectorTwo", L"CollectorThree", CSound_Manager::ID_OBJECT_SOUND_FOR, CSound_Manager::ID_OBJECT_SOUND_FIVE, CSound_Manager::ID_OBJECT_SOUND_SIX);
				
				return 1;
			}

		}


		if (true == m_pSphere->Collision_Sphere(pTargetSphere))
		{
			m_bIsScaleCheck = true;
		}

	}

	return 0;
}

_int CColleter::Cage_Trace(_double TimeDelta)
{
	if (true == m_bIsChangeCheck)
	{
		m_dPlayer_BlnederTime += TimeDelta * m_dRandomBlendTime;

		_float4 PlayerPosition;
		PlayerPosition= _float4(m_vResultPos.x, m_vResultPos.y, m_vResultPos.z,1.f);
		PlayerPosition.y += 1.f;
		_vector Position = XMVectorLerp(m_pTransformCom->Get_State(CTransform::STATE_POSITION), XMLoadFloat4(&PlayerPosition), m_dPlayer_BlnederTime);
		_float Scale = m_fRandomDestScale * (1.0 - m_dPlayer_BlnederTime) + m_fRandomSourceScale * m_dPlayer_BlnederTime;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Position);
		m_pTransformCom->Set_Scailing(Scale);

		if (1.0 <= m_dPlayer_BlnederTime)
		{
			m_dPlayer_BlnederTime = 0.0;
			m_bIsScaleCheck = true;

			if (nullptr != m_pGameObject)
				dynamic_cast<CNpcCage*>(m_pGameObject)->Set_Locker_Count(1);

			return 1;

		}
	}

	return _int();
}



HRESULT CColleter::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Collecter0.fbx"), TEXT("Com_VIBuffer"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CColleter::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("ChangeFloor0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CColleter::SetUp_ConstantTable()
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

CColleter * CColleter::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CColleter* pInstance = new CColleter(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CColleter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CColleter::Clone(void* pArg, _uint iMyLevelIndex)
{
	CColleter* pInstance = new CColleter(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CColleter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColleter::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);
	if(nullptr != m_pGameObject)
		Safe_Release(m_pGameObject);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
}
