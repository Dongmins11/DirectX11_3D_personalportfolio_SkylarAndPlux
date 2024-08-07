#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
END


BEGIN(Client)
class CChangeCartoon final : public CGameObject
{
private:
	explicit CChangeCartoon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CChangeCartoon(const CChangeCartoon& rhs);
	virtual ~CChangeCartoon() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Set_VecCollider();
	void	Collision_Check();
	HRESULT Collision_Check_AABB();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	vector<CColider*>	m_vecTargetCollider;
private:
	_bool			m_SetVector = false;
	_bool			m_bRenderCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CChangeCartoon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END