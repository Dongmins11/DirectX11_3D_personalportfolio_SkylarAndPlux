#include "..\public\Animation_Control_Manager.h"
#include "Key_Manager.h"
#include "Input_Device.h"
#include "Model.h"

IMPLEMENT_SINGLETON(CAnimation_Control_Manager)
CAnimation_Control_Manager::CAnimation_Control_Manager()
	: pKey_Manager(CKey_Manager::Get_Instance())
	, pInput_Device(CInput_Device::Get_Instance())
{
	m_eCurState = STATE_IDLE;

	Safe_AddRef(pKey_Manager);
	Safe_AddRef(pInput_Device);
}

HRESULT CAnimation_Control_Manager::Update_Player_Key_Contral(CModel* pModel, _double TimeDelta)
{
	if (nullptr == pModel)
		return E_FAIL;


	if (FAILED(AniMation_Check(pModel, TimeDelta)))
		return E_FAIL;

	if (FAILED(pModel->Update(TimeDelta)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimation_Control_Manager::AniMation_Check(CModel* pModel, _double TimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Engine::CAnimation_Control_Manager::STATE_IDLE:
			Anim_Idle(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_WALK:
			Anim_Walk(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_JUMP:
			Anim_Jump(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_DOUBLEJUMP:
			Anim_DoubleJump(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_JETPACK:
			Anim_JetPack(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_ATTACK:
			Anim_Attack(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_DOWNATTCK:
			Anim_DownAttack(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_ROPE:
			Anim_Rope(pModel, TimeDelta);
			break;
		case Engine::CAnimation_Control_Manager::STATE_FALL:
			Anim_Fall(pModel, TimeDelta);
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}


	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Idle(CModel * pModel, _double TimeDelta)
{
	if (nullptr == pModel)
		return E_FAIL;

	pModel->Set_Animation(6);

	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Walk(CModel * pModel, _double TimeDelta)
{
	if (nullptr == pModel)
		return E_FAIL;

	pModel->Set_Animation(5);

	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Jump(CModel * pModel, _double TimeDelta)
{
	if (nullptr == pModel)
		return E_FAIL;

	pModel->Set_Animation(0);


	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_DoubleJump(CModel * pModel, _double TimeDelta)
{
	if (nullptr == pModel)
		return E_FAIL;

	pModel->Set_Animation(2);

	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_JetPack(CModel * pModel, _double TimeDelta)
{
	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Attack(CModel * pModel, _double TimeDelta)
{
	


	pModel->Set_Animation(1);



	//pModel->Set_Animation(2);
	//pModel->Set_Animation(3);
	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_DownAttack(CModel * pModel, _double TimeDelta)
{
	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Rope(CModel * pModel, _double TimeDelta)
{
	return S_OK;
}

HRESULT CAnimation_Control_Manager::Anim_Fall(CModel * pModel, _double TimeDelta)
{
	return S_OK;
}

HRESULT CAnimation_Control_Manager::Set_Key_Check(_int InputKey, CModel* pModel)
{
	if (nullptr == pModel)
		return E_FAIL;







	//플레이;어 점프도중에는  WALK나 아이들이안먹어야한다
	return S_OK;
}
void CAnimation_Control_Manager::Free()
{
	Safe_Release(pKey_Manager);
	Safe_Release(pInput_Device);
}
