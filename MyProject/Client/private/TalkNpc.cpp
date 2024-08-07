#include "stdafx.h"
#include "..\public\TalkNpc.h"
#include "GameInstance.h"
#include "Rock.h"
#include "Talk_UI.h"
#include "Camera_Fly.h"

CTalkNpc::CTalkNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTalkNpc::CTalkNpc(const CTalkNpc & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTalkNpc::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTalkNpc::Native_Construct(void* pArg, _uint iMyLevelIndex)
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

		m_pChatTransform->Set_WolrdMatirx_float4x4(m_pTransformCom->Get_WorldFloat4x4());
		m_pChatTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_pModelDesc_Client->vPos.x - 1.f, m_pModelDesc_Client->vPos.y + 3.f, m_pModelDesc_Client->vPos.z + 1.f, 1.f));
		m_pChatTransform->Set_Scailing(2.f);
		m_pTransformCom->Set_Scailing(0.04f);
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Talk_UI"), m_iMyLevel, L"Talk_UI", nullptr,(CGameObject**)&m_pTalkUi)))
		return E_FAIL;

	dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_RenderCheck(false);

	Safe_Release(pGameInstance);

	m_iCurMotion = NPC_CRY;

	return S_OK;
}

_int CTalkNpc::Tick(_double TimeDelta)
{
	if (0 > (__super::Tick(TimeDelta)))
		return -1;

	m_pModelCom->Set_Animation(m_iCurMotion);
	m_pModelCom->Update(TimeDelta, &m_bIsAnimEndCheck);
	m_pModelCom->Set_PreAnimation(m_iCurMotion);

	m_pSphere->Update(m_pTransformCom->Get_WorldMatrix());

	if (true == m_bDeadCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		m_bRenderCheck = true;
		m_dDeadTime += TimeDelta;


		if (false == m_bEffectCheck)
		{
			CVIBuffer_PointInstance::POINTINST tPointInst;
			ZeroMemory(&tPointInst, sizeof(CVIBuffer_PointInstance::POINTINST));
			tPointInst.m_iType = 0;
			tPointInst.m_iSize = 5;
			tPointInst.m_iTextureIndex = 2;
			tPointInst.m_iPassIndex = 0;
			XMStoreFloat4(&tPointInst.m_vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_Effect_Point"), m_iMyLevel, L"Clone_PointEffect", &tPointInst)))
				return E_FAIL;

			m_bEffectCheck = true;
		}
		if (4.0 <= m_dDeadTime)
		{
			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_PetNpc"), m_iMyLevel, L"Clone_PetNpc")))
				return E_FAIL;

			m_dDeadTime = 0.0;
			Safe_Release(pGameInstance);
			return 1;
		}


		Safe_Release(pGameInstance);
	}

	return _int();
}

_int CTalkNpc::LateTick(_double TimeDelta)
{
	if (0 > (__super::LateTick(TimeDelta)))
		return -1;

	if (FAILED(Collision_Check()))
		return E_FAIL;

	if (FAILED(Talk_Start(TimeDelta)))
		return E_FAIL;

	if (false == m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);

	return 0;
}

HRESULT CTalkNpc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	if (FAILED(m_pChatBuffer->Render(3)))
		return E_FAIL;

#ifdef _DEBUG
	m_pSphere->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTalkNpc::Talk_Start(_double TimeDelta)
{ 
	if (true == m_bAnimStart)
	{
		m_iCurMotion = NPC_TALK;

		if(nullptr != m_pTalkUi)
			dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_RenderCheck(true);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (m_iChatIndex == 0 && pGameInstance->Key_Down(VK_RETURN))
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_TALK);
			CSound_Manager::Get_Instance()->PlaySound(L"NpcTalkTwo", CSound_Manager::ID_NPC_TALK);

			dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_TalkIndex(1);
			++m_iChatIndex;
		}
		else if (m_iChatIndex == 1 && pGameInstance->Key_Down(VK_RETURN))
		{
			CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_TALK);
			CSound_Manager::Get_Instance()->PlaySound(L"NpcTalkThree", CSound_Manager::ID_NPC_TALK);

			dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_TalkIndex(2);
			
			if (FAILED(pGameInstance->Add_ObjectToLayer(m_iMyLevel, TEXT("Prototype_GameObject_ItemJetPack"), m_iMyLevel, L"Clone_ItemJetPack")))
				return E_FAIL;

			CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
			dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(1);
		
			++m_iChatIndex;
		}
		else if (m_iChatIndex == 2 && pGameInstance->Key_Down(VK_RETURN))
		{
			dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_RenderCheck(false);
			dynamic_cast<CTalk_UI*>(m_pTalkUi)->Set_DeadCheck(true);
			Safe_Release(m_pTalkUi);

			m_bAnimStart = false;
			m_bDeadCheck = true;
			++m_iChatIndex;
		}

		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CTalkNpc::Collision_Check()
{
	if (nullptr == m_pSphere)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPlayerObject = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Player");
	CColider*		pTargetSphere = (CColider*)pGameInstance->Get_Components(m_iMyLevel, TEXT("Clone_Player"), TEXT("Com_Sphere"), 0, 0);

	if (nullptr == pPlayerObject || nullptr == pTargetSphere)
	{
		Safe_Release(pGameInstance);
		return -1;
	}

	if (false == m_bAnimStart)
	{
		if (m_pSphere->Collision_Sphere(pTargetSphere))
		{
			if (false == m_bCryCheck)
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_TALK);
				CSound_Manager::Get_Instance()->PlaySound(L"Npc_Cry", CSound_Manager::ID_NPC_TALK);
				m_bCryCheck = true;
			}

			if (pGameInstance->Key_Down('F'))
			{
				CSound_Manager::Get_Instance()->StopSound(CSound_Manager::ID_NPC_TALK);
				CSound_Manager::Get_Instance()->PlaySound(L"NpcTalkOne", CSound_Manager::ID_NPC_TALK);

				CGameObject* pCam = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_Camera_Fly");
				dynamic_cast<CCamera_Fly*>(pCam)->Set_MainLook(true);
				dynamic_cast<CCamera_Fly*>(pCam)->Set_iCamIndex(0);
				m_bAnimStart = true;
			}
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}




