#include "stdafx.h"
#include "..\public\Camera_Fly.h"
#include "GameInstance.h"
#include "TalkNpc.h"
#include "Render_BossHead.h"
#include "FadeInOut_UI.h"
#include "Npc.h"
#include <random>
CCamera_Fly::CCamera_Fly(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice, pDeviceContext)
{
}

CCamera_Fly::CCamera_Fly(const CCamera_Fly & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Fly::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Fly::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg, iMyLevelIndex)))
		return E_FAIL;

	//ShowCursor(false);

	if (FAILED(SetUp_Component()))
		return E_FAIL;
	if (LEVEL_STAGE1 == m_iMyLevel)
	{
		m_fDisZ = -4.5f;
		m_fDisY = 1.9;
	}
	if (LEVEL_STAGE2 == m_iMyLevel)
	{
		m_fDisZ = -5.0f;
		m_fDisY = 1.9;
	}
	else if (LEVEL_BOSS_STAGE == m_iMyLevel)
	{
		m_fDisZ = -13.0f;
		m_fDisY = 5;
	}
	
	return S_OK;
}

_int CCamera_Fly::Tick(_double TimeDelta)
{
	//POINT		ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };
	//ClientToScreen(g_hWnd, &ptMouse);

	//SetCursorPos(ptMouse.x, ptMouse.y);

	if (false == m_bMainLookCheck)
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

		_long		MouseMove = 0;

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);

		if (MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
		{
			if (true == Update_MouseMove(TimeDelta, MouseMove))
				m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.1f);
		}

		
		if (FAILED(Player_SetPosition(TimeDelta)))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);
	}
	else
	{
		if (FAILED(Main_Look(TimeDelta)))
			return E_FAIL;
	}


	return __super::Tick(TimeDelta);
}

_int CCamera_Fly::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CCamera_Fly::Render()
{
	return S_OK;
}

HRESULT CCamera_Fly::Main_Look(_double TimeDelta)
{
	if (m_iPreIndex != m_iCamIndex)
	{
		m_iPreIndex = m_iCamIndex;
		m_bTestCheck = false;
	}

	switch (m_iCamIndex)
	{
	case 0:
		Talk_NpcLook(TimeDelta);
		break;
	case 1:
		Talk_ItemLook(TimeDelta);
		break;
	case 2:
		Gravity_Look(TimeDelta);
		break;
	case 3:
		BackUp_Look(TimeDelta);
		break;
	case 4:
		Stage2Boss_Look(TimeDelta);
		break;
	case 5:
		Boss_Look(TimeDelta);
		break;
	case 6:
		Cam_Shake(TimeDelta);
		break;
	case 7:
		End_Scence(TimeDelta);
		break;
	case 8:
		Cam_ShakeTwo(TimeDelta);
		break;
	case 9:
		Cam_ShakeThree(TimeDelta);
		break;
	case 10:
		Cam_ShakeFor(TimeDelta);
		break;
	}
	return S_OK;
}

HRESULT CCamera_Fly::Boss_Look(_double TimeDelta)
{
	if (false == m_bTestCheck)
	{
		m_bBackupMat = m_pTransform->Get_WorldFloat4x4();

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		if (nullptr == pPlayerTransform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		_float4 PlayerPos;
		_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat4(&PlayerPos, vPlayerPos);
		PlayerPos.x = PlayerPos.x + 5;
		PlayerPos.y = PlayerPos.y + 4;
		PlayerPos.z = PlayerPos.z - 4;
		_vector vLookPos = XMVectorSet(150.4f, 25.0f, 143.84f, 1.f);

		_vector vCamDir = vLookPos - XMLoadFloat4(&PlayerPos);
		_float3	Look;
		_vector vRight;
		_vector vUp;


		XMStoreFloat3(&Look, XMVector3Normalize(vCamDir) * XMVectorGetZ(m_pTransform->Get_Scale()));
		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&Look));
		vUp = XMVector3Cross(XMLoadFloat3(&Look), vRight);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat3(&Look));

		m_bTestCheck = true;

		Safe_Release(pGameInstance);
	}


	if (true == m_bTestCheck && false == m_bBossCheckTwo)
	{
		m_dBossLookTimeAcc += TimeDelta * 0.3;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		if (nullptr == pPlayerTransform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		_float4 PlayerPos;
		_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat4(&PlayerPos, vPlayerPos);
		PlayerPos.x = PlayerPos.x + 5;
		PlayerPos.y = PlayerPos.y + 4;
		PlayerPos.z = PlayerPos.z - 4;
		_vector vLookPos = XMVectorSet(150.4f, 25.0f, 143.84f, 1.f);


		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorLerp(XMLoadFloat4(&PlayerPos), vLookPos, m_dBossLookTimeAcc));

		if (1.0 <= m_dBossLookTimeAcc)
		{
			CGameObject*	pInOut = pGameInstance->Get_GameObject(m_iMyLevel, L"Clone_FadeInOut_Ui");
			if (nullptr == pInOut)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			dynamic_cast<CFadeInOut_UI*>(pInOut)->Set_Value(1);
			dynamic_cast<CFadeInOut_UI*>(pInOut)->Set_FadeCheck(true);

			m_dBossLookTimeAcc = 0.0;
			m_bBossCheckTwo = true;
		}
		Safe_Release(pGameInstance);
	}


	return S_OK;
}

