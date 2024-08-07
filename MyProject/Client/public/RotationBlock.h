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
class CRotationBlock final : public CGameObject
{
private:
	explicit CRotationBlock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRotationBlock(const CRotationBlock& rhs);
	virtual ~CRotationBlock() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Collision_Check(_double TimeDelta);
	HRESULT Player_Collision_Check(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
	CColider*		m_pAABB = nullptr;
	CColider*		m_pBigSphere = nullptr;
	CColider*		m_pSphere = nullptr;
private:
	_double			m_dTimeAcc = 0.0;
	_float			m_fBackUp_RotationX = 0.f;
	_float			m_fBackUp_RotationY = 0.f;
	_float			m_fBackUp_RotationZ = 0.f;
private:
	_bool			m_bFirstCheck = false;
private:
	_bool			m_bChangeCheck = false;
	_bool			m_bPlayer_CollisionCheck = false;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CRotationBlock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END