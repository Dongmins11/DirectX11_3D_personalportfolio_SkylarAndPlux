#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class Colider;
END


BEGIN(Client)
class CNpcSphere final : public CGameObject
{
private:
	explicit CNpcSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNpcSphere(const CNpcSphere& rhs);
	virtual ~CNpcSphere() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render();
public:
	void	Set_BossDeadCheck(_bool bCheck) { m_bBossEndCheck = bCheck; }
private:
	HRESULT Sphere_Dead(_double TimeDelta);
private:
	_float			m_fTimeDelta = 0.f;
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
private:
	_bool			m_bBossEndCheck = false;
	_bool			m_bDeadCheck = false;
	_float			m_fAlpha = 0.f;
	_double			m_dDeadTime = 0.0;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CNpcSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END