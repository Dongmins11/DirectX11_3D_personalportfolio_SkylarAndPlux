#pragma once

#include "Client_Defines.h"
#include "Client_AnimationDefine.h"
#include "Base.h"
#include "Rock.h"
BEGIN(Engine)
class CTransform;
class CModel;
END

BEGIN(Client)
class CRock;
class CPlayer_State;
class CPlayer_AnimState : public CBase
{
public:
	enum PLAYER_STATE {
		PLAYER_STATE_IDLE, PLAYER_STATE_WALK, PLAYER_STATE_JUMP, PLAYER_STATE_DOUBLEJUMP, PLAYER_STATE_ATTACK
		, PLAYER_STATE_DOWNATTACK, PLAYER_STATE_JETPACK, PLAYER_STATE_JETPACK_SKY, PLAYER_STATE_FAILLING, PLAYER_STATE_END
	};

	enum KEY_STATE { KEY_END, KEY_PRESING, KEY_UP, KEY_DOWN };

	enum COLLISION { COLLISION_X, COLLISION_Y, COLLISION_Z, COLLISION_END };
public:
	typedef struct tagAnimDesc
	{
		_uint	iKey[MAX_KEY];
		_uint	eKeyState[MAX_KEY];
		_uint	iKeyCount;
		_uint	iOnceKey;
	}ANIMDESC;

private:
	CPlayer_AnimState();
	virtual ~CPlayer_AnimState() = default;
public:
	HRESULT NativeConstruct(CRock* pPlayer, CTransform* pTransform, CModel* pModel, CColider* pAABB, CColider* pSphere, CColider* pHandSphere,_uint iMyLevel);
public:
	void	SetUp_State(CPlayer_State* pAnimState);
	void	Update(_double TimeDelta);
	void	Key_SetUp(_double TimeDelta);

public:/*For.Jump*/
	void	Set_JumpPower(_float fPower) { m_fJumpPower = fPower; }
	void	Set_JumpState(_bool bJump) { m_bJump = bJump; }
	void	Set_JumpTimeAcc(_float JumpTimeAcc) { m_fJumpTimeAcc = JumpTimeAcc; }
	void	Set_JumpValue(_float JumpValue) { m_fJumpValue = JumpValue; }
	void	Set_GravityPower(_float	GravityPower = 1.2f) { m_fGravityPower = GravityPower; }
	void	Set_MyBackPos(_fvector vMyPos) { XMStoreFloat4(&m_vMyPos, vMyPos); }
	void	Set_FlyTimeAcc(_float TimeAcc) { m_fFlyTimeAcc = TimeAcc; }
	_bool	Get_JumpCheck() { return m_bJump; }
public:/*For. Gravity*/
	void	Set_DownAttack(_bool bDownAttackCheck) { m_bDownAttack = bDownAttackCheck; }
	void	Set_DownAttackGravity(_bool bDownAttackGravity) { m_bDownAttackGravity = bDownAttackGravity; }
	void	Set_CollSpeed(_float fCollSpeed) { m_fCollSpeed = fCollSpeed; }
	void	Set_FallSppedValue(_float fSpeedValue) { m_fFallSpeedValue = fSpeedValue; }
	void	Set_FlyGravity(_bool bFlyGrvityCheck) { m_bFlyGravity = bFlyGrvityCheck; }
	void	Set_Gravity(_bool	bGravity) { m_bGravity = bGravity; }
public:
	_bool	Get_RealGravity() { return m_bRealGravity; }
	_float	Get_GravirySpeed() { return m_fFallSpeedValue; }
public:/*For.MoveCheck*/
	void	Set_MoveCheck(_bool bMoveCheck) { m_bIsMoveCheck = bMoveCheck; }
	void	Set_MoveValue(_double dMoveValue) {m_dMoveValue = dMoveValue; }

public:/*For.RopeCheck*/
	_bool	Get_RopeCoolisionCheck() { return m_bRopeCollisionCheck; }
	_bool	Get_RopeKeyCheck() { return m_bRopeKeyCheck; }
	void	Set_RopeCollisionCheck(_bool bRopeCollisionCheck) { m_bRopeCollisionCheck = bRopeCollisionCheck; }
	void	Set_RopeKeyCheck(_bool RopeKeyCheck) { m_bRopeKeyCheck = RopeKeyCheck; }

public:/*For.CollisionCheck*/
	_bool			Get_CollisionY() { return	m_tPlayerCollisionDesc->bCollisionY; }
	_bool			Get_Gravity() { return m_bGravity; }
	_bool			Get_JumpState() { return m_bJump; }
	_float			Get_TimeDelta() { return m_fFlyTimeAcc; }
	PLAYER_STATE	Get_Player_State() { return m_ePlayerState; }
public:
	CRock::PLAYER_LOOK Get_PlayerLook() { return m_ePlayerLook; }
private:/*For.Move*/
	void			Player_Move(_double TimeDelta);
	void			Move_State();
	void			Set_Look(_double TimeDelta);
	_vector			Player_Move_Direction(_double TimeDelta);
public:/*For.Jump*/
	void	Jump(_double TimeDelta);
	void	Gravity(_double TimeDelta);

public:/*For.Collision*/
	HRESULT Collision_Check();
	HRESULT BackUp_Pos();
	void	Check_Hit(_bool CollisionCheck);
	void	Set_Hit(_bool bCheckHit) { m_bHitAnim = bCheckHit; }

public:/*For.RopeSet*/
	HRESULT	Rope_Anim();
	void	Set_RopeObject(CRopeRobot* pRopeObject);

public:/*For.HitCheck*/
	_bool	Get_HitCheck() { return m_bHitAnim; }
public:
	HRESULT	Dead_Anim();
public:
	void	Set_MageneTicCheck(_bool bCheck) { m_bMageneticCheck = bCheck; }
public:
	HRESULT Magenetic_Change();
	HRESULT Set_Magenetic_Idle(_bool bCheck);
private:
	_bool	Key_Up(_int iKey);
private:
	CRock*				m_pPlayer = nullptr;
	CTransform*			m_pTransform = nullptr;
	CModel*				m_pModel = nullptr;
	CColider*			m_pAABB = nullptr;
	CColider*			m_pSphere = nullptr;
	CColider*			m_pHandSphere = nullptr;
	CPlayer_State*		m_pAnimState = nullptr;
private:
	PLAYER_STATE		m_ePlayerState = PLAYER_STATE_END;
	CRock::PLAYER_LOOK	m_ePlayerLook = CRock::PLAYER_LOOK_END;
	ANIMDESC			m_tAnimDesc;
private:
	_uint				m_iKeyStack = 0;
	_uint				m_iCurrentAnimIndex = 0;
private: /*For.Jump*/
	_bool					m_bJump = false;
	_bool					m_bGravity = false;
	_bool					m_bRealGravity = false;
	_bool					m_bDownAttack = false;
	_bool					m_bDownAttackGravity = false;
	_bool					m_bFlyGravity = false;
	_float					m_fFlyTimeAcc = 0.f;
	_float					m_fGravityPower = 0.f;
	_float					m_fJumpPower = 0.f;
	_float					m_fJumpTimeAcc = 0.f;
	_float					m_fJumpValue = 0.f;
	_float					m_fFallSpeedValue = 0.f;
	_float					m_fMaxFallSpeed = 0.f;
	_float					m_fCollSpeed = 0.f;
	_float					m_fDownAttackTimeAcc = 0.f;
	_float4					m_vMyPos;
	_float4					m_vBackupPos;
	_matrix					m_BackUpMatrix;
private: /*For.Collision*/
	_bool					m_bCollisionCheck = false;
	_bool					m_bColCheck[COLLISION_END] = { false };
	_bool					m_bHitAnim = false;
	_float					m_fColX = 0.f;
	_float					m_fColY = 0.f;
	_float					m_fColZ = 0.f;
	_double					m_dCollisionTimeAcc = 0.0;
	CRock::PLAYERCOLLISIONDESC*	m_tPlayerCollisionDesc;
private: /*For.Move*/
	_bool					m_bIsMoveCheck = false;
	_float3					m_Player_Rotation;
	_double					m_dMoveValue = 0.0;
private: /*For.Rope*/
	_bool					m_bRopeCollisionCheck = false;
	_bool					m_bRopeKeyCheck = false;
private:
	_bool					m_bKeyState[0xFF] = {false};
private:
	_uint					m_iMyLevel = 0;
private:
	_bool					m_bMageneticCheck = false;
public:
	static CPlayer_AnimState* Create(CRock* pPlayer, CTransform* pTransform, CModel* pModel, CColider* pAABB, CColider* pSphere, CColider* pHandSphere,_uint iMyLevel);
	virtual void Free() override;
};

END