HRESULT CCamera_Fly::Stage2Boss_Look(_double TimeDelta)
{
	if (false == m_bTestCheck)
	{
		m_bBackupMat = m_pTransform->Get_WorldFloat4x4();

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		CTransform* pTargetTrasnform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"BossHead0.fbx", L"Com_Transform", 0, 0);
		if (nullptr == pPlayerTransform && nullptr == pTargetTrasnform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		_float4 PlayerPos;
		_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat4(&PlayerPos, vPlayerPos);
		PlayerPos.y = PlayerPos.y + 10;
		_vector vBossPos = pTargetTrasnform->Get_State(CTransform::STATE_POSITION);

		_vector vCamDir = vBossPos - vPlayerPos;
		_float3	Look;
		_vector vRight;
		_vector vUp;


		XMStoreFloat3(&Look, XMVector3Normalize(vCamDir) * XMVectorGetZ(m_pTransform->Get_Scale()));
		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&Look));
		vUp = XMVector3Cross(XMLoadFloat3(&Look), vRight);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat3(&Look));

		m_bTestCheck = true;

		Safe_Release(pGameInstance);

	}

	if (true == m_bTestCheck && false == m_bBossCheck)
	{
		m_dBossLookTimeAcc += TimeDelta;

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0, 0);
		CTransform* pTargetTrasnform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"BossHead0.fbx", L"Com_Transform", 0, 0);
		if (nullptr == pPlayerTransform && nullptr == pTargetTrasnform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		_float4 PlayerPos;
		_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat4(&PlayerPos, vPlayerPos);
		PlayerPos.y = PlayerPos.y + 10;
		
		_float4 BossPos;
		_vector vBossPos = pTargetTrasnform->Get_State(CTransform::STATE_POSITION);
		XMStoreFloat4(&BossPos, vBossPos);
		BossPos.z = BossPos.z - 10.f;

		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorLerp(XMLoadFloat4(&PlayerPos), XMLoadFloat4(&BossPos), m_dBossLookTimeAcc));
		
		if (1.0 <= m_dBossLookTimeAcc)
		{
			CGameObject* pGameObject = pGameInstance->Get_GameObject(m_iMyLevel, L"BossHead0.fbx");
			if (nullptr == pGameObject)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}
			dynamic_cast<CRender_BossHead*>(pGameObject)->Set_AnimCheck(true);
			m_dBossLookTimeAcc = 0.0;
			m_bBossCheck = true;
		}

		Safe_Release(pGameInstance);
	}



	return S_OK;
}

HRESULT CCamera_Fly::BackUp_Look(_double TimeDelta)
{
	m_pTransform->Set_WolrdMatirx_float4x4(m_bBackupMat);

	m_bMainLookCheck = false;

	return S_OK;
}

