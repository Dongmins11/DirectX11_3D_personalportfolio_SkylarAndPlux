#include "stdafx.h"
#include "..\public\NpcCage.h"
#include "GameInstance.h"
#include "Npc.h"
#include "Rock.h"
#include "Box_Ui.h"
#include "Sound_Manager.h"

CNpcCage::CNpcCage(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CNpcCage::CNpcCage(const CNpcCage & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNpcCage::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcCage::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));

		if (FAILED(SetUp_Component_Load()))
			return E_FAIL;

		m_pFloorTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pFloorTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		//m_pFloorTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		//m_pFloorTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);


		
		m_pMainDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());

		m_pLeftDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());
		m_pLeftDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pModelDesc_Client->vPos.x + 1.8f, m_pModelDesc_Client->vPos.y + 0.283817f, m_pModelDesc_Client->vPos.z, 1.f));
		m_pLeftDoorTransformCom->Set_Rotation(_float3(0.f, 90.f, 0.f));

		m_pRightDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());
		m_pRightDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pModelDesc_Client->vPos.x - 1.8f, m_pModelDesc_Client->vPos.y + 0.284817f, m_pModelDesc_Client->vPos.z, 1.f));
		m_pRightDoorTransformCom->Set_Rotation(_float3(0.f, -90.f, 0.f));

		m_pBackDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());
		m_pBackDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pModelDesc_Client->vPos.x, m_pModelDesc_Client->vPos.y + 0.284817f, m_pModelDesc_Client->vPos.z - 1.5f, 1.f));
		m_pBackDoorTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));

		m_pBackDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());
		m_pLockTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pModelDesc_Client->vPos.x + 0.11407f, m_pModelDesc_Client->vPos.y + 3.2536f, m_pModelDesc_Client->vPos.z + 2.2751f, 1.f));
		m_pLockTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));


		m_pCeilingTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		MODELDESC_CLIENT tModel_Desc;
		ZeroMemory(&tModel_Desc, sizeof(MODELDESC_CLIENT));
		XMStoreFloat4(&tModel_Desc.vPos, m_pFloorTransformCom->Get_State(CTransform::STATE_POSITION));


		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Npc"), m_iMyLevel, L"Clone_Npc", &tModel_Desc, (CGameObject**)&m_pNpcObject)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;

		m_pFloorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 14.f, 80.f, 1.f));

		m_pMainDoorTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());

		m_pLeftDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f + 1.8f, 14.f + 0.283817f, 80.f, 1.f));
		m_pLeftDoorTransformCom->Set_Rotation(_float3(0.f, 90.f, 0.f));

		m_pRightDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f - 1.8f, 14.f + 0.284817f, 80.f, 1.f));
		m_pRightDoorTransformCom->Set_Rotation(_float3(0.f, -90.f, 0.f));

		m_pBackDoorTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f, 14.f + 0.284817f, 80 - 1.5f, 1.f));
		m_pBackDoorTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));

		m_pLockTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.f + 0.11407f , 14.f + 3.2536f, 80 + 2.2751f, 1.f));
		m_pLockTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));

		m_pCeilingTransformCom->Set_WolrdMatirx_float4x4(m_pFloorTransformCom->Get_WorldFloat4x4());

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		MODELDESC_CLIENT tModel_Desc;
		ZeroMemory(&tModel_Desc, sizeof(MODELDESC_CLIENT));
		XMStoreFloat4(&tModel_Desc.vPos, m_pFloorTransformCom->Get_State(CTransform::STATE_POSITION));


		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Npc"), m_iMyLevel, L"Clone_Npc",&tModel_Desc,(CGameObject**)&m_pNpcObject)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}

	m_fStartValue = 0.1f;
	m_fEndValue = 100.f;
	m_iBlendTime = 2;
	m_fBlendValue = 80.f;

	XMStoreFloat4(&m_vLockerDestPos,m_pLockTransformCom->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&m_vMainDestPos,m_pMainDoorTransformCom->Get_State(CTransform::STATE_POSITION));

	m_vMainSourcePos = _float4(m_vMainDestPos.x, m_vMainDestPos.y + 10, m_vMainDestPos.z, 1.f);
	m_vLockerSourcePos = _float4(m_vLockerDestPos.x, m_vLockerDestPos.y + 10, m_vLockerDestPos.z,1.f);

	return S_OK;
}

