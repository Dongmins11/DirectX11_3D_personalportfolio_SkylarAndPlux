#include "stdafx.h"
#include "..\Public\Rock.h"
#include "Player_AnimState.h"
#include "GameInstance.h"
#include "RopeRobot.h"
#include "Colleter.h"
#include "NpcCage.h"
#include "Hp_Ui.h"
#include "Crystal_Ui.h"

CRock::CRock(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CRock::CRock(const CRock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRock::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRock::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pModelDesc_Client = new MODELDESC_CLIENT;
		memcpy(m_pModelDesc_Client, pArg, sizeof(MODELDESC_CLIENT));
		if (FAILED(SetUp_Component()))
			return E_FAIL;


		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pModelDesc_Client->vPos));
		m_pTransformCom->Set_Scailing(m_pModelDesc_Client->vScaile);
		m_pTransformCom->Set_ScailingXYZ(m_pModelDesc_Client->vScaileXYZ);
		m_pTransformCom->Set_Rotation(m_pModelDesc_Client->vRotaition);
	}
	else
	{
		if (FAILED(SetUp_Component()))
			return E_FAIL;
	}
	if (LEVEL_STAGE1 == m_iMyLevel)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(232.7f, 14.f, 81.7f, 1.f));

	}
	else if (LEVEL_STAGE2 == m_iMyLevel)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(232.9f,108.9f,12.9f,1.f));

	}
	else if (LEVEL_BOSS_STAGE == m_iMyLevel)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(200.3f, 10.0f, 147.0f, 1.f));
		m_pTransformCom->Set_MyLook(-m_pTransformCom->Get_State(CTransform::STATE_RIGHT));

	}
	m_vecHpUi.reserve(5);
	m_pPlayer_AnimState = CPlayer_AnimState::Create(this, m_pTransformCom, m_pVIBufferCom, m_pAABB, m_pSphere, m_pHandSphere, m_iMyLevel);
	m_pVIBufferCom->Set_Animation(IDLE_ANIM);
	m_pVIBufferCom->Set_PreAnimation(IDLE_ANIM);

	for (_uint i = 0; i < 5; ++i)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CHp_Ui::HPUIDESC tHpUiDesc;
		ZeroMemory(&tHpUiDesc, sizeof(CHp_Ui::HPUIDESC));

		tHpUiDesc.fPosX = (i * 50) + 100;
		tHpUiDesc.fPosY = (i*3) + 100;
		tHpUiDesc.fSizeX = 40;
		tHpUiDesc.fSizeY = 40;

		CHp_Ui*	pHp_Ui = nullptr;

		if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_HP_Ui"), m_iMyLevel, L"Clone_Hp_Ui", &tHpUiDesc,(CGameObject**)&pHp_Ui)))
			return E_FAIL;

		if (nullptr == pHp_Ui)
		{
			MSGBOX("Failed to Create Hp_Ui : To.Player : From NativeConstruct");
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		Safe_Release(pGameInstance);
		m_vecHpUi.push_back(pHp_Ui);
	}

	m_iHp = 4;
	m_iMaxHp = 4;

	return S_OK;
}