HRESULT CCamera_Fly::Gravity_Look(_double TimeDelta)
{
	if (false == m_bTestCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"GravityMesh0.fbx", L"Com_Transform", 0, 0);
		if (nullptr == pTargetTransform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_bBackupMat = m_pTransform->Get_WorldFloat4x4();
		_vector TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);
		_vector	MyPos = XMVectorSet(157.f, 45.f, 248.f, 1.f);
		_float3	Look;
		_vector vRight;
		_vector vUp;
		_vector vLook = TargetPos - MyPos;

		XMStoreFloat3(&Look, XMVector3Normalize(vLook) * XMVectorGetZ(m_pTransform->Get_Scale()));
		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMLoadFloat3(&Look));
		vUp = XMVector3Cross(XMLoadFloat3(&Look), vRight);

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat3(&Look));
		m_pTransform->Set_State(CTransform::STATE_POSITION, MyPos);


		Look.y *= 0.35f;
		m_pTransform->Set_MyLook(XMLoadFloat3(&Look));
		

		Safe_Release(pGameInstance);
		m_bTestCheck = true;
	}
	return S_OK;
}

HRESULT CCamera_Fly::Talk_NpcLook(_double TimeDelta)
{
	if (false == m_bTestCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);


		CGameObject*	pGameObject = pGameInstance->Get_GameObject(m_iMyLevel, L"TalkNpc0.fbx");
		CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"TalkNpc0.fbx", L"Com_Transform", 0, 0);
		if (nullptr == pGameObject || nullptr == pTargetTransform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_bBackupMat = m_pTransform->Get_WorldFloat4x4();
		_float3	vLook;
		_vector vLook2;
		_float4 vPosition;
		_vector vRight;
		_vector vUp;

		XMStoreFloat3(&vLook, pTargetTransform->Get_State(CTransform::STATE_LOOK));
		vLook2 = pTargetTransform->Get_State(CTransform::STATE_LOOK);
		XMStoreFloat4(&vPosition, pTargetTransform->Get_State(CTransform::STATE_POSITION));

		vLook2 = XMVector3Normalize(vLook2) * XMVectorGetZ(m_pTransform->Get_Scale());

		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook2);
		vUp = XMVector3Cross(vLook2, vRight);

		vPosition.y += 2.f;
		vPosition.z -= 1.6f;
		vPosition.x -= 2.f;

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook2);
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


		vLook.x = -vLook.x;
		vLook.y = 0.f;
		vLook.z = -vLook.z;
		m_pTransform->Set_MyLook(XMLoadFloat3(&vLook));

		Safe_Release(pGameInstance);
		m_bTestCheck = true;
	}

	return S_OK;
}

HRESULT CCamera_Fly::Talk_ItemLook(_double TimeDelta)
{
	if (false == m_bTestCheck)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform*		pTargetTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_ItemJetPack", L"Com_Transform", 0, 0);
		if (nullptr == pTargetTransform)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_bBackupMat = m_pTransform->Get_WorldFloat4x4();
		_float3	vLook;
		_vector vLook2;
		_float4 vPosition;
		_vector vRight;
		_vector vUp;

		XMStoreFloat3(&vLook, pTargetTransform->Get_State(CTransform::STATE_LOOK));
		vLook2 = pTargetTransform->Get_State(CTransform::STATE_LOOK);
		XMStoreFloat4(&vPosition, pTargetTransform->Get_State(CTransform::STATE_POSITION));

		vLook2 = XMVector3Normalize(vLook2) * XMVectorGetZ(m_pTransform->Get_Scale());

		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook2);
		vUp = XMVector3Cross(vLook2, vRight);

		vPosition.z -= 2.0f;

		m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransform->Set_State(CTransform::STATE_UP, vUp);
		m_pTransform->Set_State(CTransform::STATE_LOOK, vLook2);
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPosition));


		vLook.x = vLook.x;
		vLook.y = 0.f;
		vLook.z = vLook.z;
		m_pTransform->Set_MyLook(XMLoadFloat3(&vLook));

		Safe_Release(pGameInstance);
		m_bTestCheck = true;
	}


	if (true == m_bTestCheck)
	{
		m_bTestTime += TimeDelta;
		if (5.0 <= m_bTestTime)
		{
			m_bTestTime = 0.0;
			m_pTransform->Set_WolrdMatirx_float4x4(m_bBackupMat);
			m_bMainLookCheck = false;
		}
	}

	return S_OK;
}