_int CNpcCage::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	if (FAILED(Animation_Play(TimeDelta)))
		return E_FAIL;

	m_pSphere->Update(m_pFloorTransformCom->Get_WorldMatrix());

	if (true == m_bMainRenderCheck)
	{
		m_dDeadTime += TimeDelta;

		if (1.0 <= m_dDeadTime)
		{
			if (false == m_bEffectCheck)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				CVIBuffer_PointInstance::POINTINST tPointInst;
				ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
				tPointInst.m_iType = 0;
				tPointInst.m_iSize = 5;
				tPointInst.m_iTextureIndex = 2;
				tPointInst.m_iPassIndex = 0;
				XMStoreFloat4(&tPointInst.m_vPosition, m_pFloorTransformCom->Get_State(CTransform::STATE_POSITION));

				if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PointEffect", &tPointInst)))
					return E_FAIL;

				

				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_EXPLOSIVE);
				CSound_Manager::Get_Instance()->PlaySound(L"Npc_Explosive", CSound_Manager::ID_NPC_EXPLOSIVE);

				Safe_Release(pGameInstance);

				m_bEffectCheck = true;
			}
			dynamic_cast<CNpc*>(m_pNpcObject)->Set_CageCheck(true);

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			Safe_AddRef(pGameInstance);

			CGameObject* pGameObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Box_Ui");
			dynamic_cast<CBox_Ui*>(pGameObject)->Set_BoxCurNumberAdd(1);
			Safe_Release(pGameInstance);

			m_dDeadTime = 0.0;
			return 1;
		}
	}

	return 0;
}

_int CNpcCage::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(Collision_Check_Cage()))
		return E_FAIL;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	return 0;
}

HRESULT CNpcCage::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pFloorModelCom->Render(0)))
		return E_FAIL;

	if (false == m_bMainRenderCheck)
	{
		if (FAILED(m_pMainDoorModelCom->Render(0)))
			return E_FAIL;
		if (FAILED(m_pCeilingModelCom->Render(0)))
			return E_FAIL;
		if (FAILED(m_pLockModelCom->Render(0)))
			return E_FAIL;
	}
	if (FAILED(m_pLeftDoorModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pRightDoorModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pBackDoorModelCom->Render(0)))
		return E_FAIL;




#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CNpcCage::Collision_Check_Cage()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject*	pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);


	if (true == m_pSphere->Collision_Sphere(pTargetSphere))
		dynamic_cast<CRock*>(pPlayerObject)->Set_NpcCage(m_pLockTransformCom,this);
	else
		dynamic_cast<CRock*>(pPlayerObject)->Set_NpcCage(nullptr,nullptr);

	Safe_Release(pGameInstance);
	
	return S_OK;
}