_int CRock::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pAABB->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	m_pPlayer_AnimState->Update(TimeDelta);

	Player_ColliderSet();


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (LEVEL_STAGE2 == m_iMyLevel)
	{
		if (false == m_bPetCheck)
		{
			m_dPetTime += TimeDelta;
			if (4.0 <= m_dPetTime)
			{
				if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_PetNpc"), m_iMyLevel, L"Clone_PetNpc")))
					return E_FAIL;

				m_dPetTime = 0.0;
				m_bPetCheck = true;
			}
		}
	}
	else if (LEVEL_BOSS_STAGE == m_iMyLevel)
	{
		if (false == m_bPetCheck)
		{
			m_dPetTime += TimeDelta;
			if (2.0 <= m_dPetTime)
			{
				if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_PetNpc"), m_iMyLevel, L"Clone_PetNpc")))
					return E_FAIL;

				m_dPetTime = 0.0;
				m_bPetCheck = true;
			}
		}
	}

	if (pGameInstance->Key_Down('J'))
	{
		if (LEVEL_BOSS_STAGE == m_iMyLevel)
		{
			CVIBuffer_PointInstance::POINTINST tPointInst;
			ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
			tPointInst.m_iType = 3;
			tPointInst.m_iSize = 5;
			tPointInst.m_iTextureIndex = 2;
			tPointInst.m_iPassIndex = 0;
			XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		//	if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PetPointEffect", &tPointInst)))
			//	return E_FAIL;

		}
		else
		{

			MODELDESC_CLIENT tModelDesc;
			ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));

			XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			tModelDesc.vRotaition = _float3(1.f, 1.f, 1.f);
			tModelDesc.vScaile = 1.f;

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Collector"), m_iMyLevel, L"Clone_Collector", &tModelDesc)))
				return E_FAIL;
		}
	}
	if (pGameInstance->Key_Down('M'))
	{
		if (LEVEL_STAGE1 == m_iMyLevel)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(16.f,35.f,348.f,1.f));
		else if (LEVEL_STAGE2 == m_iMyLevel)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(232.9f, 70.9f, 33.f, 1.f));
	}
	if (pGameInstance->Key_Down('N'))
	{
		if (LEVEL_STAGE1 == m_iMyLevel)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(16.f, 35.f, 348.f, 1.f));
		else if (LEVEL_STAGE2 == m_iMyLevel)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.1f, 50.9f, 300.f, 1.f));
		else if (LEVEL_BOSS_STAGE == m_iMyLevel)
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(140.4f, 9.0f, 143.84f, 1.f));
		}
	}
	if (pGameInstance->Key_Down('B'))
	{
		if (LEVEL_STAGE2 == m_iMyLevel)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(233.1f, 148.f, 426.f, 1.f));
	}
	if (pGameInstance->Key_Down('I'))
	{
		m_iHp -= 1;

		if (-1 >= m_iHp)
			m_iHp = -1;
	}
	if (pGameInstance->Key_Down('O'))
	{
		m_iHp += 1;

		if (m_iHp >= m_iMaxHp)
			m_iHp = m_iMaxHp;
	}
	Safe_Release(pGameInstance);

	if (FAILED(HP_UiManager(TimeDelta)))
		return -1;

	if (true == m_bBounce)
		Bouns_Pos(TimeDelta);



	return 0;
}

_int CRock::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;


	Jump(TimeDelta * 0.7);
	Gravity(TimeDelta * 0.7);

	if (FAILED(Collision_Check()))
		return -1;
	if (FAILED(Collision_Hit_Check(TimeDelta)))
		return -1;
	if (FAILED(BackUp_Pos()))
		return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return 0;
}

HRESULT CRock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pVIBufferCom->Render(0);

#ifdef _DEBUG
	m_pAABB->Render();
	m_pSphere->Render();
	m_pHandSphere->Render();
#endif // _DEBUG



	return S_OK;
}

CRock::PLAYER_LOOK CRock::Get_PlayerLook()
{
	if (nullptr == m_pPlayer_AnimState)
		return PLAYER_LOOK_END;

	return m_pPlayer_AnimState->Get_PlayerLook();
}


HRESULT CRock::Bouns_Pos(_double TimeDelta)
{
	m_dBounceTimeAcc += TimeDelta * 1.5;

	_vector vPos;
	vPos = XMVectorLerp(XMLoadFloat4(&m_vBounceInitialPos), XMLoadFloat4(&m_vBounceGoalPos), m_dBounceTimeAcc);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	if (1.0 <= m_dBounceTimeAcc)
	{
		m_dBounceTimeAcc = 0.f;
		m_bBounce = false;
	}

	return S_OK;
}

void CRock::Set_Bounce(_float3 vDir, _float fPower, _bool bBounceCheck)
{
	m_iHp -=1;
	m_vBounceDir = vDir;
	m_fBouncePower = fPower;
	m_bBounce = bBounceCheck;
	XMStoreFloat4(&m_vBounceInitialPos,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&m_vBounceGoalPos,XMLoadFloat4(&m_vBounceInitialPos) + XMLoadFloat3(&m_vBounceDir)*m_fBouncePower);
}