HRESULT CCamera_Fly::Cam_ShakeTwo(_double TimeDelta)
{
	if (FAILED(Player_SetPosition_ShakeTwo(TimeDelta)))
		return E_FAIL;

	m_bShakeTime += TimeDelta;

	if (0.2 <= m_bShakeTime)
	{
		m_bShakeTime = 0.0;
		m_bMainLookCheck = false;
	}


	return S_OK;
}

HRESULT CCamera_Fly::Cam_ShakeThree(_double TimeDelta)
{
	if (FAILED(Player_SetPosition_ShakeThree(TimeDelta)))
		return E_FAIL;

	m_bShakeTime += TimeDelta;

	if (LEVEL_STAGE1 == m_iMyLevel)
	{
		if (0.6 <= m_bShakeTime)
		{
			m_bShakeTime = 0.0;
			m_bMainLookCheck = false;
		}
	}
	else
	{
		if (0.4 <= m_bShakeTime)
		{
			m_bShakeTime = 0.0;
			m_bMainLookCheck = false;
		}
	}

	return S_OK;
}

HRESULT CCamera_Fly::Cam_ShakeFor(_double TimeDelta)
{
	if (FAILED(Player_SetPosition_ShakeFor(TimeDelta)))
		return E_FAIL;

	m_bShakeTime += TimeDelta;

	if (LEVEL_STAGE1 == m_iMyLevel)
	{
		if (0.6 <= m_bShakeTime)
		{
			m_bShakeTime = 0.0;
			m_bMainLookCheck = false;
		}
	}
	else
	{
		if (0.4 <= m_bShakeTime)
		{
			m_bShakeTime = 0.0;
			m_bMainLookCheck = false;
		}
	}


	return S_OK;
}

HRESULT CCamera_Fly::Cam_Shake(_double TimeDelta)
{

	if (FAILED(Player_SetPosition_Shake(TimeDelta)))
		return E_FAIL;

	m_bShakeTime += TimeDelta;

	if (0.4 <= m_bShakeTime)
	{
		m_bShakeTime = 0.0;
		m_bMainLookCheck = false;
	}


	return S_OK;
}

HRESULT CCamera_Fly::End_Scence(_double TimeDelta)
{

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CTransform* pNpcTransform = (CTransform*)pGameInstance->Get_Components(m_iMyLevel, L"Clone_BossNpc", L"Com_Transform", 0, 0);
		if (nullptr == pNpcTransform)
		{
			MSGBOX("Failed to Camera : Function : End_Secne = Finder NpcTransform");
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		Safe_Release(pGameInstance);

		_matrix MyMat = m_pTransform->Get_WorldMatrix();

		const _float4x4* NpcMat = pNpcTransform->Get_WorldMatrixPtr();
		_vector	Npc = XMLoadFloat4(&*(_float4*)&NpcMat->m[3]);

		m_Distance = XMVectorSet(3.f, 2.f, -20.f, 1.f);
		memcpy(&MyMat.r[3], &Npc, sizeof(_float4));
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

		if (false == m_bEndLookCheck)
		{
			m_pTransform->Set_MyLook(pNpcTransform->Get_State(CTransform::STATE_POSITION) - XMVector3TransformCoord(m_Distance, MyMat));
			m_bEndLookCheck = true;
		}


	return S_OK;
}

HRESULT CCamera_Fly::SetUp_Component()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform* Transform = dynamic_cast<CTransform*>(pGameInstance->Get_Components(m_iMyLevel, L"Clone_Player", L"Com_Transform", 0));
	m_pPlayerMatrix = Transform->Get_WorldMatrixPtr();
	if (nullptr == m_pPlayerMatrix)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CCamera_Fly::Player_SetPosition_Shake(_double TimeDelta)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);

	m_Distance = XMVectorSet((rand() % 2) - 1, m_fDisY + ((rand() % 2) - 1), m_fDisZ, 1.f);

	memcpy(&MyMat.r[3], &PlayerPos, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

	return S_OK;
}

HRESULT CCamera_Fly::Player_SetPosition_ShakeTwo(_double TimeDelta)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);

	m_Distance = XMVectorSet((rand() % 2) - 1, m_fDisY + ((rand() % 2) - 1), m_fDisZ + (rand() % 2) -1, 1.f);

	memcpy(&MyMat.r[3], &PlayerPos, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

	return S_OK;
}

