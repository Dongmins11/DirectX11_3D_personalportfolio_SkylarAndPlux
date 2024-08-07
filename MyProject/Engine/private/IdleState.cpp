#include "..\public\IdleState.h"



CIdleState::CIdleState()
{
}


void CIdleState::Set_KeyInput(CModel* pModel, _int iKeyState)
{
	if (nullptr == pModel)
		return;

	if (FAILED(pModel->Set_Animation(6)))
		return;

}

void CIdleState::Anim_Update(CModel * pModel, _double TimeDelta)
{
	if (FAILED(pModel->Update(TimeDelta)))
		return;
}

CIdleState * CIdleState::Create()
{
	CIdleState* pInstnace = new CIdleState;

	return pInstnace;
}

void CIdleState::Free()
{
	__super::Free();
}