HRESULT CNpcCage::Animation_Play(_double TimeDelta)
{

	if (FAILED(Animation_Locker(TimeDelta)))
		return E_FAIL;

	if (FAILED(Animation_Main(TimeDelta)))
		return E_FAIL;

	if (FAILED(Animation_Door(TimeDelta)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcCage::Animation_Main(_double TimeDelta)
{
	if (true == m_bMainAnimationCheck)
	{
		m_dLockerTime += TimeDelta * 5.0;

		_vector MainPosUp = XMVectorLerp(XMLoadFloat4(&m_vMainDestPos), XMLoadFloat4(&m_vMainSourcePos), m_dLockerTime);
		_vector LockerPosUp = XMVectorLerp(XMLoadFloat4(&m_vLockerDestPos), XMLoadFloat4(&m_vLockerSourcePos), m_dLockerTime);

		m_pMainDoorTransformCom->Set_State(CTransform::STATE_POSITION, MainPosUp);
		m_pCeilingTransformCom->Set_State(CTransform::STATE_POSITION, MainPosUp);
		m_pLockTransformCom->Set_State(CTransform::STATE_POSITION, LockerPosUp);

		if (1.0 <= m_dLockerTime)
		{
			m_dLockerTime = 0.0;
			m_bMainRenderCheck = true;
			m_bIsPlayCheck = true;
			m_bMainAnimationCheck = false;
		}
	}


	return S_OK;
}

HRESULT CNpcCage::Animation_Door(_double TimeDelta)
{
	if (true == m_bIsPlayCheck)
	{
		m_dAnimationPlayTime += TimeDelta * (_double)m_iBlendTime;


		if (FAILED(Animation_Left(m_dAnimationPlayTime)))
			return E_FAIL;

		if (FAILED(Animation_Right(m_dAnimationPlayTime)))
			return E_FAIL;

		if (FAILED(Animation_Back(m_dAnimationPlayTime)))
			return E_FAIL;

		if (1.0 <= m_dAnimationPlayTime)
		{
			_uint iAnimationIndex = m_iAnimationIndex % 2;

			if (0 == iAnimationIndex)
			{
				m_fStartValue = 100.f;
				m_fEndValue = m_fBlendValue;

				m_iBlendTime += 2;
			}
			else
			{
				m_fStartValue = m_fEndValue;
				m_fEndValue = 100.f;

				m_fBlendValue += 10.f;

				if (3 == m_iAnimationIndex)
					m_fBlendValue -= 5.f;

				m_iBlendTime += 2;
			}


			if (5 == m_iAnimationIndex)
			{
				m_bIsPlayCheck = false;
				
			}

			m_dAnimationPlayTime = 0.0;
			++m_iAnimationIndex;
		}

	}

	return S_OK;
}

HRESULT CNpcCage::Animation_Left(_double TimeDelta)
{
	if (nullptr == m_pLeftDoorTransformCom)
		return E_FAIL;

	_float RoationZ = m_fStartValue * (1.0 - TimeDelta) + m_fEndValue * TimeDelta;

	m_pLeftDoorTransformCom->Set_Rotation(_float3(0.f, 90.f, -RoationZ));

	return S_OK;
}

HRESULT CNpcCage::Animation_Right(_double TimeDelta)
{
	if (nullptr == m_pRightDoorTransformCom)
		return E_FAIL;

	_float RoationZ = m_fStartValue * (1.0 - TimeDelta) + m_fEndValue * TimeDelta;

	m_pRightDoorTransformCom->Set_Rotation(_float3(0.f, -90.f, RoationZ));

	return S_OK;
}

HRESULT CNpcCage::Animation_Back(_double TimeDelta)
{
	if (nullptr == m_pBackDoorTransformCom)
		return E_FAIL;

	_float RoationX = m_fStartValue * (1.0 - TimeDelta) + m_fEndValue * TimeDelta;

	m_pBackDoorTransformCom->Set_Rotation(_float3(RoationX, 180.f, 0.f));

	return S_OK;
}

HRESULT CNpcCage::Animation_Locker(_double TimeDelta)
{
	if (nullptr == m_pLockTransformCom)
		return E_FAIL;

	if (true == m_bIsLockCheck)
	{
		m_dLockerTime += TimeDelta * 2.0;
		_float LokcerScale = 1.f * (1.0 - m_dLockerTime) + 3.f * m_dLockerTime;
		m_pLockTransformCom->Set_Scailing(LokcerScale);

		if (1.f <= m_dLockerTime)
		{
			m_dLockerTime = 0.0;
			m_bIsLockCheck = false;
		}
	}



	if (10 <= m_iLockerCount)
	{
		if (false == m_bIsLockCheck)
		{
			m_dLockerTime += TimeDelta;
			_float LokcerScale = 3.f * (1.0 - m_dLockerTime) + 1.f * m_dLockerTime;
			m_pLockTransformCom->Set_Scailing(LokcerScale);

			if (1.f <= m_dLockerTime)
			{
				m_iLockerCount = 0;
				m_dLockerTime = 0.0;
				m_bMainAnimationCheck = true;

				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_HELP);
				CSound_Manager::Get_Instance()->PlaySound(L"Npc_HelpBGM", CSound_Manager::ID_NPC_HELP);
			}

		}
	}
	

	return S_OK;
}



HRESULT CNpcCage::SetUp_Component_Load()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */ /*For.Floor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pFloorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.Ceiling*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pCeilingModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.MainDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage2.fbx"), TEXT("Com_Model"), (CComponent**)&m_pMainDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.BackDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage3.fbx"), TEXT("Com_Model"), (CComponent**)&m_pBackDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.LeftDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage4.fbx"), TEXT("Com_Model"), (CComponent**)&m_pLeftDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.RightDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage5.fbx"), TEXT("Com_Model"), (CComponent**)&m_pRightDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.LockDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage6.fbx"), TEXT("Com_Model"), (CComponent**)&m_pLockModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_FloorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_FloorTransform"), (CComponent**)&m_pFloorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_CeilingTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_CeilingTransform"), (CComponent**)&m_pCeilingTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_MainDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_MainDoorTransform"), (CComponent**)&m_pMainDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_BackDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_BackDoorTransform"), (CComponent**)&m_pBackDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_LeftDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftDoorTransform"), (CComponent**)&m_pLeftDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_RightDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightDoorTransform"), (CComponent**)&m_pRightDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_LockTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LockTransform"), (CComponent**)&m_pLockTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 10.0f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpcCage::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */ /*For.Floor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage0.fbx"), TEXT("Com_Model"), (CComponent**)&m_pFloorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.Ceiling*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage1.fbx"), TEXT("Com_Model"), (CComponent**)&m_pCeilingModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.MainDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage2.fbx"), TEXT("Com_Model"), (CComponent**)&m_pMainDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.BackDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage3.fbx"), TEXT("Com_Model"), (CComponent**)&m_pBackDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.LeftDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage4.fbx"), TEXT("Com_Model"), (CComponent**)&m_pLeftDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.RightDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage5.fbx"), TEXT("Com_Model"), (CComponent**)&m_pRightDoorModelCom)))
		return E_FAIL;

	/* Com_Model */ /*For.LockDoor*/
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("NpcCage6.fbx"), TEXT("Com_Model"), (CComponent**)&m_pLockModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_FloorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_FloorTransform"), (CComponent**)&m_pFloorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_CeilingTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_CeilingTransform"), (CComponent**)&m_pCeilingTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_MainDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_MainDoorTransform"), (CComponent**)&m_pMainDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_BackDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_BackDoorTransform"), (CComponent**)&m_pBackDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_LeftDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LeftDoorTransform"), (CComponent**)&m_pLeftDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_RightDoorTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_RightDoorTransform"), (CComponent**)&m_pRightDoorTransformCom, &TransformDesc)))
		return E_FAIL;

	/* Com_LockTransform */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_LockTransform"), (CComponent**)&m_pLockTransformCom, &TransformDesc)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.3f, 0.0f);
	ColliderDesc.vExtents = _float3(4.7f, 0.7f, 4.7f);
	ColliderDesc.fRadius = 10.0f;

	/* Com_Sphere */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpcCage::SetUp_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	m_pFloorModelCom->Set_RawValue("g_WorldMatrix", &m_pFloorTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pFloorModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pFloorModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pMainDoorModelCom->Set_RawValue("g_WorldMatrix", &m_pMainDoorTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pMainDoorModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pMainDoorModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pLeftDoorModelCom->Set_RawValue("g_WorldMatrix", &m_pLeftDoorTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLeftDoorModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLeftDoorModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pRightDoorModelCom->Set_RawValue("g_WorldMatrix", &m_pRightDoorTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pRightDoorModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pRightDoorModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pBackDoorModelCom->Set_RawValue("g_WorldMatrix", &m_pBackDoorTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pBackDoorModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pBackDoorModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pLockModelCom->Set_RawValue("g_WorldMatrix", &m_pLockTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pLockModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pLockModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	m_pCeilingModelCom->Set_RawValue("g_WorldMatrix", &m_pCeilingTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pCeilingModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pCeilingModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));


	Safe_Release(pGameInstance);

	return S_OK;
}

CNpcCage * CNpcCage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNpcCage* pInstance = new CNpcCage(pDevice, pDeviceContext);
	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Create CNpcCage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNpcCage::Clone(void* pArg, _uint iMyLevelIndex)
{
	CNpcCage* pInstance = new CNpcCage(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Create CNpcCage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcCage::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	if (nullptr != m_pNpcObject)
		Safe_Release(m_pNpcObject);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pSphere);

	Safe_Release(m_pFloorTransformCom);
	Safe_Release(m_pMainDoorTransformCom);
	Safe_Release(m_pLeftDoorTransformCom);
	Safe_Release(m_pRightDoorTransformCom);
	Safe_Release(m_pBackDoorTransformCom);
	Safe_Release(m_pLockTransformCom);
	Safe_Release(m_pCeilingTransformCom);

	Safe_Release(m_pFloorModelCom);
	Safe_Release(m_pMainDoorModelCom);
	Safe_Release(m_pLeftDoorModelCom);
	Safe_Release(m_pRightDoorModelCom);
	Safe_Release(m_pBackDoorModelCom);
	Safe_Release(m_pLockModelCom);
	Safe_Release(m_pCeilingModelCom);
}
