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
class CRotationBall final : public CGameObject
{
private:
	explicit CRotationBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRotationBall(const CRotationBall& rhs);
	virtual ~CRotationBall() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	_bool	Get_BallInCheck() { return m_bBallIn; }
private:
	HRESULT Ball_Check(_double TimeDelta);
	HRESULT Ball_Animation(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
	CColider*		m_pSphere = nullptr;
private:
	_float			m_fRotZ = 0.f;
private:
	_bool			m_bChangeCheck = false;
	_bool			m_bPlayer_CollisionCheck = false;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	_double			m_dTimeAcc = 0.0;
	_bool			m_bBallInCheck = false;
	_bool			m_bSkillCheck = false;
	_bool			m_bSkillCheckTwo = false;
	_bool			m_bBallIn = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();

public:
	static	CRotationBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END