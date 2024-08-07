#include "..\public\WalkState.h"



CWalkState::CWalkState()
{
}

void CWalkState::Set_KeyInput(CModel * pModel, _int iKeyState)
{
	if (nullptr == pModel)
		return;

	if (GetKeyState(iKeyState) & 0x8000)
	{
		//점프한다
		pModel->Set_Animation(1);
	}

}

void CWalkState::Anim_Update(CModel * pModel, _double TimeDelta)
{
	pModel->Update(TimeDelta);
}

CWalkState * CWalkState::Create()
{
	CWalkState* pInstance = new CWalkState;
	return pInstance;
}

void CWalkState::Free()
{
	__super::Free();
}
