#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class Colider;
END


BEGIN(Client)
class CBallHole final : public CGameObject
{
private:
	explicit CBallHole(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBallHole(const CBallHole& rhs);
	virtual ~CBallHole() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Player_Collision_Check(_double TimeDelta);
private:
	HRESULT Main_Animation(_double TimeDelta);
	HRESULT Rotation_Change_First(_double TimeDelta);
	HRESULT Rotation_Change_Second(_double TimeDelta);
	HRESULT Rotation_Change_Third(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
private:
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pLTTransformCom = nullptr;
	CTransform*		m_pLUTransformCom = nullptr;
	CTransform*		m_pRUTransformCom = nullptr;
	CTransform*		m_pLDTransformCom = nullptr;
	CTransform*		m_pRDTransformCom = nullptr;
private:
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pLTModelCom = nullptr;
	CModel*			m_pLUModelCom = nullptr;
	CModel*			m_pRUModelCom = nullptr;
	CModel*			m_pLDModelCom = nullptr;
	CModel*			m_pRDModelCom = nullptr;
private:
	CColider*		m_pAABB = nullptr;
	CColider*		m_pSphereCom = nullptr;
private:
	_double			m_dTimeAcc = 0.0;
	_double			m_dCheckTime = 0.0;
	_float			m_fBackUp_RotationX = 0.f;
private:
	_bool			m_bFirstCheck = false;
	_bool			m_bThirdCheck = false;
private:
	_bool			m_bChangeCheck = false;
	_bool			m_bPlayer_CollisionCheck = false;
private:
	_bool			m_bSkillCheck = false;
private:
	_bool			m_bSoundCheck = false;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CBallHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END