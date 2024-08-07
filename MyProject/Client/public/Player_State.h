#pragma once

#include "Base.h"
#include "Client_AnimationDefine.h"
#include "Player_AnimState.h"
#include "Rock.h"
#include "JetPack_Ui.h"
#include "GameInstance.h"


BEGIN(Engine)
class CTransform;
class CTransform;
class CModel;
END

BEGIN(Client)
class CRock;
class CPlayer_State abstract : public CBase
{
public:
	CPlayer_State();
	virtual ~CPlayer_State() = default;

public:
	virtual void	HandleInput(CPlayer_AnimState* pPlayerAnimState, const CPlayer_AnimState::ANIMDESC& pPlayerAnimDesc) = 0;
	virtual void	Update(CPlayer_AnimState* pPlayerAnimState, _double TimeDelta) = 0;
	virtual void	Reset_Anim(CPlayer_AnimState* pPlayerAnimState) = 0;
	virtual void	Set_Collision(CPlayer_AnimState* pPlayerAnimState) = 0;
	void	Set_StateInfo(CRock* pPlayer,CColider* pAABB, CTransform* pTransform, CModel* pModel, CColider* pSphere, CColider* pHandSphere,_uint iMyLevel);

protected:
	CRock*			m_pPlayer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CModel*			m_pModel = nullptr;
	CColider*		m_pAABB = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pHandSphere = nullptr;
	CJetPack_Ui*	m_pJetPack_Ui = nullptr;
protected:
	_uint		m_iCurMotion = 0;
	_uint		m_iJumpCount = 0;
	_bool		m_bLoopCheck= false;
	_bool		m_bAnimCheck = false;
	_bool		m_bEndCheck = false;
protected:
	_uint		m_iLevelIndex = 0;
public:
	virtual void Free() override;
};

END