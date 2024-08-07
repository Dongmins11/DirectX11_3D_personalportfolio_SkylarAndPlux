#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CColider;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CDeadColider_Object final : public CGameObject
{
private:
	explicit CDeadColider_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDeadColider_Object(const CDeadColider_Object& rhs);
	virtual ~CDeadColider_Object() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Collision_Check();
	HRESULT Player_DeadCheck();
private:
	HRESULT Setup_Component_Load();
	HRESULT Setup_Component();
	HRESULT Setup_ConstantTable();
private:
	CRenderer*					m_pRendererCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CColider*					m_pAABB = nullptr;
	CVIBuffer_Cube*				m_pVIBufferCom = nullptr;
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	_bool						m_bDeadCheck = false;
	_double						m_bResetTimeAcc = 0.0;

public:
	static CDeadColider_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END