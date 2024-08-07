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

class CAnimCollider_Object final : public CGameObject
{
private:
	explicit CAnimCollider_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CAnimCollider_Object(const CAnimCollider_Object& rhs);
	virtual ~CAnimCollider_Object() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Collision_Check();
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
	_double						m_dResetCollisionTimeAcc = false;
private:
	_bool						m_bCamCheck = false;

public:
	static CAnimCollider_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END