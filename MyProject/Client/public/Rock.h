#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Sound_Manager.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CModel;
class CNavigation;
class CTransform;
class CColider;
END

BEGIN(Client)
class CHp_Ui;
class CPlayer_AnimState;
class CRopeRobot;
class CRock final : public CGameObject
{
public:
	enum PLAYER_LOOK {
		PLAYER_LOOK_FRONT, PLAYER_LOOK_FRONTLEFT, PLAYER_LOOK_FRONTRIGHT,
		PLAYER_LOOK_BACK, PLAYER_LOOK_BACKLEFT, PLAYER_LOOK_BACKRIGHT,
		PLAYER_LOOK_LEFT, PLAYER_LOOK_RIGHT, PLAYER_LOOK_END
	};
public:
	typedef struct tagPlayerCollisionDesc
	{
		_bool	bMainCollision;
		_bool	bCollisionX;
		_bool	bCollisionY;
		_bool	bCollisionZ;
		_float	fDisX;
		_float	fDisY;
		_float	fDisZ;
	}PLAYERCOLLISIONDESC;

private:
	CRock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRock(const CRock& rhs);
	virtual ~CRock() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	PLAYER_LOOK	Get_PlayerLook();
public:
	void	Set_Bounce(_float3 vDir, _float fPower, _bool bBounceCheck);
private:
	HRESULT Bouns_Pos(_double TimeDelta);
public:/*For.AnimMotion*/
	_uint	Get_CurrentAnimation() { return m_iCurMotion; }
	void	Set_CurrentAnimation(_uint iCurMotion) { m_iCurMotion = iCurMotion; }
public:/*For.Collider*/
	void	Set_PlayerCollision(const PLAYERCOLLISIONDESC& tPlayerCollisionDesc) { m_tPlayer_CollisionDesc = tPlayerCollisionDesc; }
	PLAYERCOLLISIONDESC* Get_Player_Collision() { return &m_tPlayer_CollisionDesc; }
public:
	_float4x4		Get_Player_ColiderFloat4x4() { return m_HandColiderMat; }
	const _float4x4*Get_Player_ColiderFloat4x4ptr() { return &m_HandColiderMat; }
	_matrix			Get_Player_ColiderMat() { return XMLoadFloat4x4(&m_HandColiderMat); }
public:
	void		Set_RopeObject(CRopeRobot* pRopeObject);
	_bool		Get_RopeCheck() { return m_bIsRopeCheck; }
public:
	void		Set_DeadAnimation();
public:
	void		Set_NpcCage(CTransform* pTargetTransform,CGameObject* pTargetObject);
public:/*For. Hit*/
	_bool		Get_HitCheck();
public:/*For. Dead*/
	void	Set_DeadCheck(_bool bDeadCheck) { m_bIsDeadCheck = bDeadCheck; }
	void	Set_ShutDownCheck(_bool bShutDownCheck) { m_bShutDownCheck = bShutDownCheck; }
	_bool	Get_ShutDownCheck() { return m_bShutDownCheck; }
	_bool	Get_DeadCheck() { return m_bIsDeadCheck; }
public:
	void	Set_SavePosition(_float4 vPos) { m_vSavePosition = vPos; }
	_vector	Get_SavePosition() { return XMLoadFloat4(&m_vSavePosition); }
public:
	HRESULT	Magenetic_SkillOn();
	HRESULT	Magenetic_SkillDown(_bool bCheck);
public:
	void	Set_BossEndCheck(_bool bCheck) { m_bBossEndCheck = bCheck; }
	_bool	Get_BossEndCheck() { return m_bBossEndCheck; }
public:
	void	Hp_Attack(_int iAttack) { m_iHp -= iAttack; }
	void	Hp_Heal(_uint iHeal) { m_iHp += iHeal; }
public:
	void	Set_All_HP_RenderCheck(_bool bCheck) { m_bAllHpRenderCheck = bCheck; }
private:
	HRESULT Collision_Check();
	HRESULT BackUp_Pos();
	HRESULT Collision_Hit_Check(_double TimeDelta);
private:
	HRESULT HP_UiManager(_double TimeDelta);
private:/*For.Jump*/
	void	Jump(_double TimeDelta);
	void	Gravity(_double TimeDelta);
private:/*For.Collider*/
	void	Player_ColliderSet();
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
	CPlayer_AnimState*		m_pPlayer_AnimState = nullptr;
private:
	CColider*				m_pHandSphere = nullptr;
private:
	_int					m_iHp = 0;
	_int					m_iMaxHp = 0;
private:
	_uint					m_iCurrntIndex = 0;
	_uint					m_iStackCount = 0;
private:
	_uint					m_iCurMotion = 0;
private:
	_double					m_CoolTimeCheck = 0.0;
private:/*For.Collision*/
	_float4x4				m_MatOffset;
	_float4x4				m_MatPivot;
	const _float4x4*		m_pMatCombined = nullptr;
	const _float4x4*		m_pMatTargetWorld = nullptr;
	_float4x4				m_MatWorldColider;
	PLAYERCOLLISIONDESC		m_tPlayer_CollisionDesc;
private:
	_float4x4				m_HandColiderMat;
private:
	_bool					m_bIsRopeCheck = false;
private:
	_bool					m_bIsDeadCheck = false;
	_bool					m_bShutDownCheck = false;
private:
	_float4					m_vSavePosition;
private:
	_double					m_dPetTime = 0.0;
	_bool					m_bPetCheck = false;
private:
	_bool					m_bBounce = false;
	_float					m_fBouncePower = 0.f;
	_double					m_dBounceTimeAcc = 0.0;
	_float3					m_vBounceDir;
	_float4					m_vBounceInitialPos;
	_float4					m_vBounceGoalPos;
private:
	vector<CHp_Ui*>			m_vecHpUi;
private:
	_bool					m_bBossEndCheck = false;
private:
	_bool					m_bAllHpRenderCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();



public:
	static CRock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END