HRESULT CTalkNpc::SetUp_Component()
{
	/* Com_Renderer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, L"TalkNpc0.fbx", TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_ChatTransform"), (CComponent**)&m_pChatTransform, &TransformDesc)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pChatBuffer)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::SetUp_Component(m_iMyLevel, TEXT("Prototype_Component_Texture_Chat"), TEXT("Com_Texture"), (CComponent**)&m_pChatTexture)))
		return E_FAIL;


	CColider::COLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CColider::COLIDERDESC));

	ColliderDesc.vCenter = _float3(0.0f, 0.f, 0.0f);
	ColliderDesc.vExtents = _float3(50.f, 90.f, 50.f);
	ColliderDesc.fRadius = 80.f;


	if (FAILED(__super::SetUp_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Sphere"), (CComponent**)&m_pSphere, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTalkNpc::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pModelCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));

	m_pChatBuffer->Set_RawValue("g_WorldMatrix", &m_pChatTransform->Get_WorldMatrixTranspose(), sizeof(_matrix));
	m_pChatBuffer->Set_RawValue("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pChatBuffer->Set_RawValue("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_HoldingMatrix(CPipeLine::D3DTS_PROJECTION)), sizeof(_matrix));
	
	m_pChatTexture->SetUp_OnShader("g_DiffuseTexture", m_pChatBuffer, 0);



	Safe_Release(pGameInstance);
	return S_OK;
}

CTalkNpc * CTalkNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTalkNpc*	pInstance = new CTalkNpc(pDevice, pDeviceContext);

	if (FAILED(pInstance->Native_Construct_Prototype()))
	{
		MSGBOX("Failed to Created : CTalkNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTalkNpc::Clone(void* pArg, _uint iMyLevelIndex)
{
	CTalkNpc*	pInstance = new CTalkNpc(*this);

	if (FAILED(pInstance->Native_Construct(pArg,iMyLevelIndex)))
	{
		MSGBOX("Failed to Created : CTalkNpc");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTalkNpc::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete(m_pModelDesc_Client);

	if (nullptr != m_pTalkUi)
	{
		Safe_Release(m_pTalkUi);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSphere);
	
	Safe_Release(m_pChatBuffer);
	Safe_Release(m_pChatTexture);
	Safe_Release(m_pChatTransform);
	Safe_Release(m_pTransformCom);
}
