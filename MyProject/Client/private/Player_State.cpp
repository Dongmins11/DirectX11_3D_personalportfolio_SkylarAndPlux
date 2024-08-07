#include "stdafx.h"
#include "..\public\Player_State.h"
#include "Player_AnimState.h"
#include "Rock.h"
#include "JetPack_Ui.h"
#include "GameInstance.h"

CPlayer_State::CPlayer_State()
{
}

void CPlayer_State::Set_StateInfo(CRock* pPlayer, CColider* pAABB, CTransform* pTransform, CModel* pModel, CColider* pSphere, CColider* pHandSphere, _uint iMyLevel)
{
	m_pPlayer = pPlayer;
	m_pAABB = pAABB;
	m_pTransform = pTransform;
	m_pModel = pModel;
	m_pSphere = pSphere;
	m_pHandSphere = pHandSphere;
	m_iLevelIndex = iMyLevel;


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	m_pJetPack_Ui = dynamic_cast<CJetPack_Ui*>(pGameInstance->Get_GameObject(m_iLevelIndex, L"Clone_JetPack_Ui"));
	if (nullptr == m_pJetPack_Ui)
	{
		MSGBOX("Failed to Finder JetPackUi : Form Player StateFuntion : Set_StateInfo");
		return;
	}
	Safe_AddRef(m_pJetPack_Ui);

	Safe_Release(pGameInstance);

}
void CPlayer_State::Free()
{
	Safe_Release(m_pJetPack_Ui);
}