void CRock::Set_RopeObject(CRopeRobot * pRopeObject)
{
	if (nullptr == pRopeObject)
		return;

	m_pPlayer_AnimState->Set_RopeObject(pRopeObject);

}

void CRock::Set_DeadAnimation()
{
	m_pPlayer_AnimState->Dead_Anim();
}

void CRock::Set_NpcCage(CTransform * pTargetTransform, CGameObject* pTargetObject)
{
	if (nullptr == pTargetTransform)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down('G'))
	{
		dynamic_cast<CNpcCage*>(pTargetObject)->Set_AnimationStart(true);
		for (_uint i = 0; i < 10; ++i)
		{
			MODELDESC_CLIENT tModelDesc;
			CGameObject* pGameObject = nullptr;
			ZeroMemory(&tModelDesc, sizeof(MODELDESC_CLIENT));

			XMStoreFloat4(&tModelDesc.vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat3(&tModelDesc.vScaileXYZ, pTargetTransform->Get_State(CTransform::STATE_POSITION));
			tModelDesc.vRotaition = _float3(1.f, 1.f, 1.f);
			tModelDesc.vScaile = 1.f;
			tModelDesc.iType = 1;
			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Collector"), m_iMyLevel, L"Clone_Collector", &tModelDesc,(CGameObject**)&pGameObject)))
				return;

			dynamic_cast<CColleter*>(pGameObject)->Set_TargetObject(pTargetObject);

			Safe_Release(pGameObject);
		}
		CGameObject* pUi = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Crystal_Ui");
		if (10 <= dynamic_cast<CCrystal_Ui*>(pUi)->Get_CristalNumber())
			dynamic_cast<CCrystal_Ui*>(pUi)->Set_CristalMinus(10);
	}
	Safe_Release(pGameInstance);

}

_bool CRock::Get_HitCheck()
{
	if (nullptr == m_pPlayer_AnimState)
		return false;

	return m_pPlayer_AnimState->Get_HitCheck();
}

