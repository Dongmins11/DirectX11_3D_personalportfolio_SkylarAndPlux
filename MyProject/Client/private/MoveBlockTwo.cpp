#include "stdafx.h"
#include "..\public\MoveBlockTwo.h"
#include "GameInstance.h"
#include "Rock.h"


CMoveBlockTwo::CMoveBlockTwo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMoveBlockTwo::CMoveBlockTwo(const CMoveBlockTwo & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMoveBlockTwo::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMoveBlockTwo::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component_Load()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);

		m_pRopeTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet((m_pModelDesc_Client->vPos.x + 8.5f), m_pModelDesc_Client->vPos.y, m_pModelDesc_Client->vPos.z, m_pModelDesc_Client->vPos.w));
		m_pRopeTransformCom->Set_ScailingXYZ(_float3(0.5f, 1.f, 0.5f));
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 23.f, 146.f, 1.f));
		m_pRopeTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 23.f, 146.f, 1.f));
		m_pRopeTransformCom->Set_Scailing(1.f);

	}

	if (FAILED(Set_Pos()))
		return E_FAIL;

	ZeroMemory(&m_tPlayerCollisionDesc, sizeof(CRock::PLAYERCOLLISIONDESC));

	return S_OK;
}

_int CMoveBlockTwo::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;



	m_pSphere->Update(m_pRopeTransformCom->Get_WorldMatrix());
	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());

	return 0;
}

_int CMoveBlockTwo::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Player_Collision_Check(TimeDelta)))
		return -1;

	if (FAILED(Move_State(TimeDelta)))
		return -1;

	if (FAILED(Rope_Look(TimeDelta)))
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return -1;

	return 0;
}

HRESULT CMoveBlockTwo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	if (true == m_bIsRenderCheck)
	{
		if (FAILED(m_pRopeModelCom->Render(0)))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pSphere->Render();
	m_pAABB->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CMoveBlockTwo::Player_Collision_Check(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_AABB"), 0, 0);
	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
		return -1;

	if (true == pTargetSphere->Collision_AABBXYZ(m_pAABB,
		&m_tPlayerCollisionDesc.bCollisionX, &m_tPlayerCollisionDesc.bCollisionY, &m_tPlayerCollisionDesc.bCollisionZ,
		&m_tPlayerCollisionDesc.fDisX, &m_tPlayerCollisionDesc.fDisY, &m_tPlayerCollisionDesc.fDisZ))
	{
		m_tPlayerCollisionDesc.bMainCollision = true;
		dynamic_cast<CRock*>(pPlayerObject)->Set_PlayerCollision(m_tPlayerCollisionDesc);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CMoveBlockTwo::Set_Pos()
{
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3	Look;

	XMVector3Normalize(vLook);
	vLook *= 7;
	XMStoreFloat3(&Look, vLook);
	Look.x = -Look.y;
	Look.z = 0.f;
	Look.y = 0.f;

	XMStoreFloat4(&m_vMyPos, vPos);

	vPos += XMLoadFloat3(&Look);
	XMStoreFloat4(&m_vBackUpPos, vPos);

	return S_OK;
}

HRESULT CMoveBlockTwo::Rope_Look(_double TimeDelta)
{
	//랜더 킬때 보여주는것이다 bool조건받아서 이거를 똑같이 해주자
	if (true == m_bIsRenderCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");

		if (nullptr == pPlayerObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		Safe_Release(pGameInstance);

		_vector MyLook = dynamic_cast<CRock*>(pPlayerObject)->Get_Player_ColiderMat().r[3] - m_pRopeTransformCom->Get_State(CTransform::STATE_POSITION);

		_vector	vLookLength = dynamic_cast<CRock*>(pPlayerObject)->Get_Player_ColiderMat().r[3] - m_pRopeTransformCom->Get_State(CTransform::STATE_POSITION);
		_float	fLength = XMVectorGetX(XMVector3Length(vLookLength));

		m_pRopeTransformCom->Set_MyLook(-MyLook);
		m_pRopeTransformCom->Set_ScaleZ(fLength * 0.5f);
	}

	return S_OK;
}

HRESULT CMoveBlockTwo::Move_State(_double TimeDelta)
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (false == m_bMoveCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Magenetic_Skill"), TEXT("Com_Sphere"), 0, 0);
		if (nullptr == pTargetSphere)
			return E_FAIL;
		if (false == pTargetSphere->Get_CheckCollision() && true == m_pSphere->Collision_Sphere(pTargetSphere))
		{
			m_dTimeAcc += TimeDelta;
			_vector SetPos = XMVectorLerp(XMLoadFloat4(&m_vMyPos), XMLoadFloat4(&m_vBackUpPos), m_dTimeAcc);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, SetPos);
			m_bIsRenderCheck = true;


			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float4 Position;
			XMStoreFloat4(&Position, vPosition);
			m_pRopeTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet((Position.x + 8.5f), Position.y, Position.z, 1.f));

			if (1.0 <= m_dTimeAcc)
			{
				m_dTimeAcc = 0.0;
				m_bMoveCheck = true;
				m_bIsRenderCheck = false;
			}

		}

		Safe_Release(pGameInstance);
	}




	return S_OK;
}


HRESULT CMoveBlockTwo::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("MoveBlock1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_RopeModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot3.fbx"), TEXT("Com_RopeModel"), (CComponent**)&m_pRopeModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RopeTransform"), (CComponent**)&m_pRopeTransformCom, &TransformDesc)))
		return E_FAIL;




	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(6.f, 2.f, 6.f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMoveBlockTwo::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("MoveBlock2.fbx"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_RopeModel */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("RopeRobot3.fbx"), TEXT("Com_RopeModel"), (CComponent**)&m_pRopeModelCom)))
		return E_FAIL;


	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com.Transform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RopeTransform"), (CComponent**)&m_pRopeTransformCom, &TransformDesc)))
		return E_FAIL;




	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, -0.5f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 5.f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMoveBlockTwo::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pRopeModelCom->Set_RawValue("g_WorldMatrix", &m_pRopeTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRopeModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRopeModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);

	return S_OK;
}


CMoveBlockTwo * CMoveBlockTwo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMoveBlockTwo* pInstance = new CMoveBlockTwo(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CMoveBlockTwo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMoveBlockTwo::Clone(void* pArg, _uint iMyLevelIndex)
{
	CMoveBlockTwo* pInstance = new CMoveBlockTwo(*this);

	if (FAILED(pInstance->Native_Construct(pArg, iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CMoveBlockTwo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoveBlockTwo::Free()
{
	__super::Free();

	if (true == m_bIsClone)
	{
		if (nullptr != m_pModelDesc_Client)
		{
			Safe_Delete(m_pModelDesc_Client);
		}
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);

	Safe_Release(m_pRopeModelCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pRopeTransformCom);
	Safe_Release(m_pTransformCom);
}
