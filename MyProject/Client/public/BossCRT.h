#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)
class CBoss_AnimState;
class CBoss_Ui;
class CBossCRT final : public CGameObject
{
private:
	explicit CBossCRT(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBossCRT(const CBossCRT& rhs);
	virtual ~CBossCRT() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Hp_Down(_uint iAttack) { m_iHp -= iAttack; }
public:
	void	Set_FaceChange(_bool bCheck) { m_bFaceCheck = bCheck; }
public:
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
	void	Set_AnimCheck(_bool bCheck) { m_bAnimCheck = bCheck; }
public:
	void	Set_HitCheck(_bool bCheck) { m_bHitCheck = bCheck; }
	_bool	Get_HitCheck() { return m_bHitCheck; }
public:
	void	Set_Effect_RenderCheck(_bool bCheck) { m_bEffectRenderCheck = bCheck; }
public:
	void	Set_HitMotion(_double TimeDelta);
private:
	_int	Hit_Crt(_double TimeDelta);
private:
	HRESULT Lerp_LeftHand(_double TimeDelta);
	HRESULT Lerp_RightHand(_double TimeDelta);
private:
	HRESULT Boss_Look(_double TimeDelta);
private:
	HRESULT Anim_Start(_double TimeDelta);
private:
	HRESULT Lerp_Y(_double TimeDelta);
private:
	HRESULT Left_Player_Collision(_double TimeDelta);
	HRESULT Right_Player_Collision(_double TimeDelta);
private:
	_bool			m_bFristCheck = false;
private:
	CRenderer*		m_pRendererCom = nullptr;
private:
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pLeftTransformCom = nullptr;
	CTransform*		m_pRightTransformCom = nullptr;
private:
	CTransform*		m_pTargetTransform = nullptr;
private:
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pTwoModelCom = nullptr;
	CModel*			m_pLeftModelCom = nullptr;
	CModel*			m_pRightModelCom = nullptr;
private:
	CColider*		m_pLeftAABB = nullptr;
	CColider*		m_pRightAABB = nullptr;
	CColider*		m_pAABB = nullptr;
	CColider*		m_pLeftSphere = nullptr;
	CColider*		m_pRightSphere = nullptr;
private:
	CVIBuffer_Rect*	m_pLeftUpBuffer = nullptr;
	CVIBuffer_Rect*	m_pRightUpBuffer = nullptr;
	CVIBuffer_Rect*	m_pLeftDownBuffer = nullptr;
	CVIBuffer_Rect*	m_pRightDownBuffer = nullptr;
private:
	CTransform*		m_pLeftUpTransformCom = nullptr;
	CTransform*		m_pLeftDownTransformCom = nullptr;
	CTransform*		m_pRightUpTransformCom = nullptr;
	CTransform*		m_pRightDownTransformCom = nullptr;
private:
	CTexture*		m_pEffectTexture = nullptr;
private:
	CTexture*		m_pFaceTexture = nullptr;
private:
	CBoss_AnimState*	m_pAnimState = nullptr;
private:
	CBoss_Ui*		m_pBoss_Ui = nullptr;
private:
	_int			m_iHp = 0;
	_vector			m_vDistance;
private:
	_double			m_dRatio = 0.0;
	_bool			m_bAnimCheck = false;
	_bool			m_bNextCheck = false;
	_bool			m_bRenderCheck = false;
	_bool			m_bLastCheck = false;
private:
	_bool			m_YCheck = false;
	_double			m_dLerpTime = 0.0;
	_float			m_fDisY = 0.f;
private:
	_bool			m_bHitCheck = false;
private:
	_bool			m_bBounceCheck = false;
	_double			m_bBounceTimeAcc = 0.0;
private:
	_bool			m_bStartCheck = false;
private:
	_uint			m_iFaceIndex = 0;
	_bool			m_bFaceCheck = false;
private:
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
	_double			m_dTimeAccFilp = 0.0;
private:
	_bool			m_bEffectRenderCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_Component_Buffer();
	HRESULT SetUp_ConstantTable_Buffer();
public:
	static	CBossCRT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END