HRESULT CRock::Magenetic_SkillOn()
{
	if (nullptr == m_pPlayer_AnimState)
		return E_FAIL;

	if (FAILED(m_pPlayer_AnimState->Magenetic_Change()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRock::Magenetic_SkillDown(_bool bCheck)
{
	if (nullptr == m_pPlayer_AnimState)
		return E_FAIL;

	if (FAILED(m_pPlayer_AnimState->Set_Magenetic_Idle(bCheck)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRock::Collision_Check()
{
	if (nullptr == m_pPlayer_AnimState)
		return E_FAIL;
	m_pPlayer_AnimState->Collision_Check();

	return S_OK;
}

HRESULT CRock::BackUp_Pos()
{
	if (nullptr == m_pPlayer_AnimState)
		return E_FAIL;

	m_pPlayer_AnimState->BackUp_Pos();

	return S_OK;
}

HRESULT CRock::Collision_Hit_Check(_double TimeDelta)
{
	if (nullptr == m_pSphere)
		return E_FAIL;

	if (true == m_bIsColision)
	{
		m_pSphere->Set_CheckCollision(true);
		m_pPlayer_AnimState->Check_Hit(true);
		m_CoolTimeCheck += TimeDelta;
		if (2.0 <= m_CoolTimeCheck)
		{
			m_CoolTimeCheck = 0.0;
			m_pPlayer_AnimState->Set_Hit(false);
			m_bIsColision = false;
		}
	}
	else
	{
		m_CoolTimeCheck = 0.0;
		m_pPlayer_AnimState->Check_Hit(false);
		m_pSphere->Set_CheckCollision(false);
	}


	
	
	return S_OK;
}

HRESULT CRock::HP_UiManager(_double TimeDelta)
{
	if (false == m_bAllHpRenderCheck)
	{
		if (0 <= m_iHp)
		{
			for (_uint i = 0; i <= m_iMaxHp; ++i)
			{
				if (m_iHp != i)
					m_vecHpUi[i]->Set_TransCheck(false);
				else
					m_vecHpUi[i]->Set_TransCheck(true);

				if (m_iHp < i)
					m_vecHpUi[i]->Set_PassIndedx(12);
				else
					m_vecHpUi[i]->Set_PassIndedx(1);
			}
		}
		else
		{
			for (_uint i = 0; i <= m_iMaxHp; ++i)
			{
				m_vecHpUi[i]->Set_TransCheck(false);
				m_vecHpUi[i]->Set_PassIndedx(12);
			}
		}
	}
	else
	{
		for (auto& iter : m_vecHpUi)
			iter->Set_RenderCheck(true);
	}
	return S_OK;
}

void CRock::Jump(_double TimeDelta)
{
	if (nullptr == m_pPlayer_AnimState)
		return;
	
	m_pPlayer_AnimState->Jump(TimeDelta);

}

void CRock::Gravity(_double TimeDelta)
{
	if (nullptr == m_pPlayer_AnimState)
		return;

	m_pPlayer_AnimState->Gravity(TimeDelta);
}

void CRock::Player_ColliderSet()
{
	if (nullptr != m_pMatCombined)
	{
		_matrix TransformationMatrix = XMLoadFloat4x4(&m_MatOffset) * XMLoadFloat4x4(m_pMatCombined) * XMLoadFloat4x4(&m_MatPivot) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldFloat4x4());
		m_pHandSphere->Update(XMLoadFloat4x4(&m_MatWorldColider) * TransformationMatrix);
		XMStoreFloat4x4(&m_HandColiderMat,XMLoadFloat4x4(&m_MatWorldColider) * TransformationMatrix);
	}
}


 
HRESULT CRock::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	///* Com_VIBuffer */
	//if (FAILED(__super::SetUp_Component(LEVEL_STATIC, m_pModelDesc_Client->wstrPath, TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	//	return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, L"Skylar1.fbx", TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CNavigation::NAVIDESC		NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentIndex = 0;
	m_iCurrntIndex = NaviDesc.iCurrentIndex;
	/* Com_Navigation */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_VIBuffer_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom,&NaviDesc)))
		return E_FAIL;

	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.8f, 0.0f);
	ColliderDesc.vExtents = _float3(0.5f, 0.9f, 0.5f);
	ColliderDesc.fRadius = 1.f;

	/* Com_Colider_AABB */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABB, &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 25.f;

	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_HandSphere"), (CComponent**)&m_pHandSphere, &ColliderDesc)))
		return E_FAIL;

	_float4x4 PivotMat = m_pVIBufferCom->Get_PivotMatrix();
	XMStoreFloat4x4(&PivotMat, (XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(XMConvertToRadians(180.0f))));
	m_pVIBufferCom->Set_PivotMat(XMLoadFloat4x4(&PivotMat));

	_matrix ColiderMatrix;
	ColiderMatrix = XMMatrixIdentity();
	ColiderMatrix.r[3] = XMVectorSet(-100.f, 0.f, -140.f, 1.f);
	XMStoreFloat4x4(&m_MatWorldColider, ColiderMatrix);


	
	m_MatOffset = m_pVIBufferCom->Get_OffsetMatrix("rt_wrist");
	m_MatPivot = m_pVIBufferCom->Get_PivotMatrix();
	m_pMatCombined = m_pVIBufferCom->Get_CombinedMatrixPtr("rt_wrist");
	

	return S_OK;
}

HRESULT CRock::SetUp_ConstantTable()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pVIBufferCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pVIBufferCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	Safe_Release(pGameInstance);
	return S_OK;
}

CRock * CRock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRock*	pInstance = new CRock(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CRock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRock::Clone(void* pArg, _uint iMyLevelIndex)
{
	CRock*	pInstance = new CRock(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CRock");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRock::Free()
{
	__super::Free();

	for (auto iter : m_vecHpUi)
	{
		Safe_Release(iter);
	}
	m_vecHpUi.clear();
	m_vecHpUi.swap(vector<CHp_Ui*>());


	Safe_Release(m_pAABB);
	Safe_Release(m_pSphere);
	Safe_Release(m_pHandSphere);
	Safe_Release(m_pNavigationCom);
	Safe_Delete(m_pModelDesc_Client);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPlayer_AnimState);


}