HRESULT CCamera_Fly::Player_SetPosition_ShakeThree(_double TimeDelta)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);

	//mt19937 engine((unsigned int)time(NULL));
	//uniform_real_distribution<> distribution(-0.5f, 0.5f);
	//auto generator = bind(distribution, engine);

	if (LEVEL_STAGE1 == m_iMyLevel)
		m_Distance = XMVectorSet(0.f, m_fDisY, m_fDisZ + (rand() % 2) - 1, 1.f);
	else
		m_Distance = XMVectorSet(0.f, m_fDisY, m_fDisZ + (rand() % 4) - 2, 1.f);

	memcpy(&MyMat.r[3], &PlayerPos, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

	return S_OK;
}

HRESULT CCamera_Fly::Player_SetPosition_ShakeFor(_double TimeDelta)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);

	m_Distance = XMVectorSet(0.f, m_fDisY + (rand() % 2) - 1, m_fDisZ, 1.f);

	memcpy(&MyMat.r[3], &PlayerPos, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

	return S_OK;
}

HRESULT CCamera_Fly::Player_SetPosition(_double TimeDelta)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);

	if(LEVEL_STAGE1 == m_iMyLevel)
		m_Distance = XMVectorSet(0.f, 1.9f, -4.5f, 1.f);
	else if (LEVEL_STAGE2 == m_iMyLevel)
		m_Distance = XMVectorSet(0.f, 1.9f, m_fDisZ, 1.f);
	else if (LEVEL_BOSS_STAGE == m_iMyLevel)
	{
		if (false == m_bDistanceCheck)
		{
			m_fDisZ += -TimeDelta * 3;
			m_fDisY += TimeDelta * 3;

			if (-13.f >= m_fDisZ)
				m_fDisZ = -13.f;
			if (5.f <= m_fDisY)
				m_fDisY = 5.f;
		}
		else
		{
			m_fDisZ += TimeDelta * 3;
			m_fDisY -= TimeDelta * 3;

			if (-8.f <= m_fDisZ)
				m_fDisZ = -8.f;
			if (1.9 >= m_fDisY)
				m_fDisY = 1.9;
		}
		m_Distance = XMVectorSet(0.f, m_fDisY, m_fDisZ, 1.f);
	}



	memcpy(&MyMat.r[3], &PlayerPos, sizeof(_float4));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(m_Distance, MyMat));

	return S_OK;
}

_bool CCamera_Fly::Update_MouseMove(_double TimneDelta, _long MouseMove)
{
	_matrix MyMat = m_pTransform->Get_WorldMatrix();

	_vector	PlayerPos = XMLoadFloat4(&*(_float4*)&m_pPlayerMatrix->m[3]);
	_vector MyPos = m_pTransform->Get_State(CTransform::STATE_POSITION) - PlayerPos;

	_float Dot = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Normalize(MyPos)));

	if (0.25 >= Dot)
	{
		Dot = 0.23f;
		m_bIsCheckDot[0] = true;
	}
	if (0.99 <= Dot)
	{
		Dot = 0.99f;
		m_bIsCheckDot[1] = true;
	}


	if (true == m_bIsCheckDot[0])
	{
		if (0 < MouseMove)
		{
			m_bIsCheckDot[0] = false;
			return true;
		}
		else
			return false;
	}
	if (true == m_bIsCheckDot[1])
	{
		if (0 > MouseMove)
		{
			m_bIsCheckDot[1] = false;
			return true;
		}
		else
			return false;
	}

	return true;
}


CCamera_Fly * CCamera_Fly::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Fly* pInstance = new CCamera_Fly(pDevice, pDeviceContext);

	if (pInstance->Native_Construct_Prototype())
	{
		MSGBOX("Failed to Create CameraFly");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Fly::Clone(void* pArg, _uint iMyLevelIndex)
{
	CCamera_Fly* pInstance = new CCamera_Fly(*this);

	if (pInstance->Native_Construct(pArg, iMyLevelIndex))
	{
		MSGBOX("Failed to Create CameraFly");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Fly::Free()
{
	__super::Free();

}
